
// Libraries
#include "src/config.h"                   // user configurations
#include "src/system.h"                   // system functions
#include "src/gpio.h"                     // GPIO functions
#include "src/delay.h"                    // delay functions
#include "src/flash.h"                    // data flash functions
#include "src/usb_cdc.h"                  // USB-CDC serial functions
#include "src/nrf24l01.h"                 // nRF24L01+ functions
#include "src/uart.h"                     // UART header file
#include "src/display.h"                  // Header for display matrix
#include "src/spi.h"
#include "src/adc.h"
#include "src/speaker.h"




// Prototypes for used interrupts
void USB_interrupt(void);
void USB_ISR(void) __interrupt(INT_NO_USB) {
  USB_interrupt();
}

// Global variables
__xdata uint8_t buffer[NRF_PAYLOAD];      // rx/tx buffer
__code uint8_t passcode[4] = {1,5,5,2}; 
uint8_t buttonPressed;
uint8_t buttonLast = 0;
uint8_t updateTimer = 0;
uint8_t clockOn = 0;
uint8_t keyboardActive = 0;
uint8_t keylogged[4]; 
uint8_t keynum = 0;
uint8_t keyboardtimeout = 10;

// ===================================================================================
// Print Functions and String Conversions
// ===================================================================================

// Convert byte nibble into hex character and print via CDC
void CDC_printNibble(uint8_t nibble) {
  (nibble <= 9) ? (nibble += '0') : (nibble += ('A' - 10));
  CDC_write(nibble);
}

// Convert byte into hex string and print via CDC
void CDC_printByte(uint8_t value) {
  CDC_printNibble (value >> 4);
  CDC_printNibble (value & 0x0F);
}

// Convert an array of bytes into hex string and print via CDC
void CDC_printBytes(uint8_t *ptr, uint8_t len) {
  while(len--) CDC_printByte(*ptr++);
}

// Convert character representing a hex nibble into 4-bit value
uint8_t hexDigit(uint8_t c) {
  if     ((c >= '0') && (c <= '9')) return(c - '0');
  else if((c >= 'a') && (c <= 'f')) return(c - 'a' + 10);
  else if((c >= 'A') && (c <= 'F')) return(c - 'A' + 10); 
  return 0;
}

// Convert string containing a hex byte into 8-bit value
uint8_t hexByte(uint8_t *ptr) {
  return((hexDigit(*ptr++) << 4) + hexDigit(*ptr));
}

// Convert string containing 5 hex bytes into address array
void hexAddress(uint8_t *sptr, uint8_t *aptr) {
  uint8_t i;
  for(i=5; i; i--) {
    *aptr++ = hexByte(sptr);
    sptr += 2;
  }
}

// Print the current NRF settings via CDC
void CDC_printSettings(void) {
  CDC_println("# nRF24L01+ Configuration:");
  CDC_print  ("# RF channel: "); CDC_printByte (NRF_channel);    CDC_write('\n');
  CDC_print  ("# TX address: "); CDC_printBytes(NRF_tx_addr, 5); CDC_write('\n');
  CDC_print  ("# RX address: "); CDC_printBytes(NRF_rx_addr, 5); CDC_write('\n');
  CDC_print  ("# Data rate:  "); CDC_print(NRF_STR[NRF_speed]);  CDC_println("bps");
  CDC_print  ("# Power rate: "); CDC_print(NRF_STR_PW[NRF_power]);CDC_println("bBm");
}

// ===================================================================================
// Data Flash Implementation
// ===================================================================================

// FLASH write user settings
void FLASH_writeSettings(void) {
  uint8_t i;
  FLASH_update(2, NRF_channel);
  FLASH_update(3, NRF_speed);
  FLASH_update(4, NRF_power);
  for(i=0; i<5; i++) {
    FLASH_update(5+i, NRF_tx_addr[i]);
    FLASH_update(10+i, NRF_rx_addr[i]);
  }
}

// FLASH read user settings; if FLASH values are invalid, write defaults
void FLASH_readSettings(void) {
  uint8_t i;
  uint16_t identifier = ((uint16_t)FLASH_read(1) << 8) | FLASH_read(0);
  if (identifier == FLASH_IDENT) {
    NRF_channel =  FLASH_read(2);
    NRF_speed   =  FLASH_read(3);
    NRF_power   =  FLASH_read(4);
    for(i=0; i<5; i++) {
      NRF_tx_addr[i] = FLASH_read(5+i);
      NRF_rx_addr[i] = FLASH_read(10+i);
    }
  }
  else {
    FLASH_update(0, (uint8_t)FLASH_IDENT);
    FLASH_update(1, (uint8_t)(FLASH_IDENT >> 8));
    FLASH_writeSettings();
  }
}

// ===================================================================================
// Command Parser
// ===================================================================================
void parse(void) {
  uint8_t cmd = buffer[1];                          // read the command
  switch(cmd) {                                     // what command?
    case 'c': NRF_channel = hexByte(buffer + 2) & 0x7F;
              break;
    case 't': hexAddress(buffer + 2, NRF_tx_addr);
              break;
    case 'r': hexAddress(buffer + 2, NRF_rx_addr);
              break;
    case 's': NRF_speed = hexByte(buffer + 2);
              if(NRF_speed > 2) NRF_speed = 2;
              break;
    case 'p': NRF_power = hexByte(buffer + 2);
              if(NRF_power > 3) NRF_power = 3;
              break;
    default:  break;
  }
  NRF_configure();                                  // reconfigure the NRF
  CDC_printSettings();                              // print settings via CDC
  FLASH_writeSettings();                            // update settings in data flash
}

void bufferStringer(uint8_t* str){
  uint8_t i =0;
  while(*str){
    buffer[i] = *str;
    i++;
    str++;
  }
}

uint8_t checkPasscode(){
  uint8_t result = 1;
  for(uint8_t i=0; i<4; i++){
    if(keylogged[i]!=passcode[i]){
      result = 0;
      break;
    }
  }
  return result;
}


// ===================================================================================
// Main Function
// ===================================================================================
void main(void) {
  CLK_config();                                       // configure system clock
  //UART_Init(9600);                                    // UART initialization
  SPI_init();
  initialize();
  setTime(0,0);
  displayDigits(1);                      
  DLY_ms(100);
  //PIN_high(PIN_BUZZER);
  ADC_BUTTONSInit(ADC_SPEED, ADC_CHANNEL);
  SPEAKER_Init();
                                                             
	
  while(1){        
      buttonPressed = ADC_BUTTONSGetButton(ADC_CHANNEL);                                                       
      if((buttonLast!=buttonPressed)&&(buttonPressed!=0)){
        
        if(keyboardActive){
          switch(buttonPressed){
            case 1: clockOn = 1; break;
            case 2: clockOn = 0; break;
            case 3: 
              incrementTime(60); 
              displayDigits(1);
              break;
            case 4: 
              decrementTime(60); 
              displayDigits(1);
              break;
            case 5: 
              setTime(0,0);
              displayDigits(1);
              clockOn = 0;
              break;
            default:
            
          }
        }
        
        if(!keyboardActive){
          if(buttonPressed == passcode[0]){
            keynum = 1;
            keylogged[0] = buttonPressed;
          } else {
            keylogged[keynum] = buttonPressed;
            keynum++;
            if(keynum == 4){
              if(checkPasscode()){
                keyboardActive = 1;
                keyboardtimeout = 2000;
                PIN_low(PIN_LED);
                SPEAKER_Generate(1);
              } else {
                keynum = 0;
                keylogged[0]=0;
                keylogged[1]=0;
                keylogged[2]=0;
                keylogged[3]=0;
              }
            } 
          }
        }
      } 
      buttonLast = buttonPressed;
      if(clockOn){
        updateTimer++;
        
      }
      if(keyboardActive){
        
        keyboardtimeout--;
        if(!keyboardtimeout){
          keylogged[0]=0;
          keylogged[1]=0;
          keylogged[2]=0;
          keylogged[3]=0;
          keyboardActive = 0;
          PIN_high(PIN_LED);
        }
      }
      if(updateTimer >= 40){
        updateTimer = 0;
        decrementTime(1);
        if(display_timeseconds==0){
          clockOn = 0;
          SPEAKER_Generate(1);
        }
        displayDigits(1);
      }
      DLY_ms(25);   

  }
  // while(1) {
  //   //UART0_SendByte('a');
 
  //   display_counter++;
  //   updateDigits();
  //   displayDigits(1);  
    
  //   DLY_ms(1000);
  // }
}