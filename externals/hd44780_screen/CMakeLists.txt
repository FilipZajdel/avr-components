set(CMAKE_C_COMPILER avr-gcc)

get_property(TIMERS_INC GLOBAL PROPERTY TIMERS_INCLUDE) 

include_directories(${TIMERS_INC})

add_library(hd44780lib lcdscreen.c)
