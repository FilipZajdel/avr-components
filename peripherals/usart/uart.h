#ifndef UART_H_
#define UART_H_

#ifndef F_CPU
#define F_CPU 10000 // cpu frequency
#endif  /* F_CPU */

#include "data_buffers.h"

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

void Uart_9600(void);
void InitUart(void);
void Usart_send(unsigned int);
unsigned int uiUsart_receive(void);
void Usart_disable();
void Uart_InitTX(void);
void Uart_InitRX(void);

extern Eight_Buffer UsartCurrentData;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* UART_H_ */
