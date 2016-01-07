/*
 * serial_stdio.h
 *
 * Created: 05-Aug-15 12:50:21 PM
 *  Author: emon1
 */ 


#ifndef SERIAL_STDIO_H_
#define SERIAL_STDIO_H_


#ifdef __cplusplus
extern "C" {
	#endif


#include <stdio.h>
#include "usart.h"


#define USE_STDIO_ON_USARTC0	1
#define USE_STDIO_ON_USARTC1	0
#define USE_STDIO_ON_USARTD0	0
#define USE_STDIO_ON_USARTD1	0
#define USE_STDIO_ON_USARTE0	1
#define USE_STDIO_ON_USARTE1	0
#define USE_STDIO_ON_USARTF0	0
#define USE_STDIO_ON_USARTF1	0





#define create_serial_stream(usart_unit) \
	int usart_unit ## _putchar(char ch, FILE *stream){\
		if(ch=='\n')\
		usart_unit ## _putchar('\r',stream);\
		usart_transmit_by_val(&usart_unit ,(char) ch);\
		return 0;\
	}\
	\
	int usart_unit ## _getchar(FILE *stream)\
	{\
		char ch;\
		usart_receive_by_ref( &usart_unit, (uint8_t *) &ch);\
		usart_unit ## _putchar(ch,stream);\
		return ch;\
	}
	
	
	
#if USE_STDIO_ON_USARTC0 == 1
	create_serial_stream(USARTC0);
#endif

#if USE_STDIO_ON_USARTC1 == 1
create_serial_stream(USARTC1);
#endif

#if USE_STDIO_ON_USARTD0 == 1
create_serial_stream(USARTD0);
#endif

#if USE_STDIO_ON_USARTD1 == 1
create_serial_stream(USARTD1);
#endif

#if USE_STDIO_ON_USARTE0 == 1
create_serial_stream(USARTE0);
#endif

#if USE_STDIO_ON_USARTE1 == 1
create_serial_stream(USARTE1);
#endif

#if USE_STDIO_ON_USARTF0 == 1
create_serial_stream(USARTF0);
#endif

#if USE_STDIO_ON_USARTF1 == 1
create_serial_stream(USARTF1);
#endif


/**
 * initializing the stdio file stream over uart
 */
void set_stdio_stream(FILE* stdio_file)
{
	stdout = stdin = stdio_file;
}

void create_file_stream(FILE* stdio_file, int (*putc_func)(char ch, FILE *stream) , int (*getc_func) (FILE *stream)){
	FILE temp_file = FDEV_SETUP_STREAM(putc_func, getc_func, _FDEV_SETUP_RW);
	*stdio_file = temp_file;
}


#ifdef __cplusplus
}
#endif


#endif /* SERIAL_STDIO_H_ */