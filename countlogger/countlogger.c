
#include "src/config.h"                   // user configurations
#include "src/system.h"                   // system functions
#include "src/gpio.h"                     // GPIO functions
#include "src/delay.h"                    // delay functions
#include "src/flash.h"                    // data flash functions
#include "src/usb_cdc.h"                  // USB-CDC serial functions

#define BUFFER_SIZE 32
// Prototypes for used interrupts
void USB_interrupt(void);
void USB_ISR(void) __interrupt(INT_NO_USB) {
  USB_interrupt();
}

// Global variables
__xdata uint8_t buffer[BUFFER_SIZE];      // rx/tx buffer

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





// ===================================================================================
// Main Function
// ===================================================================================
void main(void) {
  // Variables
  uint8_t buflen;                                   // data length in buffer
  uint8_t bufptr;                                   // buffer pointer

  // Setup
  CLK_config();                                     // configure system clock
  DLY_ms(5);                                        // wait for clock to settle
  CDC_init();                                       // init USB CDC
  WDT_start();

  // Loop
  while(1) {

    buflen = CDC_available();                       // get number of bytes in CDC IN
    if(buflen) {                                    // something coming in via USB?
      bufptr = 0;                                   // reset buffer pointer
      if(buflen > BUFFER_SIZE) buflen = BUFFER_SIZE;// restrict length to max payload
      while(buflen--) buffer[bufptr++] = CDC_read();// get data from CDC
      PIN_low(PIN_LED);
      
    }

    
    
    
    PIN_high(PIN_LED);                              // switch off LED
    WDT_reset();
    
  }
}
