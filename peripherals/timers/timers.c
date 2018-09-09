#include "timers.h"
#include <avr/io.h>
#include <avr/interrupt.h>

void (*pT1FunctionInInterrupt)(void);
void (*pT2FunctionInInterrupt)(void);

void usWaitTimer1(unsigned int uiTime)
{
	TCCR1B = (_BV(CS10) | _BV(WGM12));
	GTCCR = _BV(PSRSYNC);
	OCR1B = (8*uiTime)-1;
	TCNT1 = 0;
	while( 0x00 == (TIFR1 & _BV(OCF1B))){};
	TIFR1 |= _BV(OCF1B);
}

void usInterruptTimer1(unsigned int uiTime, void (*InterruptFunction)(void))
{
	pT1FunctionInInterrupt = InterruptFunction;
	TIMSK1 = _BV(OCIE1A);
	TCCR1B = _BV(CS10);
	GTCCR = _BV(PSRSYNC);
	OCR1A = (8*uiTime)-1;
	TCNT1 = 0;
}
void msWaitTimer1(unsigned int uiTime)
{
	TCCR1B = (_BV(CS12) | _BV(CS10) | _BV(WGM12));
	GTCCR = _BV(PSRSYNC);
	OCR1A = (uiTime*8)-1;
	TCNT1 = 0;
	while( 0x00 == (TIFR1 & _BV(OCF1A))){};
	TIFR1 |= _BV(OCF1A);
}

void msInterruptTimer1(unsigned int uiTime, void (*InterruptFunction)(void))
{
	pT1FunctionInInterrupt = InterruptFunction;
	TCCR1B = (_BV(CS12)|_BV(CS10));
	TIMSK1 =_BV(OCIE1A);
	GTCCR = _BV(PSRSYNC);
	OCR1A = (uiTime*8)-1;
	TCNT1 = 0;
}

ISR(TIMER1_COMPA_vect)
{
	TCNT1=0;
	pT1FunctionInInterrupt();
}

void usWaitTimer2(unsigned int uiMicroseconds)
{
	TCCR2B = (_BV(CS21) | _BV(WGM12));
	GTCCR = _BV(PSRSYNC);
	OCR2A = uiMicroseconds-1;
	TCNT2 = 0;
	while( 0x00 == (TIFR2 & _BV(OCF2A))){};
	TIFR2 = _BV(OCF2A);
}

void usInterruptTimer2(unsigned int uiTime, void (*FunctionInInterrupt)(void))
{
	pT2FunctionInInterrupt = FunctionInInterrupt;
	TCCR2B = (_BV(CS21) | _BV(WGM12));
	TIMSK2 |= _BV(OCIE2A);
	GTCCR  |= _BV(PSRSYNC);
	OCR2A = uiTime-1;
	TCNT2 = 0;
}

ISR(TIMER2_COMPA_vect)
{
	pT2FunctionInInterrupt();
	TCNT2 = 0;
}
