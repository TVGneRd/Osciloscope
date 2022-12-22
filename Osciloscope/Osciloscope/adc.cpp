#include "adc.h"
//----------------------------------------
void ADC_Init(void)
{
	ADMUX = 0x00;
	ADCSRA = 0x8D;
}
//----------------------------------------
unsigned int ADC_convert ()
{
	ADMUX= 0 | (ADMUX & 0xF0);
	//задержка для стабилизации входного напряжения
	_delay_us(10);
	//начинаем преобразование (ADSC = 1)
	ADCSRA |= 0x40;
	while((ADCSRA & 0x10)==0); //ждем, пока АЦП закончит преобразование (ADIF = 0)
	ADCSRA|=0x10;//устанавливаем ADIF
	return ADCW;//ADCW - содержит ADCH и ADCL как нам нужно
}