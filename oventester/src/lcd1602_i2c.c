#include "lcd1602_i2c.h"


void LCD1602_I2C_write(unsigned char data) {
  I2C_write_byte(LCD_ADDR, data);
}

void LCD1602_I2C_latch() {
  p_write = p_write & 0xF9;
  LCD1602_I2C_write(p_write);
  DLY_ms(1);
  p_write = p_write | 0x04;
  LCD1602_I2C_write(p_write);
  DLY_ms(1);
  p_write = p_write & 0xF9;
  LCD1602_I2C_write(p_write);
  DLY_ms(1);
}

void LCD1602_I2C_write4bit(unsigned char data) {
  p_write &= 0x0F;
  p_write |= data << 4;
  LCD1602_I2C_latch();
}

void LCD1602_I2C_writeByte(unsigned char data) {
  LCD1602_I2C_write4bit(data >> 4);
  LCD1602_I2C_write4bit(data);
}

void LCD1602_I2C_writeCommand(unsigned char data) {
  p_write = p_write & 0xF8;  // RS LOW
  LCD1602_I2C_write(p_write);
  LCD1602_I2C_writeByte(data);
}
void LCD1602_I2C_writeData(unsigned char data) {
  p_write = p_write | 0x01;  // RS HIGH
  LCD1602_I2C_write(p_write);
  LCD1602_I2C_writeByte(data);
}

void LCD1602_I2C_setPosition(unsigned char x, unsigned char y) {
  if (y == 0) {
    LCD1602_I2C_writeCommand(0x80 | x);
  }
  if (y == 1) {
    LCD1602_I2C_writeCommand(0x80 | 0x40 | x);
  }
}
void LCD1602_I2C_print(const char *str) {
  while (*str) {
    LCD1602_I2C_writeData(*str);
    str += 1;
  }
}

void LCD1602_I2C_clear(){
   LCD1602_I2C_writeCommand(0x01);
}

void LCD1602_I2C_setBacklight(unsigned char data) {
  p_write=0;
  if (data) {
    p_write |= 0x08;
    LCD1602_I2C_write(p_write);
  } else {
    p_write &= 0xF7;
    LCD1602_I2C_write(p_write);
  }
  DLY_ms(50);
}

void LCD1602_I2C_begin() {
  LCD1602_I2C_write(0x00);
  LCD1602_I2C_write4bit(0x03);
  DLY_ms(4500);
  LCD1602_I2C_write4bit(0x03);
  DLY_ms(4500);
  LCD1602_I2C_write4bit(0x03);
  DLY_ms(150);
  LCD1602_I2C_write4bit(0x02);

  LCD1602_I2C_write4bit(0x02);
  LCD1602_I2C_write4bit(0x0C);
  LCD1602_I2C_write4bit(0x00);
  LCD1602_I2C_write4bit(0x08);
  LCD1602_I2C_write4bit(0x00);
  LCD1602_I2C_write4bit(0x01);
  LCD1602_I2C_write4bit(0x00);
  LCD1602_I2C_write4bit(0x06);

  LCD1602_I2C_write4bit(0x00);
  LCD1602_I2C_write4bit(0x0F);
  LCD1602_I2C_setBacklight(1);
  
}