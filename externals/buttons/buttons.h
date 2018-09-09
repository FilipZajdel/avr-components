#ifndef BUTTONS_H_
#define BUTTONS_H_
#include <avr/interrupt.h>

enum eTermometerScale { CELSIUS=0 , KELVIN=273 };

ISR(PCINT0_vect);
void InitButtonScale(enum eTermometerScale *);

#endif /* BUTTONS_H_ */
