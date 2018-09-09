#include "atmega328_adc.h"
#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t *Adc_CurrentValue = 0;

void Adc_InitFreerun(uint8_t Adc_Channel, volatile uint8_t *pAdc_Value){
	PRR &= ~(1<<PRADC);
	ADCSRA |= (1<<ADEN) | (1<<ADIE) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0) | (1<<ADATE);
	ADCSRB &= ~((1<<ADTS0)|(1<<ADTS1)|(1<<ADTS2));
	ADMUX |= (Adc_Channel & 0b1111) | (1<<REFS0) | (1<<ADLAR);
	DIDR0 |= (1<<Adc_Channel);
	Adc_CurrentValue = pAdc_Value;
}

void Adc_StartFreerun(){
	ADCSRA |= (1<<ADSC);
}

void Adc_StopFreerun(){
	ADCSRA &= ~(ADEN);
}

uint8_t Adc_ReadChannel(){
	return (ADMUX & 0b1111);
}

uint8_t Adc_ReadValue(void){
	//return ADCH;
	return *Adc_CurrentValue;
}

ISR(ADC_vect){
	*Adc_CurrentValue = ADCH;
}
