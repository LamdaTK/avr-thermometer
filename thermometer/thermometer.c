//------------------------------------------------------------------------------
//   AVR-Temp Sensor      
//------------------------------------------------------------------------------
// Based on the publication:   
//		Using DS18B20 digital temperature sensor on AVR microcontrollers
//		Description and application, Version 1.0 (Preliminary)
//		by Gerard Marull Paretas, September 2007
//------------------------------------------------------------------------------
// Fuses 
//  set 8MHz:		
//  read current:	avrdude -v -c avrispv2 -P usb -p m8
//  ATmega8
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
				
		// calculate digits
		char digit3 = digit/100;
		char kalan = digit%100;
		char digit2 = kalan/10;                      
		char digit1 = kalan%10;	
		
		// calculate digits of the decimal part
		char decimal4 = decimal/1000;	
		kalan = decimal%1000;
		char decimal3 = kalan/100;
		kalan = decimal%100;
		char decimal2 = kalan/10;                      
		char decimal1 = kalan%10;	

		// print values to 7 segment display
		matrix_transmit(1, digit_map[digit2]);
		matrix_transmit(2, (digit_map[digit1]) | 0b10000000);  // Light up the "dot" in the digit
		//matrix_transmit(4, digit_map[decimal4]);
		//matrix_transmit(5, digit_map[decimal3]);
		//matrix_transmit(6, digit_map[decimal2]);
		//matrix_transmit(7, digit_map[decimal1]);
		
		//_delay_ms(1000);
	}
	
	return 0;
}   


