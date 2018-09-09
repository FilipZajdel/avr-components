/**@file data_buffers.h
 * @author Filip Zajdel
 * @date 23/07/2018
 * @brief File includes some kind of buffers which can be used in other projects
 */
#ifndef data_buffers_h
#define data_buffers_h

#include <stdint.h>
//#define NULL '\0'
#define EIGHT_BIT_DATA_LENGTH 32
#define DATA_END_CHAR 0
#define NEW_LINE	'\n'
#define TERMINATOR '\r'

/** @brief Enumerator which includes states corresponding with possible buffer states */
typedef enum{
	FREE = 0,
	READY = 1,
	BUSY =  2,
	OVERFLOW = 3,
	LAST_CHAR = 4
}eBufferPossibleStates;

/** @brief Main struct to store data from different sources (eg. uart, hid device)
 * 		   By default the end of data is NULL
 * 		   counter: 	counts data in buffer
 * 		   BufferState: corresponds with current state of buffer
 * 		   flag:		for any idea of its use
 */
typedef struct{
	unsigned char Data[EIGHT_BIT_DATA_LENGTH];
	union{
		struct{
			uint8_t counter: 5;
			eBufferPossibleStates BufferState: 3;
		};
		uint8_t content;
	};
}Eight_Buffer;

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

/**@brief Writes one character to buffer
 * @param Destination Buffer
 * @param Character to write*/
void Eight_WriteToBuffer(Eight_Buffer *DestBuffer, unsigned char Character);
/** @brief Copies content of the buffer to destination given as a reference
 *  @param Source buffer
 *  @param if true - buffer is set to default after copying
 *  	   if false- function only copies the data and buffer stays unchanged
 *  @return last character taken from buffer*/
unsigned char Eight_CopyDataFromBuffer(Eight_Buffer *SourceBuffer, uint8_t flagBufferErase);
/** @brief Returns status of a buffer given as a reference
 * @param reference to buffer
 * @return State of buffer*/
eBufferPossibleStates Eight_ReadStatus(Eight_Buffer *Buffer);
/** @brief Returns the length of data stored in buffer
 * @param reference to buffer
 * @return number of characters in buffer*/
uint8_t Eight_ReadBufferUsage(Eight_Buffer *Buffer);
/** @brief Cleanes the buffer given as reference. Fills its Data[] with NULL, sets counter's value to 0, and
 *         Changes BufferState to FREE
 *  @param reference to buffer*/
void Eight_EraseBuffer(Eight_Buffer *);
/** @brief Writes given string to buffer. If string is too long, returns -1, and cuts string to suitable length.
 *  @return 0 if succeed
 *  @return -1 if buffer overflow*/
int Eight_WriteStringToBuffer(Eight_Buffer *, char *);

#ifdef __cplusplus
}
#endif /* __cpluspluc */

#endif /* DATA_BUFFERS_H */
