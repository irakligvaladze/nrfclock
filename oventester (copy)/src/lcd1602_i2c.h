/*
  This Library is written for HD44780 LCD with PCF8574 I2C IO expander
  Author: Irakli Gvaladze
  Date:  February 2024
  
  PCF8574    HD44780
       P0    RS
       P1    R/W
       P2    E
       P3    *LED+
       P4    D4
       P5    D5
       P6    D6
       P7    D7
*/

#pragma once

#include "i2c.h"
#include "delay.h"


static unsigned char p_write;


void LCD1602_I2C_write(unsigned char data);
void LCD1602_I2C_latch();
void LCD1602_I2C_write4bit(unsigned char data);
void LCD1602_I2C_writeByte(unsigned char data);
void LCD1602_I2C_writeCommand(unsigned char data);
void LCD1602_I2C_writeData(unsigned char data);
void LCD1602_I2C_setPosition(unsigned char x, unsigned char y);
void LCD1602_I2C_print(const char *str);
void LCD1602_I2C_clear();
void LCD1602_I2C_setBacklight(unsigned char data);
void LCD1602_I2C_begin();



