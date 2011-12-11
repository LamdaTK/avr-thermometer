#ifndef F_CPU
#define F_CPU 1000000L
#endif

#include <avr/io.h>

#define OP_NOOP			0x00
#define OP_DIGIT0		0x01
#define OP_DIGIT1		0x02
#define OP_DIGIT2		0x03
#define OP_DIGIT3		0x04
#define OP_DIGIT4		0x05
#define OP_DIGIT5		0x06
#define OP_DIGIT6		0x07
#define OP_DIGIT7		0x08

#define OP_DECODEMODE	0x09
#define OP_INTENSITY	0x0A
#define OP_SCANLIMIT	0x0B
#define OP_SHUTDOWN		0x0C
#define OP_DISPLAYTEST	0x0F


#define SPI_DDR		DDRB
#define SPI_PORT	PORTB
#define SPI_SS		2 // Atmega8 PB2 ---> Max7219 pin 12 (LOAD)
#define SPI_MOSI	3 // Atmega8 PB3 ---> Max7219 pin 1  (DIN)
#define SPI_MISO	4 // No connection
#define SPI_SCK		5 // Atmega8 PB5 ---> Max7219 pin 13 (LOAD)

// Dedine digits
#define DIGIT_0  0x7E  // 0
#define DIGIT_1  0x30  // 1
#define DIGIT_2  0x6D  // 2
#define DIGIT_3  0x79  // 3
#define DIGIT_4  0x33  // 4
#define DIGIT_5  0x5B  // 5
#define DIGIT_6  0x5F  // 6
#define DIGIT_7  0x70  // 7
#define DIGIT_8  0x7F  // 8
#define DIGIT_9  0x7B  // 9
