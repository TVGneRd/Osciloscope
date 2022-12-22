/*
 * Osciloscope.cpp
 *
 * Created: 10.11.2022 15:15:34
 * Author : dimas
 */ 

#include "main.h"

extern unsigned int adc_value = 5;
bool adc_value_updated = false;
bool usart_ready = false;

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
	rem = (unsigned long)(f * 1e3);
	sprintf(cp, "%lu.%3.3lu", l, rem);
	return buf;
}

ISR(USART_RXC_vect)
{
	if(usart_ready) { UDR; return;}
	
	if(UDR == 0){
		USART_Transmit(0xFF);
		usart_ready = true;
		
		Clear();
		Curs(0,0);
		PrintString("USART CONNECTED");
	}
}

ISR(ADC_vect)
{
	char high_adc=0, low_adc=0;
	
	low_adc = ADCL;
	high_adc = ADCH;//Верхняя часть регистра ADC должна быть считана последней, иначе не продолжится преобразование
	adc_value = high_adc * 256 + low_adc;
	
	ADCSRA |= (1 << ADSC);
	
	adc_value_updated = true;
}

void TransmitInt(unsigned int num){
	unsigned char *ptr = (unsigned char *)&num;
	
	USART_Transmit(*(ptr++));
	USART_Transmit(*(ptr));
}

void loop(){
	unsigned int i = 0;
	
	sei();
	
	while(1)
	{
		//Передаем при включении
		
		if(i == 60){
			
			Clear(); // очистка экрана
			cli();
			Curs(1,0);
			PrintString("VOLTAGE: ");
			PrintString(ftoa(adc_value / 1023.f * 5.f));
			sei();
			
			i = 0;
		}
		
		if(usart_ready && adc_value_updated){
			adc_value_updated = false;
			TransmitInt((unsigned int) adc_value);
		}
		
		_delay_ms(10);
		
		i++;
	}
}

int main()
{
	_delay_ms(1000);
	
	USART_Init(8); //12 - 9600 8 - 115200

	ADC_Init(); //Инициализируем АЦП
	LCD_Init();
	
	Clear(); // очистка экрана
	led(1);  // включение и отключение подсветки экрана
	Curs(0,0);
	
	PrintString("WAITING...");

	wire_set(8000000, 100000); // тактовая частота контроллера, частота шины I2C

	loop();
}

