
/* I2C driver for 8051
 * Author: Irakli Gvaladze
 * Date: February 2024
 */
#pragma once

#define SDA P3_4
#define SCL P3_5


#include "ch554.h"


unsigned char addrW;
unsigned char addrR;

unsigned int t = 0;

void i2c_begin(unsigned char addrI2C);
void i2c_start();
void i2c_stop();
void i2c_restart();
void i2c_send_ack();
void i2c_send_nack();
void i2c_write(unsigned char data);
unsigned char i2c_read();
void i2c_write_byte(unsigned char data);
unsigned char i2c_read_byte();