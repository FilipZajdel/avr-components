set(CMAKE_C_COMPILER avr-gcc)
set(MESSAGE_DECODER_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}" PARENT_SCOPE)

add_library(message_decoderlib message_decoder.c)
