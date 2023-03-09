#include "adc.h"
//----------------------------------------
void ADC_Init(void)
{

	ADCSRA |= (1<<ADEN);//Вкл АЦП
	
	ADCSRA |= 1<<ADPS2 | 1<<ADPS1 | 1<<ADPS0; // предделитель на 128
	
	ADCSRA |= (1<<ADIE);// разрешаем прерывание
	
	ADCSRA |= 1<<ADFR;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         
	
	ADMUX |= (1<<REFS1) | (1<<REFS0); //AVCC with external capacitor at AREF pin
	
	ADMUX |= (0<<ADLAR); //лев выравнивание
	
	ADMUX |= (0<<MUX1) | (1<<MUX0); // PC3 запуск по 1 каналу
	
	//ADMUX = (ADMUX & 0x00);
	
	ADCSRA |= 1<<ADSC;
	
}


//----------------------------------------
unsigned int ADC_convert ()
{
	ADMUX = (ADMUX & 0xF0);
	//задержка для стабилизации входного напряжения
	_delay_us(10);
	//начинаем преобразование (ADSC = 1)
	ADCSRA |= 0x40;
	while((ADCSRA & 0x10)==0); //ждем, пока АЦП закончит преобразование (ADIF = 0)
	ADCSRA|=0x10;//устанавливаем ADIF
	return ADCW;//ADCW - содержит ADCH и ADCL как нам нужно
}