/* I2C driver for 8051
 * Author: Irakli Gvaladze
 * Date: February 2024
 */


#include "i2c.h"

void i2c_begin(unsigned char addrI2C, unsigned int time){
	t = time;
	addrW = addrI2C << 1;
	addrR = addrW + 1;
}
void i2c_start()
{
  SDA=1;
  delay(t);
  SCL=1;
  delay(t);
  SDA=0;
  delay(t);
  SCL=0;
  delay(t);
}


void i2c_stop()
{
  SDA=0;
  delay(t);
  SCL=1;
  delay(t);
  SDA=1;
  delay(t);
}

void i2c_restart(){
	SDA = 1;
	SCL = 1;
	SDA = 0;
	delay(t);
	SCL = 0;
}

void i2c_send_ack(){
	SDA = 0;
	delay(t);
	SCL = 1;
	delay(t);
	SCL = 0;
	delay(t);
}

void i2c_send_nack(){
	SDA = 0;
	delay(t);
	SCL = 1;
	delay(t);
	SCL = 0;
	delay(t);

}

void i2c_write(unsigned char data){
	for(unsigned char i=0; i < 8; i++){
		SDA = (data & 0x80) ? 1 : 0;
		delay(t);
		SCL = 1;
		delay(t);
		SCL = 0;
		delay(t);
		data <<= 1;
	}
	SCL = 1;
	delay(t);
	SCL = 0;
	delay(t);
	


}


unsigned char i2c_read(){
	unsigned char data = 0;
	for(int i=0; i < 8; i++){
		SCL = 1;
		delay(t);
		data <<= 1;
		data |= SDA;
		SCL = 0;
		delay(t);
	}
	return data;
}

void i2c_write_byte(unsigned char data)
{
  i2c_start();
  i2c_write(addrW);
  i2c_write(data);
  i2c_stop();
}

unsigned char i2c_read_byte()
{
  unsigned char rec;
  i2c_start();
  i2c_write(addrR);
  rec=i2c_read();
  //i2c_send_nack();
  i2c_stop();
  return rec;
}
