
#include "display.h"


__code uint8_t number_0[] = {0b00011000, 
                             0b00100100, 
                             0b00100100, 
                             0b00100100, 
                             0b00100100,
                             0b00100100,
                             0b00011000,
                             0b00000000};

__code uint8_t number_1[] = {0b00010000, 
                             0b00011000, 
                             0b00010000, 
                             0b00010000, 
                             0b00010000,
                             0b00010000,
                             0b00111000,
                             0b00000000};

__code uint8_t number_2[] = {0b00011000, 
                             0b00100100, 
                             0b00100000, 
                             0b00010000, 
                             0b00001000,
                             0b00000100,
                             0b00111100,
                             0b00000000};
  
__code uint8_t number_3[] = {0b00011000, 
                             0b00100100, 
                             0b00100000, 
                             0b00011000, 
                             0b00100000,
                             0b00100100,
                             0b00011000,
                             0b00000000};

__code uint8_t number_4[] = {0b00010000, 
                             0b00011000, 
                             0b00010100, 
                             0b00111100, 
                             0b00010000,
                             0b00010000,
                             0b00010000,
                             0b00000000};

__code uint8_t number_5[] = {0b00111100, 
                             0b00000100, 
                             0b00011100, 
                             0b00100000, 
                             0b00100000,
                             0b00100100,
                             0b00011000,
                             0b00000000};

__code uint8_t number_6[] = {0b00011000, 
                             0b00100100, 
                             0b00000100, 
                             0b00011100, 
                             0b00100100,
                             0b00100100,
                             0b00011000,
                             0b00000000};

__code uint8_t number_7[] = {0b00111100, 
                             0b00100000, 
                             0b00010000, 
                             0b00001000, 
                             0b00001000,
                             0b00001000,
                             0b00001000,
                             0b00000000};

__code uint8_t number_8[] = {0b00011000, 
                             0b00100100, 
                             0b00100100, 
                             0b00011000, 
                             0b00100100,
                             0b00100100,
                             0b00011000,
                             0b00000000};

__code uint8_t number_9[] = {0b00011000, 
                             0b00100100, 
                             0b00100100, 
                             0b00111000, 
                             0b00100000,
                             0b00100100,
                             0b00011000,
                             0b00000000};
__code uint8_t number_a[] = {0b00011000, 
                             0b00100100, 
                             0b00100100, 
                             0b00111100, 
                             0b00100100,
                             0b00100100,
                             0b00100100,
                             0b00000000};
__code uint8_t number_b[] = {0b00011100, 
                             0b00100100, 
                             0b00100100, 
                             0b00011100, 
                             0b00100100,
                             0b00100100,
                             0b00011100,
                             0b00000000};
__code uint8_t number_c[] = {0b0001100, 
                             0b00100100, 
                             0b00000100, 
                             0b00000100, 
                             0b00000100,
                             0b00100100,
                             0b00011000,
                             0b00000000};
__code uint8_t number_d[] = {0b00001100, 
                             0b00010100, 
                             0b00100100, 
                             0b00100100, 
                             0b00100100,
                             0b00010100,
                             0b00001100,
                             0b00000000};
__code uint8_t number_e[] = {0b00111100, 
                             0b00000100, 
                             0b00000100, 
                             0b00111100, 
                             0b00000100,
                             0b00000100,
                             0b00111100,
                             0b00000000};
__code uint8_t number_f[] = {0b00111100, 
                             0b00000100, 
                             0b00000100, 
                             0b00111100, 
                             0b00000100,
                             0b00000100,
                             0b00000100,
                             0b00000000};





extern unsigned int display_counter = 0;
extern unsigned int display_timeseconds = 0;

uint8_t display_buffer[DISPNUM];
uint8_t display_digits[DISPNUM]; 


void setTime(unsigned int minutes, unsigned int seconds){
  display_timeseconds = seconds + minutes*60;
  fillBufferTime();
}

void decrementTime(unsigned int seconds){
  if(display_timeseconds < seconds){
    display_timeseconds = 0;
  } else {
    display_timeseconds -= seconds;
  }
  fillBufferTime();
  
}

void incrementTime(unsigned int seconds){
  if(display_timeseconds + seconds > 5999){
    display_timeseconds = 5999;
  } else {
    display_timeseconds += seconds;
  }
  fillBufferTime();
  
}

void fillBufferTime(){
    unsigned int minutes = display_timeseconds/60;
    unsigned int seconds = display_timeseconds - minutes*60;
    display_digits[3] = seconds%10;
    display_digits[2] = seconds/10;
    display_digits[1] = minutes%10;
    display_digits[0] = minutes/10;
}



void sendCommand(uint8_t msb, uint8_t lsb){
    PIN_low(PIN_DISP_CS);
    SPI_transfer(msb);
    SPI_transfer(lsb);
    PIN_high(PIN_DISP_CS);
}

void sendCommandStealth(uint8_t msb, uint8_t lsb){
    
    SPI_transfer(msb);
    SPI_transfer(lsb);
  
}


void sendBuffer(uint8_t address){
  for(uint8_t i=0; i<DISPNUM; i++){
    sendCommand(address, display_buffer[(DISPNUM-1)-i ]);
  }
  // clean shift registers
  for(uint8_t i=0; i<DISPNUM; i++){
    sendCommandStealth(0x00,0x00);
  }

}

void initialize(){
    //display test
    for(uint8_t i=0; i<DISPNUM; i++){
      display_buffer[i] = 0x00;
    }
    sendBuffer(0x0F);

    //scan limit
    for(uint8_t i=0; i<DISPNUM; i++){
      display_buffer[i] = 0x07;
    }
    sendBuffer(0x0B);

    //Decode mode
    for(uint8_t i=0; i<DISPNUM; i++){
      display_buffer[i] = 0x00;
    }
    sendBuffer(0x09);

    //clear display_digits
    for(uint8_t j=0; j<8; j++){
      //Decode mode
      for(uint8_t i=0; i<DISPNUM; i++){
        display_buffer[i] = 0x00;
      }
      sendBuffer(j+1);
    }
    

    //intensity
    for(uint8_t i=0; i<DISPNUM; i++){
      display_buffer[i] = 0x08;
    }
    sendBuffer(0x0A);

    //turn on
    for(uint8_t i=0; i<DISPNUM; i++){
      display_buffer[i] = 0x01;
    }
    sendBuffer(0x0C);

}

void displayDigits(uint8_t dot){
  for(uint8_t i=0; i<8; i++){
    for(uint8_t j=0; j<DISPNUM; j++){
      //modify display_buffer
      switch(display_digits[j]){
        case 0:
          display_buffer[j] = number_0[7-i];
          break;
        case 1: 
          display_buffer[j] = number_1[7-i];
          break;
        case 2: 
          display_buffer[j] = number_2[7-i];
          break;
        case 3: 
          display_buffer[j] = number_3[7-i];
          break;
        case 4: 
          display_buffer[j] = number_4[7-i];
          break;
        case 5: 
          display_buffer[j] = number_5[7-i];
          break;
        case 6: 
          display_buffer[j] = number_6[7-i];
          break;
        case 7: 
          display_buffer[j] = number_7[7-i];
          break;
        case 8: 
          display_buffer[j] = number_8[7-i];
          break;
        case 9: 
          display_buffer[j] = number_9[7-i];
          break;
        case 10: 
          display_buffer[j] = number_a[7-i];
          break;
        case 11: 
          display_buffer[j] = number_b[7-i];
          break;
        case 12: 
          display_buffer[j] = number_c[7-i];
          break;
        case 13: 
          display_buffer[j] = number_d[7-i];
          break;
        case 14: 
          display_buffer[j] = number_e[7-i];
          break;
        case 15: 
          display_buffer[j] = number_f[7-i];
          break;
      }
    }
    
    if(dot){
       if((i==2)||(i==3)||(i==5)||(i==6)){
        display_buffer[1] = display_buffer[1]|0b10000000;
        //display_buffer[2] = display_buffer[2]|0b00000001;
      }
    }
   
    
    sendBuffer(i+1);
  }
}


void updateDigits(){
    display_digits[0] = display_counter/1000;
    display_digits[1] = (display_counter - 1000*display_digits[0])/100;
    display_digits[2] = (display_counter - 1000*display_digits[0] - 100*display_digits[1])/10;
    display_digits[3] = display_counter%10;
}

