/*
 * i2c.h
 *
 * Created: 10.11.2022 15:07:23
 *  Author: dimas
 */ 
#ifndef I2C_H_
#define I2C_H_

#include "main.h"

void wire_set(long f_clock, long i2c_clock);

void wire_start_w(unsigned char i2c_addr);

void wire_write(unsigned char i2c_data);

void wire_stop();

#endif