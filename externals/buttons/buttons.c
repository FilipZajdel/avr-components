#include "buttons.h"
#include "timers.h"
#include <avr/io.h>

#define SCALE_BUTTON	2
#define BUTTON_PCINT	PCINT2
#define BUTTON_PORT		PORTB
#define BUTTON_PIN		PINB
#define BUTTON_DDR		DDRB

volatile enum eTermometerScale *eCurrentScale;

void InitButtonScale(enum eTermometerScale *peTScale){
	BUTTON_DDR &= ~_BV(SCALE_BUTTON);
	BUTTON_PORT&= ~_BV(SCALE_BUTTON);
	PCICR  |= _BV(PCIE0);
	PCMSK0 |= _BV(BUTTON_PCINT);
	eCurrentScale = peTScale;
}

ISR(PCINT0_vect)
{
	msWaitTimer1(40);

	switch (*eCurrentScale){
		case KELVIN:
			if( 0x00 != (BUTTON_PIN & _BV(SCALE_BUTTON)))	*eCurrentScale = CELSIUS;
			else											*eCurrentScale = KELVIN;

			break;
		case CELSIUS:
			if( 0x00 != (BUTTON_PIN & _BV(SCALE_BUTTON)))	*eCurrentScale = KELVIN;
			else											*eCurrentScale = CELSIUS;

			break;
	}
	PCIFR |= _BV(PCIF0);
}
