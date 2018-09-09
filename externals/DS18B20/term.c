#include "term.h"
#include "timers.h"

enum ErrorFlag *pCurrentFlag;

// Functions

void InitDS18B20(enum ErrorFlag *pFlag){
	pCurrentFlag = pFlag;
	set1wire;
}

void Reset_pulse(){
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
			set1wire;
			usWaitTimer2(240);
			usWaitTimer2(240);
			clear1wire;
		}
}

int  Reset_presence()
{
	volatile unsigned char ucPresenceFlag=0;
	volatile unsigned char counter=0;

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			Reset_pulse();
			usWaitTimer2(25);

			while((counter < 0xFF) && (0 != (DS18PIN & _BV(ONE_WIRE_BUS))))
				{
					usWaitTimer2(1);
					counter++;
				}
			if(0xFF <= counter)	ucPresenceFlag=0;
			else				ucPresenceFlag=1;

			counter=0;

			if( 0x01 == ucPresenceFlag )
			{
				while((counter < 0xFF) && (0 == (DS18PIN & _BV(ONE_WIRE_BUS))))
					{
						usWaitTimer2(2);
						counter++;
					}
				if(0xFF <= counter)	ucPresenceFlag=0;
				else				ucPresenceFlag=1;
			}
		}
	if( 0x01 == ucPresenceFlag)		return 1;
	else							return 0;
}

void send_one(int bit)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			set1wire;
			usWaitTimer2(2);
			if(0 == bit){
			usWaitTimer2(100);}
			clear1wire;
			if(1 == bit)usWaitTimer2(100);
		}
}
unsigned char read_one()
{
	volatile unsigned char ucCounter=0;
	volatile unsigned char bit=0;

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		set1wire;
		usWaitTimer2(2);
		clear1wire;
		usWaitTimer2(15);

		if( DS18PIN & (1<<ONE_WIRE_BUS))	bit = 1;
		else								bit = 0;

		while( ucCounter < 0x40 && (0x00 == (DS18PIN & (1<<ONE_WIRE_BUS))))
		{
			usWaitTimer2(1);
			ucCounter++;
		}
	}
	if( 0x40 == ucCounter)		*pCurrentFlag = ERROR;
	else						*pCurrentFlag = OK;

	return bit;
}

void send_byte(unsigned char message)
{
	for(int i=0; i<8; i++)
	{
		send_one( (message>>i) & 0x01 );
		usWaitTimer2(18);
	}
}

unsigned char read_byte()
{
	unsigned char byte=0;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		unsigned char temp;

		for(int i=0; i<8; i++)
		{
			if( 1 == read_one())
			{
				temp = 0x01<<i;
				byte +=temp;
			}
			if( ERROR == *pCurrentFlag)
			{
				break;
			}
			usWaitTimer2(1);
		}
	}
	return byte;
}

uint16_t Read_Temperature_One_Device()
{
	volatile unsigned int uiConvertTimeCounter=0;
	volatile uint16_t uiCurrentTemperature;

	send_byte(SKIP_ROM);
	send_byte(CONVERT);
	while( (uiConvertTimeCounter < 0xFF ) && (0x00 == (DS18PIN & _BV(ONE_WIRE_BUS))))
	{
		usWaitTimer2(5);
		uiConvertTimeCounter++;
	}

	if( 0xFF == uiConvertTimeCounter ){
		*pCurrentFlag = ERROR;
		return 0;
	}
	else{
		*pCurrentFlag = OK;
	}

	if( 0x01 ==	Reset_presence()){
		*pCurrentFlag = OK;
	}
	else{
		*pCurrentFlag = ERROR;
		return 0;
	}

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
		send_byte(SKIP_ROM);
		send_byte(READ_SCRATCHPAD);

		uiCurrentTemperature = (read_byte() & 0x00FF);
		if( ERROR == *pCurrentFlag)		return 0;

		uiCurrentTemperature |= ((read_byte()<<8) & 0xFF00);
		if( ERROR == *pCurrentFlag)		return 0;
	}
	Reset_pulse();
	return uiCurrentTemperature;
}
float convert_temp(uint8_t MSB, uint8_t LSB){
	if(0x01 == (MSB>>7)){
		MSB = ~(MSB);
		LSB = ~(LSB)+1;
		return (float)(MSB+0.0625*LSB)*(-1.0);
	}
	return (float)(MSB+0.0625*LSB);
}
