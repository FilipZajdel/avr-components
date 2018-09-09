#include "lcdscreen.h"
#include "timers.h"
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>

#define NULL '\0'
enum eREGISTER_SELECT { IR=RS0, DR=RS1 };
enum eREAD_WRITE_REGISTER { READ = RW1, WRITE = RW0 };
enum eBUSY_FLAG { BUSY, NOT_BUSY };

volatile enum eMode eCurrentMode;

void SetRegisters(enum eREGISTER_SELECT RS, enum eREAD_WRITE_REGISTER RW);
void Select();
void WriteTetrade(unsigned int uiTetrade);
unsigned int ReadTetrade();
enum eBUSY_FLAG eReadBusyFlag()	;
unsigned int uiReadAddress();
unsigned int uiReadByte();

void LCDWrite(enum eDatatype eType, unsigned char ucData)
{
	if( COMMAND == eType )	SetRegisters( IR, WRITE );
	else if(DATA == eType)	SetRegisters( DR, WRITE );

	switch (eCurrentMode)
	{
	case FOURPIN:
		WriteTetrade(ucData >> 4);
		Select();
		usWaitTimer2(250);
		WriteTetrade(ucData & 0x0F);
		usWaitTimer2(250);
		Select();
		break;

	case EIGHTPIN:
		DATADDR |= EIGHT_BIT_PINS;
		DATA_PORT = ucData;
		usWaitTimer2(50);
		Select();
		break;
	}
}

void LCDCommand(unsigned int uiCommand)
{
	if ( EIGHTPIN == eCurrentMode )
	{
			SetRegisters( IR, WRITE );
			usWaitTimer2(50);
			DATADDR |= EIGHT_BIT_PINS;
			DATA_PORT = uiCommand;
			usWaitTimer2(50);
			Select();
	}
	else if( FOURPIN == eCurrentMode)
	{
			SetRegisters( IR, WRITE );
			WriteTetrade(uiCommand>>4);
			usWaitTimer2(150);
			Select();
			CONTROL_PORT |=E;
			WriteTetrade(uiCommand & 0x0Fu);
			usWaitTimer2(150);
			Select();
	}
}

void SetAddress(unsigned int uiColumn, unsigned int uiRow)
{
	volatile unsigned int auiRowsOffset [4] = { 0x00, 0x40, 0x80, 0x120 };
	LCDWrite(COMMAND,(SET_DDRAM_ADDR | uiColumn | auiRowsOffset[uiRow]));
}

void DisplayString(unsigned char *pcString)
{
	for( int i=0; pcString[i] != NULL; i++)
	{
		if( 0x10 == uiReadAddress())	SetAddress(0,1);

		LCDWrite(DATA, pcString[i]);
	}
}

void LCDInitDefaultSettings(enum eMode Mode)
{
	msWaitTimer1(50);
	COMMANDDDR |= (E|RW1|RS1);
	eCurrentMode = Mode;

	if( EIGHTPIN == eCurrentMode)
	{
		LCDWrite(COMMAND,CLR_DISPLAY);
		usWaitTimer2(50);
		LCDWrite(COMMAND,RETURN_HOME);
		usWaitTimer2(50);
		LCDWrite(COMMAND,INCREMENT_ADDRESS);
		usWaitTimer2(50);
		LCDWrite(COMMAND,DISPLAY_ON);
		usWaitTimer2(50);
		LCDWrite(COMMAND,(DATA_LENGTH_EIGHT | BIG_FONT | SET_TWO_LINES));
		usWaitTimer2(50);
	}
	else if(FOURPIN == eCurrentMode)
	{
		SetRegisters(IR, WRITE);

		WriteTetrade(0x03u);
		Select();
		msWaitTimer1(DELAYMS);

		WriteTetrade(0x03u);
		Select();
		msWaitTimer1(DELAYMS);

		WriteTetrade(0x03u);
		Select();
		msWaitTimer1(DELAYMS);

		WriteTetrade(0x02u);
		Select();
		msWaitTimer1(DELAYMS);

		LCDWrite(COMMAND,(SET_TWO_LINES | BIG_FONT));
		LCDWrite(COMMAND,DISPLAY_OFF);
		LCDWrite(COMMAND,CLR_DISPLAY);
		LCDWrite(COMMAND,INCREMENT_ADDRESS);
		LCDWrite(COMMAND,(DISPLAY_ON | BLINK_ON | CURSOR_ON));
	}
}

void DisplayInt(unsigned int uiNumber)
{
	unsigned char ucCurrentNumber[]="     ";
	itoa(uiNumber, ucCurrentNumber, 10);
	DisplayString(ucCurrentNumber);
}

void MovingString(unsigned char *pStr)
{
	uint8_t uiStringLength;
	static int iBeginningAddress;
	enum eMoveSide { LEFT, RIGHT };
	static enum eMoveSide eSide = RIGHT;

	for( uiStringLength=0; pStr[uiStringLength] != NULL; uiStringLength++){};

	switch (eSide)
	{
		case RIGHT:
			if( (MAX_CHAR_IN_ROW - uiStringLength) == iBeginningAddress)
			{
				iBeginningAddress--;
				eSide = LEFT;
			}
			else
			{
				iBeginningAddress++;
				eSide = RIGHT;
			}
			break;

		case LEFT:
			if( 0 == iBeginningAddress)
			{
				iBeginningAddress++;
				eSide = RIGHT;
			}
			else
			{
				iBeginningAddress--;
				eSide = LEFT;
			}
			break;
	}
	LCDWrite(COMMAND,CLR_DISPLAY);
	SetAddress(iBeginningAddress, 0);
	DisplayString(pStr);
}

//////////////// Internal interface function /////////////////////////
void SetRegisters(enum eREGISTER_SELECT RS, enum eREAD_WRITE_REGISTER RW)
{
	switch (RS)
	{
		case IR:
			CONTROL_PORT &=IR;
			break;
		case DR:
			CONTROL_PORT |=DR;
			break;
	}
	switch (RW)
	{
		case WRITE:
			CONTROL_PORT &=WRITE;
			break;
		case READ:
			CONTROL_PORT |=READ;
			break;
	}
	msWaitTimer1(DELAYMS);
}
void Select()
{
	CONTROL_PORT |=E;
	CONTROL_PORT&= ~E;
	msWaitTimer1(DELAYMS);
}
void WriteTetrade(unsigned int uiTetrade)
{
	DATADDR |= (D4|D5|D6|D7);
	DATA_PORT &=~(D4|D5|D6|D7);
	DATA_PORT |= ((uiTetrade&0x0Fu)<<4);
}
void _WriteTetrade(unsigned int uiTetrade)
{
	DATADDR |= (D4|D5|D6|D7);
	DATA_PORT &=~(D4|D5|D6|D7);

	if( uiTetrade & 0x1)		DATA_PORT |= D4;
	if (uiTetrade & 0x2)	 	DATA_PORT |= D5;
	if (uiTetrade & 0x4)		DATA_PORT |= D6;
	if (uiTetrade & 0x8)		DATA_PORT |= D7;
}

unsigned int ReadTetrade()
{
	volatile unsigned int uiCurrentTetradeValue=0;

	SetRegisters( IR, READ );
	DATADDR &= ~(D7|D6|D5|D4);
	uiCurrentTetradeValue = (DATA_PIN & (D7|D6|D5|D4));
	DATADDR |= (D7|D6|D5|D4);
	return uiCurrentTetradeValue;
}
enum eBUSY_FLAG eReadBusyFlag()		// poprawic na 4 pin // done
{
	SetRegisters( IR, READ );
	DATADDR &= ~BUSY_FLAG_PORT;
	CONTROL_PORT |=E;

	if( (DATA_PIN & BUSY_FLAG_PORT) == 1 )
	{
		DATADDR |= BUSY_FLAG_PORT;
		return BUSY;
	}
	else
	{
		DATADDR |= BUSY_FLAG_PORT;
		return NOT_BUSY;
	}
}
unsigned int uiReadAddress()
{
	volatile unsigned int uiAddress=0;
	SetRegisters(IR, READ);

	switch( eCurrentMode )
	{
	case EIGHTPIN:
		//DATADDR &= ~EIGHT_BIT_PINS;
		CONTROL_PORT |=E;
		/*usWaitTimer2(240);
		usWaitTimer2(160);*/
		uiAddress = uiReadByte();
		Select();
		break;
	case FOURPIN:
		DATADDR &= ~(D7|D6|D5|D4);
		CONTROL_PORT |=E;
		//usWaitTimer2(240);
		//usWaitTimer2(160);
		uiAddress = ReadTetrade();
		Select();
		CONTROL_PORT |=E;
		//usWaitTimer2(240);
		//usWaitTimer2(160);
		uiAddress |= ((ReadTetrade()>>4)&0x0F);
		Select();
		uiAddress &= ~(0x80);
		DATADDR |=(D7|D6|D5|D4);
		break;
	}
	return (uiAddress & ~0x80);
}

unsigned int uiReadByte(void)
{
	volatile unsigned int uiByte=0;

	SetRegisters( IR, READ );
	DATADDR &= ~(EIGHT_BIT_PINS);
	CONTROL_PORT |=E;
	uiByte = DATA_PIN;
	DATADDR |=EIGHT_BIT_PINS;
	return uiByte;
}
