set(CMAKE_C_COMPILER avr-gcc)
set(ADC_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}" PARENT_SCOPE)

add_library(adclib atmega328_adc.c)
