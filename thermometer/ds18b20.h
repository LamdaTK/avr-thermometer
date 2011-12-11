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

#ifndef F_CPU
#define F_CPU 8000000UL 
#endif

#include <util/delay.h> 
// LCD
#include <stdlib.h>
#include <avr/pgmspace.h>

#define LOOP_CYCLES	10	//Number of cycles that the loop takes
#define us(num)		(num/(LOOP_CYCLES*(1/(F_CPU/1000000.0))))

/* Thermometer Connections (At your choice) */
#define THERM_PORT           PORTC
#define THERM_DDR            DDRC
#define THERM_PIN            PINC
#define THERM_DQ             PC0

/* Utils */
#define THERM_INPUT_MODE()	THERM_DDR&=~(1<<THERM_DQ)
#define THERM_OUTPUT_MODE() THERM_DDR|=(1<<THERM_DQ)
#define THERM_LOW()			THERM_PORT&=~(1<<THERM_DQ)
#define THERM_HIGH()		THERM_PORT|=(1<<THERM_DQ)

#define THERM_CMD_CONVERTTEMP   0x44
#define THERM_CMD_RSCRATCHPAD   0xbe
#define THERM_CMD_WSCRATCHPAD   0x4e
#define THERM_CMD_CPYSCRATCHPAD 0x48
#define THERM_CMD_RECEEPROM     0xb8
#define THERM_CMD_RPWRSUPPLY    0xb4
#define THERM_CMD_SEARCHROM     0xf0
#define THERM_CMD_READROM       0x33
#define THERM_CMD_MATCHROM      0x55
#define THERM_CMD_SKIPROM       0xcc
#define THERM_CMD_ALARMSEARCH   0xec
