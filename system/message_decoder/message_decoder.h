#ifndef message_decoder_h
#define message_decoder_h

#define DELIMITER ' '
#define END_CHAR 0

#include <stdint.h>
/** @brief Possible devices or program parts where data has to be delivered*/
typedef enum {TIMER, UART, RTC, GPIO, OTHER}eDeviceType;
/** @brief Type of message that is included in MessageStruct*/
typedef enum {STRING, NUMBER, M_COMMAND, VOID_POINTER, FUNC_POINTER}eTypeOfMessage;
/** @brief Possible keywords. Will be extended. */
typedef enum {RESET, TX, RX, WT, SET, OFF, ON, SAVE, ADC_VAL, TEMP}eKeywords;
/** @brief Struct to decode keywords. It contains keyword and corresponding string. */
typedef struct{
    eKeywords Keyword;
    char*     KeywordString;
}sKeywordCode;

/** @brief Union which contains value for MessageStruct. Type of message depends on MessageType. */
typedef union{
    unsigned char *StringMessage;
    uint8_t Number;
    eKeywords Keyword;
    void    *VoidPointer;
    void    (*Func)(void);
}uContent;
/** @brief Structure which can be used to deliver messages between modules or between peripherals.*/
typedef struct{
    eDeviceType    DestDevice;
    eTypeOfMessage MessageType;
    uContent       Content;
}MessageStruct;

#ifdef __cplusplus
extern "C"{
#endif  /* __cplusplus /*

/** @brief Function which takes string as an argument and splits it into array of MessageStruct
*   @param string to split
*   @param Pointer to memory where MessageStruct will be written.
*   @return Number of messages*/
uint8_t SplitStringToMessages(unsigned char *, MessageStruct *);
/** @brief Decodes string to keyword by given struct of keywords and corresponding strings
*   @param string to decode
*   @param reference where Keyword enum will be written
*   @return Returns 0 if string is on the keyword list, -1 instead */
int     DecodeKeyword(char *, eKeywords *);
/** @return Returns number of messages
*   @brief Saves messages in given array
*   @param string including full unsplitted message
*   @param reference, where array of MessageStruct will be written*/
uint8_t DecodeMessages(unsigned char *, MessageStruct *);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // message_decoder_h
