#include "message_decoder.h"
#include <string.h>
#include <stdlib.h>

#define KEYWORD_NUM  10

sKeywordCode AvailableKeywords[KEYWORD_NUM] ={
    {RESET , "reset"},
    {TX    , "transmit" },
    {RX    , "receive"},
    {WT    , "wait"},
    {SET   , "set"},
    {OFF   , "turn_off"},
    {ON    , "turn_on"},
    {SAVE  , "save"},
	{ADC_VAL   , "adc"},
	{TEMP  , "temperature"}
};

int CompareString(char *str1, char *str2){
	uint8_t CharCtr = 0;
	for(CharCtr = 0; str1[CharCtr] != END_CHAR; CharCtr++){
		if(str1[CharCtr] == str2[CharCtr]){
			continue;
		}
		else{
			return -1;
		}
	}
	if(str1[CharCtr] != str2[CharCtr]){
		return -1;
	}
	else{
		return 0;
	}
}

void ReplaceCharacter(char *StringToReplace, char OldChar, char NewChar){
    for(uint8_t char_counter=0; StringToReplace[char_counter]!=0; char_counter++){
        if( OldChar == StringToReplace[char_counter]){
            StringToReplace[char_counter] = NewChar;
        }
    }
}

uint8_t SplitStringToMessages(unsigned char *SourceString, MessageStruct *MessageArray){
    enum eAutomatState {DELIMITER_, CHARACTER} CurrentState = DELIMITER_;
    unsigned char *curr_char;
    uint8_t number_of_messages = 0;

    for(uint16_t char_counter=0; SourceString[char_counter] != END_CHAR; char_counter++){
        curr_char = &SourceString[char_counter];

        switch(CurrentState){
        case DELIMITER_:
            if( DELIMITER == *curr_char ){
                CurrentState = DELIMITER_;
            }
            else{
                MessageArray[number_of_messages].Content.StringMessage = curr_char;
                number_of_messages++;
                CurrentState = CHARACTER;
            }
            break;
        case CHARACTER:
            if( DELIMITER == *curr_char){
                CurrentState = DELIMITER_;
            }
            else{
                CurrentState = CHARACTER;
            }
            break;
        }
    }
    return number_of_messages;
}

int DecodeKeyword(char *SourceString, eKeywords *Keyword){

    for(uint8_t KeywordCounter=0; KeywordCounter < KEYWORD_NUM; KeywordCounter++)
    {
        if( 0 == CompareString(SourceString, AvailableKeywords[KeywordCounter].KeywordString)){
            *Keyword = AvailableKeywords[KeywordCounter].Keyword;
            return 0;
        }
    }
    return -1;
}
uint8_t DecodeMessages(unsigned char *UnsplittedMessage, MessageStruct *MessageArray){
    uint8_t num_of_messages = SplitStringToMessages(UnsplittedMessage, MessageArray);
    ReplaceCharacter(UnsplittedMessage, DELIMITER, END_CHAR);
    ReplaceCharacter(UnsplittedMessage, '\r', END_CHAR);
    eKeywords CurrentKeyword=OFF;
    char cTemp[3];

    for(uint8_t message_counter = 0; message_counter<num_of_messages; message_counter++){
        if( 0 == DecodeKeyword(MessageArray[message_counter].Content.StringMessage, &CurrentKeyword)){
            MessageArray[message_counter].MessageType = M_COMMAND;
            MessageArray[message_counter].Content.Keyword = CurrentKeyword;
        }
        else if( 0 != strtol(MessageArray[message_counter].Content.StringMessage, cTemp, 10)
                /*|| 0 != strtol(MessageArray[message_counter].Content.StringMessage, cTemp, 16)*/ // it doesn't detect proper
                || 0 != strtol(MessageArray[message_counter].Content.StringMessage, cTemp, 2)){
            MessageArray[message_counter].MessageType = NUMBER;
            MessageArray[message_counter].Content.Number = strtol(MessageArray[message_counter].Content.StringMessage, cTemp, 10);
        }
        else{
            MessageArray[message_counter].MessageType = STRING;
        }
    }
    return num_of_messages;
}


