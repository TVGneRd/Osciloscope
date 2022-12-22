#ifndef USART_H_
#define USART_H_

#include "main.h"

void USART_Init( unsigned int speed);
void USART_Transmit( unsigned char data );
int USART_Read();

#endif /* USART_H_ */