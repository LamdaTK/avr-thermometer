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
#include <avr/io.h> 
#include <avr/interrupt.h> 
#define F_CPU 8000000UL 
#include <util/delay.h> 

#include <stdlib.h>

#include "ds18b20.h"

inline __attribute__((gnu_inline)) void therm_delay(uint16_t delay){
	while(delay--) asm volatile("nop");
}

uint8_t therm_reset() {
	uint8_t i;
	// Pull line low and wait for 480uS
	THERM_LOW();
	THERM_OUTPUT_MODE();
	//therm_delay(us(480));
	_delay_loop_2(960);
	
	//Release line and wait for 60uS
	THERM_INPUT_MODE();
	//therm_delay(us(60));
	_delay_loop_1(160);
	//Store line value and wait until the completion of 480uS period
	i=(THERM_PIN & (1<<THERM_DQ));
	//therm_delay(us(420));
	_delay_loop_2(840);
	//Return the value read from the presence pulse (0=OK, 1=WRONG)
	return i;
}

void therm_write_bit(uint8_t bit){
	//Pull line low for 1uS
	THERM_LOW();
	THERM_OUTPUT_MODE();
	//therm_delay(us(1));
	_delay_loop_1(3);
	//If we want to write 1, release the line (if not will keep low)
	if(bit) THERM_INPUT_MODE();
	//Wait for 60uS and release the line
	//therm_delay(us(60));
	_delay_loop_1(160);
	THERM_INPUT_MODE();
}

uint8_t therm_read_bit(void){
	uint8_t bit=0;
	//Pull line low for 1uS
	THERM_LOW();
	THERM_OUTPUT_MODE();
	//therm_delay(us(1));
	_delay_loop_1(3);
	
	//Release line and wait for 14uS
	THERM_INPUT_MODE();
	//therm_delay(us(14));
	_delay_loop_1(37);
	
	//Read line value
	if(THERM_PIN&(1<<THERM_DQ)) bit=1;
	//Wait for 45uS to end and return read value	
	//therm_delay(us(45));
	_delay_loop_1(120);
	return bit;
}

uint8_t therm_read_byte(void){
	uint8_t i=8, n=0;
	while(i--){
		//Shift one position right and store read value
		n>>=1;
		n|=(therm_read_bit()<<7);
	}
	return n;
}

void therm_write_byte(uint8_t byte){
	uint8_t i=8;
	while(i--){
		//Write actual bit and shift one position right to make the next bit ready
		therm_write_bit(byte&1);
		byte>>=1;
	}
}


#define THERM_DECIMAL_STEPS_12BIT 625 //.0625
#define THERM_DECIMAL_STEPS_9BIT 500 //.500

void therm_read_temperature(int8_t *digit_part,  uint16_t *decimal_part){
	// Buffer length must be at least 12bytes long! ["+XXX.XXXX C"]
	uint8_t temperature[2];
	int8_t digit;
	uint16_t decimal;
	
	//Reset, skip ROM and start temperature conversion
	therm_reset();
	therm_write_byte(THERM_CMD_SKIPROM);
	therm_write_byte(THERM_CMD_CONVERTTEMP);
	//Wait until conversion is complete
	while(!therm_read_bit());
	//Reset, skip ROM and send command to read Scratchpad
	therm_reset();
	therm_write_byte(THERM_CMD_SKIPROM);
	therm_write_byte(THERM_CMD_RSCRATCHPAD);
	//Read Scratchpad (only 2 first bytes)
	temperature[0]=therm_read_byte();
	temperature[1]=therm_read_byte();
	therm_reset();
	
	//Store temperature integer digits and decimal digits
	digit=temperature[0]>>4;
	digit|=(temperature[1]&0x7)<<4;
	//Store decimal digits
	decimal=temperature[0]&0xf;
	decimal*=THERM_DECIMAL_STEPS_12BIT;
	
	*digit_part = digit;
	*decimal_part = decimal;
	
	//Format temperature into a string [+XXX.XXXX C]
	//sprintf(buffer, "%+d.%04u C", digit, decimal);
	
	//sprintf(buffer, "%d.%d\n",temperature[0]/2, (temperature[0]&1)*5 );
	//usart_write_str(buffer);
}



