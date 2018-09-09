cmake_minimum_required(VERSION 2.8)

set(CMAKE_C_COMPILER avr-gcc)

set(CMAKE_C_FLAGS "-std=c11 -g -Os -mmcu=atmega328p")

set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")

get_property(DATABUFFERS_PROPERTY GLOBAL PROPERTY DATA_BUFFERS_PROPERTY)

include_directories(${DATABUFFERS_PROPERTY})

set(USART_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}" PARENT_SCOPE)

add_library(usartlib uart.c)
