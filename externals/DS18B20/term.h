/*
 * term.h
 *
 *  Created on: 21 mar 2018
 *      Author: Fifi
 */

#ifndef TERM_H_
#define TERM_H_

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <util/atomic.h>

// Macros
#define ONE_WIRE_BUS	2
#define DS18PIN			PINB
#define set1wire		DDRB|=_BV(ONE_WIRE_BUS)		    //  set
#define clear1wire		DDRB &=~_BV(ONE_WIRE_BUS)		//	release

#define SKIP_ROM		0xCC
#define READ_SCRATCHPAD	0xBE
#define CONVERT			0x44

enum ErrorFlag { ERROR, OK };

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

// Functions
void InitDS18B20(enum ErrorFlag *);
void Reset_pulse();
int	 Reset_presence();
void send_one(int);
unsigned char read_one();
void send_byte(unsigned char);
unsigned char read_byte();
uint16_t Read_Temperature_One_Device();
float convert_temp(uint8_t, uint8_t);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* TERM_H_ */
