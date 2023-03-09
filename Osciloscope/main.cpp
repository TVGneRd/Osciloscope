/*
 * Osciloscope.cpp
 *
 * Created: 10.11.2022 15:15:34
 * Author : dimas
 */ 

#include "main.h"

#define UPDATE_DELAY 8192UL // 2 ^ 18 таков после которых обновляется информация на дисплее
#define BUFFER_SIZE 129 // количество запоимнаемыех значений за период UPDATE_DELAY

extern unsigned int adc_value = 5;
bool adc_value_updated = false;
bool usartReady = false;
bool is_DC = true;

int values[BUFFER_SIZE];

unsigned long int step = UPDATE_DELAY / (BUFFER_SIZE - 1); // ~ 2 ^ 11 тактов - 1 шаг, 2 ^ 18 / BUFFER_SIZE - 1 

enum Modes {RECORDER, GENERATOR_SIN};

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
	if(usartReady) { UDR; return;}
	
	if(UDR == 0x0){
		USART_Transmit(0xFF);
		usartReady = true;
	}
}

ISR(ADC_vect)
{
	if(mode != RECORDER) return;
	
	char high_adc=0, low_adc=0;
	
	low_adc = ADCL;
	high_adc = ADCH;//Верхняя часть регистра ADC должна быть считана последней, иначе не продолжится преобразование
	adc_value = high_adc * 256 + low_adc;
	
	ADCSRA |= (1 << ADSC);
	
	adc_value_updated = true;
}

ISR(INT0_vect)
{
	if(mode == RECORDER) mode = GENERATOR_SIN;
	else if(mode == GENERATOR_SIN) mode = RECORDER;
}


void TransmitInt(unsigned int num){
	USART_Transmit(num >> 8);
	USART_Transmit(num & 0xFF);
} 

bool isDC(){
	int min = INT_MAX;
	int max = 0;
	
	for(int i = 0; i < BUFFER_SIZE; i++){
		if(values[i] > max) max = values[i];
		else if(values[i] < min) min = values[i];
	}
	
	return max - min < 30;
}

void printRecordingStatus(){
	
	char voltageStr[20] = "VOLTAGE: ";
	strcat(voltageStr, ftoa(adc_value / 1023.f * 5.f));
	strcat(voltageStr, isDC() ? " --" : " -_");
	
	Curs(2,0);
	PrintString(voltageStr);
}

void printStatus(){
	Clear();
	
	char modeStr[20] = "Mode: ";
	if(mode == RECORDER){
		strcat(modeStr, "RECORDING");	
	} else if(mode == GENERATOR_SIN) {
		strcat(modeStr, "GENERATOR SIN");	
	}
	
	Curs(0,0);
	PrintString(modeStr);
	
	char uartStr[20] = "UART: ";
	strcat(uartStr, usartReady ? "CONNECTED" : "DISCONECTED");
	
	Curs(1,0);
	PrintString(uartStr);
	
	if(mode == RECORDER){
		printRecordingStatus();
	} else if(mode == GENERATOR_SIN) {
		//printGeneratorStatus();
	}
	
}

void loop(){
	unsigned long int i = 0;
	
	
	while(1)
	{
		if(mode == GENERATOR_SIN){
			adc_value_updated = true;
			adc_value = 511 + 512 * sin(i * 1.744e-3);
		} 
		
		//Передаем при включении
		if(i % step == 0){
			values[i / step] = adc_value;
		}
		
		if(i == UPDATE_DELAY){
			printStatus();
			
			i = -1;
		}
		
		if(usartReady && adc_value_updated){
			adc_value_updated = false;
			TransmitInt((unsigned int) adc_value);
		}
		
		i++;
	}
}

int main()
{
	DDRD  &= ~(1<<2);
	PORTD |= (1<<2); 
	GICR  |= (1<<INT0);
	//настраиваем условие прерывания
	MCUCR |= (1<<ISC01)|(0<<ISC00);
	
	sei();
	
	wire_set(8000000, 100000); // тактовая частота контроллера, частота шины I2C
	
	USART_Init(8); //12 - 9600 8 - 115200
	
	LCD_Init();
	ADC_Init(); //Инициализируем АЦП
	
	Clear(); // очистка экрана
	led(1);  // включение и отключение подсветки экрана
	loop();
}

