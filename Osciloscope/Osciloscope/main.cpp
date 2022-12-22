/*
 * Osciloscope.cpp
 *
 * Created: 10.11.2022 15:15:34
 * Author : dimas
 */ 

#include "main.h"

char *ftoa(float f)
{
	static char        buf[17];
	char *            cp = buf;
	unsigned long    l, rem;

	if(f < 0) {
		*cp++ = '-';
		f = -f;
	}
	l = (unsigned long)f;
	f -= (float)l;
	rem = (unsigned long)(f * 1e6);
	sprintf(cp, "%lu.%6.6lu", l, rem);
	return buf;
}

void TransmitFloat(float num){
	unsigned char *ptr;
	char i;
	ptr = (unsigned char *)&num;
	
	for (i=0;i<4;i++)
	USART_Transmit(*(ptr++));
}

void TransmitString(const char* str) {
	while(*str != '\0') {
		_delay_us(200);
		USART_Transmit(*str);
		str++;
	}
}

int main()
{
	USART_Init(8); //12 - 9600 8 - 115200
	ADC_Init(); //Инициализируем АЦП
	wire_set(8000000, 100000); // тактовая частота контроллера, частота шины I2C
	
	LCD_Init();
	Clear(); // очистка экрана
	led(1);  // включение и отключение подсветки экрана

	Curs(0,0);
	PrintString("OVCILOSCOPE v1.0");
	
	while(1)
	{
		//char value[10];
		
		int adc_conver_value = ADC_convert();
		
		float adc_value = (float) adc_conver_value * 5.f / 1023.f;
		
		TransmitFloat(adc_value); //Передаем при включении
		//TransmitString("OK");
		//TransmitString(ftoa(adc_value));
		//USART_Transmit('\n');
		//USART_Transmit('H');'
		
		Curs(1,0);
		PrintString("CURRENT VALUE: ");
		PrintString(ftoa(adc_value));
		_delay_ms(20);
	}
}

