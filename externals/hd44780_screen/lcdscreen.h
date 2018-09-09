#ifndef LCDSCREEN_H_
#define LCDSCREEN_H_

// Commands
#define ENTRY_MODE_SET	(1<<2)
#define INCREMENT_ADDRESS	(ENTRY_MODE_SET|(1<<1))
#define DECREMENT_ADDRESS	(ENTRY_MODE_SET)
#define DISPLAY_FROM_RIGHT	(ENTRY_MODE_SET|(1<<0))
#define CLR_DISPLAY		(1<<0)
#define RETURN_HOME		(1<<1)
#define DISP_CONTROL	(1<<3)
#define DISPLAY_OFF		(1<<3)
#define DISPLAY_ON		DISP_CONTROL|(1<<2)
#define CURSOR_ON		DISP_CONTROL|(1<<1)
#define BLINK_ON		DISP_CONTROL|(1<<0)
#define FUNCTION_SET	(1<<5)
#define DATA_LENGTH_EIGHT	FUNCTION_SET|(1<<4)
#define SET_TWO_LINES	FUNCTION_SET|(1<<3)
#define BIG_FONT		FUNCTION_SET|(1<<2)
#define SET_DDRAM_ADDR	(1<<7)

#define DELAYMS			5

// LCD Data
#define NEWLINE_ADDRESS  0x40
#define MAX_CHAR_IN_ROW	 16
// You have to change masks for your connections with uC
#define RS0	~(1<<0)
#define RS1 (1<<0)
#define RW0	~(1<<1)
#define RW1	(1<<1)
#define E	(1<<2)
#define COMMANDDDR		DDRC
#define DATADDR			DDRD
#define CONTROL_PORT	PORTC
#define DATA_PORT		PORTD
#define DATA_PIN		PIND
#define FOUR_BIT_MODE	(1<<1)
#define EIGHT_BIT_PINS	(_BV(PD0) | _BV(PD1) | _BV(PD2) | _BV(PD3) | _BV(PD4) | _BV(PD5) | _BV(PD6) | _BV(PD7))
#define FOUR_BIT_PINS	(_BV(PD4) | _BV(PD5) | _BV(PD6) | _BV(PD7))
#define BUSY_FLAG_PORT	(1<<7)
#define D7	(1<<7)
#define D6	(1<<6)
#define D5	(1<<5)
#define D4	(1<<4)

enum eMode	{ EIGHTPIN, FOURPIN };
enum eDatatype { COMMAND, DATA };

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

void LCDCommand(unsigned int);
void SetAddress(unsigned int, unsigned int);
void DisplayString(unsigned char *);
void DisplayInt(unsigned int);
void LCDInitDefaultSettings(enum eMode Mode);
void MovingString(unsigned char *);
void LCDWrite(enum eDatatype, unsigned char);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* LCDSCREEN_H_ */
