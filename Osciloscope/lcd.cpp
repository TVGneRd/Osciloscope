/*
 * lcd.c
 *
 * Created: 10.11.2022 14:57:43
 *  Author: dimas
 */ 
#include "lcd.h"

unsigned char led_b;

void led(bool led_on_off){
	if(led_on_off==1){i2c_write_1bit(led_b |= (1<<LED));}
	if(led_on_off==0){i2c_write_1bit(led_b &=~ (1<<LED));}
}

void Write(unsigned char addr_w, unsigned char wr1,unsigned char wr2,unsigned char wr3,unsigned char wr4,unsigned char wr5,unsigned char wr6,unsigned char wr7,unsigned char wr8){
lcd(0b01000000|addr_w*8);PrintChar(wr1);PrintChar(wr2);PrintChar(wr3);PrintChar(wr4);PrintChar(wr5);PrintChar(wr6);PrintChar(wr7);PrintChar(wr8);}

void PrintInt(int data_int){char str[6];PrintString(itoa(data_int, str, 10));}

void PrintFloat(float data_float, unsigned char dp){
	char str[8];
	if(data_float<0){data_float=-data_float;PrintChar(0b101101);PrintString(itoa((int)data_float, str, 10));}
	else{PrintString(itoa((int)data_float, str, 10));}
	int float10 = round((data_float - (int)data_float)*pow(10,dp));
	PrintChar(0b101110);
	PrintString(itoa(float10, str, 10));
}

void LCD_Init(){
	lcd(0x03);_delay_us(4500);
	lcd(0x03);_delay_us(4500);
	lcd(0x03);_delay_us(200);
	lcd(0b00000010);_delay_ms(5);
	lcd(0b00001100);_delay_ms(5);
	lcd(0b00000001);_delay_ms(5);
	
	lcd(0b00101100);_delay_ms(5); // дополнительная строка, исправляет баг с одной строкой в протеусе
}

void Clear(){lcd(0b00000001);}

void Curs(unsigned char row, unsigned char column){
	
	switch (row)
	{
		case 1:
			column += 0x40;
			break;
		case 2:
			column += 0x14;
			break;
		case 3:
			column += 0x54;
			break;
	}
		
	lcd(0b10000000 + column);
}

void PrintString(const char* str) {while(*str != '\0') {PrintChar(*str);str++;}}

void PrintChar(const char chr) {lcdSend(false, (unsigned char)chr);}

void e_pin(){
	i2c_write_1bit(led_b |= (1<<E));
	_delay_us(200);
	i2c_write_1bit(led_b &= ~(1<<E));
}

void lcd(unsigned char sett) {lcdSend(true, sett);}

void lcdSend(bool rs, unsigned char data) {
	if(rs==0){led_b |= (1<<RS);} else {led_b &= ~(1<<RS);}//RS
	_delay_us(200);
	if(((data >> 7) & 1) ==1){i2c_write_1bit(led_b |= (1<<7));} else {i2c_write_1bit(led_b &= ~(1<<7));}
	if(((data >> 6) & 1) ==1){i2c_write_1bit(led_b |= (1<<6));} else {i2c_write_1bit(led_b &= ~(1<<6));}
	if(((data >> 5) & 1) ==1){i2c_write_1bit(led_b |= (1<<5));} else {i2c_write_1bit(led_b &= ~(1<<5));}
	if(((data >> 4) & 1) ==1){i2c_write_1bit(led_b |= (1<<4));} else {i2c_write_1bit(led_b &= ~(1<<4));}
	e_pin();
	if(((data >> 3) & 1) ==1){i2c_write_1bit(led_b |= (1<<7));} else {i2c_write_1bit(led_b &= ~(1<<7));}
	if(((data >> 2) & 1) ==1){i2c_write_1bit(led_b |= (1<<6));} else {i2c_write_1bit(led_b &= ~(1<<6));}
	if(((data >> 1) & 1) ==1){i2c_write_1bit(led_b |= (1<<5));} else {i2c_write_1bit(led_b &= ~(1<<5));}
	if(((data >> 0) & 1) ==1){i2c_write_1bit(led_b |= (1<<4));} else {i2c_write_1bit(led_b &= ~(1<<4));}
	e_pin();
}

void i2c_write_1bit(unsigned char i2c_reg){
	wire_start_w(ADDR_LED);
	wire_write(i2c_reg);
	wire_stop();
}