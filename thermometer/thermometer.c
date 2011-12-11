//------------------------------------------------------------------------------
//   AVR-Temp Sensor      
//   bitman@bitmania.de
//   www.bitmania.de
//------------------------------------------------------------------------------
// Based on the publication:   
//		Using DS18B20 digital temperature sensor on AVR microcontrollers
//		Description and application, Version 1.0 (Preliminary)
//		by Gerard Marull Paretas, September 2007
//------------------------------------------------------------------------------
// Fuses 
//  set 16MHz:		avrdude -c avrispv2 -P usb -p m88 -U lfuse:w:0xFF:m -U hfuse:w:0xDF:m
//  read current:	avrdude -v -c avrispv2 -P usb -p m88
//  ATmega88
//------------------------------------------------------------------------------

#define F_CPU 8000000UL

#include <avr/io.h> 
#include <avr/interrupt.h> 
#include <util/delay.h> 

#include <stdlib.h>
#include <avr/pgmspace.h>

#include "max7219.h"
#include "ds18b20.h"


// 7 segment digits
char digit_map[10] = {0x7E, 0x30, 0x6D, 0x79, 0x33, 0x5B, 0x5F, 0x70, 0x7F, 0x7B};

int main(void)
{
	// init max7219 spi communication
	matrix_init();
	
	

	// define variable that will hold temperature digit and decimal part
	int8_t digit = 0;
	uint16_t decimal = 0;
		
	while(1) {
		therm_read_temperature(&digit, &decimal);		
		//matrix_transmit(1, DIGIT_0);  // 0
		//matrix_transmit(2, DIGIT_8);  // 0
		_delay_ms(1000);
		
		char d3 = digit/100;               //Integer and Modulus Operations
		char kalan = digit%100;
		char d2 = kalan/10;              //             
		char d1 = kalan%10;	
		
		// print values to 7 segment display
		matrix_transmit(1, digit_map[d2]);  // 0
		matrix_transmit(2, digit_map[d1]);  // 0
	}
	
	return 0;
}   


