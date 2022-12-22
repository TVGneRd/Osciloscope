/*
 * i2c.c
 *
 * Created: 10.11.2022 14:56:29
 *  Author: dimas
 */ 

#include "i2c.h"

////// I2C ////////////
void wire_set(long f_clock, long i2c_clock){
	TWBR = (((f_clock)/(i2c_clock)-16 )/2) ;
	TWSR = 0;
}

void wire_start_w(unsigned char i2c_addr){
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	TWDR = i2c_addr << 1;
	TWCR = (1<<TWINT) | (1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
}

void wire_write(unsigned char i2c_data){
	TWDR = i2c_data;
	TWCR = (1<<TWINT) | (1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
}

void wire_stop(){
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
}