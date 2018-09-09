#include "uart.h"
#include <avr/io.h>
#include <util/atomic.h>
#include <avr/interrupt.h>

char ReceivedCharacter;
Eight_Buffer UsartCurrentData;

ISR(USART_RX_vect){
	ReceivedCharacter = UDR0;
	if( READY != Eight_ReadStatus(&UsartCurrentData) && OVERFLOW != Eight_ReadStatus(&UsartCurrentData)){
		Eight_WriteToBuffer(&UsartCurrentData, ReceivedCharacter);
	}
}
ISR(USART_TX_vect){
	if( FREE != Eight_ReadStatus(&UsartCurrentData)){
		Usart_send(Eight_CopyDataFromBuffer(&UsartCurrentData, 1));
	}
}

void Uart_9600(void)
{
	#define BAUD 9600
	#include <util/setbaud.h>
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;
	#if USE_2X
		UCSR0A |= (1<<U2X0);
	#else
		UCSR0A &= ~(1<<U2X0);
	#endif
}

void Usart_send(unsigned int uiData)
{
	while(!(UCSR0A & _BV(UDRE0))){};
	UDR0 = uiData;
}

unsigned int uiUsart_receive(void)
{
	while(!(UCSR0A & _BV(RXC0))){};
	return UDR0;
}

void InitUart(void)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		UCSR0C = (_BV(UCSZ00) | _BV(UCSZ01) | _BV(UPM01));
		UCSR0B = _BV(TXEN0);
	}
	Eight_EraseBuffer(&UsartCurrentData);
}

void Usart_disable()
{
	UCSR0B &= ~_BV(TXEN0);
}

void Uart_InitTX(void){
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
		UCSR0C = (_BV(UCSZ00) | _BV(UCSZ01) | _BV(UPM01));
		UCSR0B = (_BV(TXEN0) | _BV(TXCIE0));
	}
}
void Uart_InitRX(void){
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
		UCSR0C = (_BV(UCSZ00) | _BV(UCSZ01) | _BV(UPM01));
		UCSR0B = (_BV(RXEN0) | _BV(RXCIE0));
	}
}
