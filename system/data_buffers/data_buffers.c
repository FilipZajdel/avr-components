#include "data_buffers.h"

void Eight_WriteToBuffer(Eight_Buffer *DestBuffer, unsigned char Character){
	if( (EIGHT_BIT_DATA_LENGTH-1) > Eight_ReadBufferUsage(DestBuffer)){
		if( FREE == Eight_ReadStatus(DestBuffer) || BUSY == Eight_ReadStatus(DestBuffer)){
			DestBuffer->Data[DestBuffer->counter] = Character;
			DestBuffer->counter++;
			if( TERMINATOR == Character){
				DestBuffer->Data[DestBuffer->counter] = DATA_END_CHAR;
				DestBuffer->BufferState = READY;
			}
			else{
				DestBuffer->BufferState = BUSY;
			}
		}
	}
	else{
		DestBuffer->Data[DestBuffer->counter] = DATA_END_CHAR;
		DestBuffer->BufferState = OVERFLOW;
	}
}

unsigned char Eight_CopyDataFromBuffer(Eight_Buffer *SourceBuffer, uint8_t flagBufferErase){
	static uint8_t curr_char_ctr = 0;
	unsigned char current_char;
	current_char = SourceBuffer->Data[curr_char_ctr];

	if((curr_char_ctr+1) < Eight_ReadBufferUsage(SourceBuffer)){
		SourceBuffer->BufferState = BUSY;
		curr_char_ctr++;
		return current_char;
	}
	else if((curr_char_ctr+1) == Eight_ReadBufferUsage(SourceBuffer) && LAST_CHAR != Eight_ReadStatus(SourceBuffer)){
		SourceBuffer->BufferState = LAST_CHAR;
		return DATA_END_CHAR;
		//return '\n';
	}
	else if((curr_char_ctr+1) == Eight_ReadBufferUsage(SourceBuffer) && LAST_CHAR == Eight_ReadStatus(SourceBuffer)){
		curr_char_ctr = 0;
		if( 0 != flagBufferErase){
			Eight_EraseBuffer(SourceBuffer);
			return TERMINATOR;
		}
		else{
			SourceBuffer->BufferState = READY;
			return TERMINATOR;
		}
	}
	return DATA_END_CHAR;
}

uint8_t Eight_ReadBufferUsage(Eight_Buffer *Buffer){
	return Buffer->counter;
}

eBufferPossibleStates Eight_ReadStatus(Eight_Buffer *Buffer){
	return Buffer->BufferState;
}

void Eight_EraseBuffer(Eight_Buffer *BufferToErase){
	for( uint8_t DataCounter = 0; DataCounter<EIGHT_BIT_DATA_LENGTH; DataCounter++){
		BufferToErase->Data[DataCounter] = 0;	// changes from null
	}
	BufferToErase->counter = 0;
	BufferToErase->BufferState= FREE;
}

int Eight_WriteStringToBuffer(Eight_Buffer *BufferToWrite, char *SourceString){
	Eight_EraseBuffer(BufferToWrite);

	for(uint8_t CharCtr=0; CharCtr<EIGHT_BIT_DATA_LENGTH; CharCtr++){
		switch(Eight_ReadStatus(BufferToWrite)){
		case READY: return 0;
		case OVERFLOW: return -1;
		default:
			Eight_WriteToBuffer(BufferToWrite, SourceString[CharCtr]);
		}
	}
	return -1;
}
