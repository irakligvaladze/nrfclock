/* I2C driver for 8051
 * Author: Irakli Gvaladze
 * Date: February 2024
 */
#pragma once

#include <stdint.h>
#include "ch554.h"
#include "gpio.h"
#include "config.h"
#include "delay.h"
 
#define I2C_ADD_SIZE 4
#define i2c_delay 0
#define LCD_ADDR 0

static __code uint8_t addrI2C[I2C_ADD_SIZE] = {0x27,0x20,0x30,0x30};

inline void I2C_start()
{
  PIN_high(PIN_SDA);
  DLY_us(i2c_delay);
  PIN_high(PIN_SCL); 
  DLY_us(i2c_delay);
  PIN_low(PIN_SDA); 
  DLY_us(i2c_delay);
  PIN_low(PIN_SCL);
  DLY_us(i2c_delay);
}


inline void I2C_stop()
{
  PIN_low(PIN_SDA); 
  DLY_us(i2c_delay);
  PIN_high(PIN_SCL); 
  DLY_us(i2c_delay);
  PIN_high(PIN_SDA); 
  DLY_us(i2c_delay);
}

inline void I2C_restart(){
	PIN_high(PIN_SDA);
	DLY_us(i2c_delay); 
	PIN_high(PIN_SCL);
	DLY_us(i2c_delay); 
	PIN_low(PIN_SDA); 
	DLY_us(i2c_delay);
	PIN_low(PIN_SCL); 
}

inline void I2C_send_ack(){
	PIN_low(PIN_SDA); 
	DLY_us(i2c_delay);
	PIN_high(PIN_SCL); 
	DLY_us(i2c_delay);
	PIN_low(PIN_SCL); 
	DLY_us(i2c_delay);
}

inline void I2C_send_nack(){
	PIN_low(PIN_SDA); 
	DLY_us(i2c_delay);
	PIN_high(PIN_SCL); 
	DLY_us(i2c_delay);
	PIN_low(PIN_SCL); 
	DLY_us(i2c_delay);

}

inline void I2C_write(uint8_t data){
	for(uint8_t i=0; i < 8; i++){
		if(data & 0x80){
			PIN_high(PIN_SDA);
		} else {
			PIN_low(PIN_SDA);
		}
		DLY_us(i2c_delay);
		PIN_high(PIN_SCL); 
		DLY_us(i2c_delay);
		PIN_low(PIN_SCL); 
		DLY_us(i2c_delay);
		data <<= 1;
	}
	PIN_high(PIN_SCL); 
	DLY_us(i2c_delay);
	PIN_low(PIN_SCL); 
	DLY_us(i2c_delay);
	


}


inline uint8_t I2C_read(){
	uint8_t data = 0;
	for(int i=0; i < 8; i++){
		PIN_high(PIN_SCL); 
		
		data <<= 1;
		data |= PIN_SDA;
		PIN_low(PIN_SCL); 
		
	}
	return data;
}

inline void I2C_write_byte(uint8_t addrNum, uint8_t data)
{
  I2C_start();
  I2C_write(addrI2C[addrNum]<<1);
  I2C_write(data);
  I2C_stop();
}

inline uint8_t I2C_read_byte(uint8_t addrNum)
{
  uint8_t rec;
  I2C_start();
  I2C_write((addrI2C[addrNum] << 1)+1);
  rec=I2C_read();
  //I2C_send_nack();
  I2C_stop();
  return rec;
}