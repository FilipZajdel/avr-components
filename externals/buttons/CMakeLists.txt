set(CMAKE_C_COMPILER avr-gcc)
set(TIMERS_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}" PARENT_SCOPE)

add_library(timerslib timers.c)
