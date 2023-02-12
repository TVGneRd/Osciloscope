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
bool is_DC = true;

int values[128];

#define UPDATE_DELAY 131072UL

enum Modes {RECORDER, GENERATOR};

Modes mode = RECORDER;

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
	rem = (unsigned long)(f * 1e2);
	sprintf(cp, "%lu.%2.2lu", l, rem);
	return buf;
}

ISR(USART_RXC_vect)
{
	if(usart_ready) { UDR; return;}
	
	if(UDR == 0){
		USART_Transmit(0xFF);
		usart_ready = true;
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

bool isDC(){
	int min = INT_MAX;
	int max = 0;
	
	for(int i = 0; i < 128; i++){
		if(values[i] > max) max = values[i];
		else if(values[i] < min) min = values[i];
	}
	
	return max - min < 30;
}

void printRecordingStatus(){
	char uartStr[20] = "UART: ";
	strcat(uartStr, usart_ready ? "CONNECTED" : "DISCONECTED");
	
	Curs(1,0);
	PrintString(uartStr);
	
	char voltageStr[20] = "VOLTAGE: ";
	strcat(voltageStr, ftoa(adc_value / 1023.f * 5.f));
	strcat(voltageStr, isDC() ? " --" : " -_");
	
	Curs(2,0);
	PrintString(voltageStr);
}

void printStatus(){
	Clear();
	
	char modeStr[20] = "Mode: ";
	strcat(modeStr, mode == RECORDER ? "RECORDING" : "GENERATOR");	
	
	Curs(0,0);
	PrintString(modeStr);
	
	printRecordingStatus();
}

void loop(){
	unsigned long int i = 0;
	unsigned long int step = UPDATE_DELAY >> 7;
	sei();
	
	while(1)
	{
		if(mode == GENERATOR){
			
		} else {
			   
		}
		
		//Передаем при включении
		if(i % step == 0){
			values[i / step] = adc_value;
		}
		
		if(i == UPDATE_DELAY){
			cli();
			printStatus();
			sei();
			
			i = -1;
		}
		
		if(usart_ready && adc_value_updated){
			adc_value_updated = false;
			TransmitInt((unsigned int) adc_value);
		}
		
		//_delay_ms(10);
		
		i++;
	}
}

int main()
{
	wire_set(8000000, 100000); // тактовая частота контроллера, частота шины I2C
	
	USART_Init(8); //12 - 9600 8 - 115200
	
	ADC_Init(); //Инициализируем АЦП
	LCD_Init();
	
	Clear(); // очистка экрана
	led(1);  // включение и отключение подсветки экрана
	loop();
}

