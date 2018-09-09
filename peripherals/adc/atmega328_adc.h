#ifndef ATMEGA328_ADC_H_
#define ATMEGA328_ADC_H_
#include <stdint.h>

#define ADC_FREERUN	1
#define ADC_AUTOTRG 0

#ifdef __cplusplus
extern "C"{
#endif

#if ADC_FREERUN == 1
/** @brief initializes adc in freerun mode. */
void Adc_InitFreerun(uint8_t, volatile uint8_t *);
void Adc_StartFreerun(void);
void Adc_StopFreerun(void);
uint8_t Adc_ReadChannel(void);
uint8_t Adc_ReadValue(void);
#endif

#if ADC_AUTOTRG== 1
/** @brief initializes adc in auto-trigger mode using timer0. */
void Adc_InitAutotrigger(uint8_t, uint8_t *Adc_Value);
void Adc_StartAutotrigger();
void Adc_StartAutotrigger();
#endif

#ifdef __cplusplus
}
#endif

#endif /* ATMEGA328_ADC_H_ */
