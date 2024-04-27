#pragma once
#include <stdint.h>
#include "gpio.h"
#include "config.h"
#include "spi.h"


extern __code uint8_t number_0[];
extern __code uint8_t number_1[];
extern __code uint8_t number_2[];
extern __code uint8_t number_3[];
extern __code uint8_t number_4[];
extern __code uint8_t number_5[];
extern __code uint8_t number_6[];
extern __code uint8_t number_7[];
extern __code uint8_t number_8[];
extern __code uint8_t number_9[];
extern __code uint8_t number_a[];
extern __code uint8_t number_b[];
extern __code uint8_t number_c[];
extern __code uint8_t number_d[];
extern __code uint8_t number_e[];
extern __code uint8_t number_f[];

extern unsigned int display_counter;
extern unsigned int display_timeseconds;

#define DISPNUM 4

extern uint8_t display_buffer[DISPNUM];
extern uint8_t display_digits[DISPNUM]; 

void setTime(unsigned int minutes, unsigned int seconds);
void decrementTime(unsigned int sec);
void incrementTime(unsigned int sec);
void fillBufferTime();
void sendCommand(uint8_t msb, uint8_t lsb);
void sendCommandStealth(uint8_t msb, uint8_t lsb);
void sendBuffer(uint8_t address);
void initialize();
void displayDigits(uint8_t dot);
void updateDigits();