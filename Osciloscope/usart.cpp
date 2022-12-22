#include "usart.h"

void USART_Init( unsigned int speed)//������������� ������ USART
{
		UBRRH = (unsigned char)(speed>>8);
		UBRRL = (unsigned char)speed;
		
		UCSRB=(1<<RXEN)|(1<<TXEN); //�������� ����� � �������� �� USART
		UCSRB |= (1<<RXCIE); //��������� ���������� ��� ��������
		UCSRA |= (1<<U2X); // ��� 8 ���
		UCSRC = (1<<URSEL)|(1<<USBS)|(1<<UCSZ1)|(1<<UCSZ0);// ���������� ������ � �������� UCSRC (URSEL=1),
		//������������ ����� (UMSEL=0), ��� �������� �������� (UPM1=0 � UPM0=0),
		//1 ����-��� (USBS=0), 8-��� ������� (UCSZ1=1 � UCSZ0=1)
		//UCSRC |= (1<<UPM1);//��������
}

void USART_Transmit( unsigned char data ) //������� �������� ������
{
	while ( !(UCSRA & (1<<UDRE)) ); //�������� ����������� ������ ������
	UDR = data; //������ �������� ������
}

int USART_Read()
{
	return ( (UCSRA >> RXC) & 1 ) ? UDR : -1;  // ���������� �������� ������ �����
}