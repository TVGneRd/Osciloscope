#ifndef LCD_H_
#define LCD_H_
/*
 * lcd.h
 *
 * Created: 10.11.2022 14:59:39
 * Author: dimas
 */ 

#define RS           0
#define E            2
#define LED          3
#define ADDR_LED     0x27

#define F_CPU 8000000UL

#include "main.h"

extern unsigned char led_b;

void led(bool led_on_off);

void Write(unsigned char addr_w, unsigned char wr1,unsigned char wr2,unsigned char wr3,unsigned char wr4,unsigned char wr5,unsigned char wr6,unsigned char wr7,unsigned char wr8);

void PrintInt(int data_int);

void PrintFloat(float data_float, unsigned char dp);

void LCD_Init();

void Clear();

void Curs(unsigned char str, unsigned char mesto);

void PrintString(const char* str);

void PrintChar(const char chr);

void e_pin();

void lcd(unsigned char sett);

void lcdSend(bool rs, unsigned char data);

void i2c_write_1bit(unsigned char i2c_reg);

#endif