#include <avr/io.h>
#include <util/delay.h>
#include "max7219.h"

void SPI_MasterInit(void)
{
	// Set MOSI, SCK, and SS as output
	SPI_DDR = (1<<SPI_MOSI)|(1<<SPI_SCK)|(1<<SPI_SS);
	// Enable SPI, Master, set clock rate fck/16
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}
void SPI_MasterTransmit(char cData)
{
	// Start transmission
	SPDR = cData;
	// Wait for transmission complete
	while(!(SPSR & (1<<SPIF)));
	
	// Read data
	cData = SPDR;
}

void matrix_transmit(char address, char data)
{
	SPI_PORT &= ~(1<<SPI_SS);
	
	SPI_MasterTransmit(address);
	SPI_MasterTransmit(data);
	
	SPI_PORT |= (1<<SPI_SS);
}

void matrix_clear(void)
{
	for(int i = 1; i <= 8; i++)
		matrix_transmit(i,0);
}

void matrix_init(void)
{
	SPI_MasterInit();
	
	matrix_transmit(OP_SHUTDOWN,0b00000000);
	matrix_clear();
	matrix_transmit(OP_DISPLAYTEST,0x00);
	matrix_transmit(OP_SCANLIMIT,0x07);
	matrix_transmit(OP_DECODEMODE,0x00);
	matrix_transmit(OP_INTENSITY,0x0F);
	matrix_transmit(OP_SHUTDOWN,0b00000001);
}

void matrix_demo1(void)
{
	uint8_t i,j;
	while(1)
	{
		for(uint8_t row = 0; row < 8; row++)
		{
			for(uint8_t col = 0; col < 8; col++)
			{
				i = row+1;
				j = ((row%2)?(col):(7-col));
				
				matrix_transmit(i, (1 << j));
				_delay_ms(100);
				matrix_transmit(i, 0);
			}
		}
		for(uint8_t col = 0; col < 8; col++)
		{
			for(uint8_t row = 0; row < 8; row++)
			{
				i = (col%2)?(row+1):(8-row);
				j = col;
				
				matrix_transmit(i, (1 << j));
				_delay_ms(100);
				matrix_transmit(i, 0);
			}
		}
	}
}

void digits(void)
{	
	while(1)
	{
		for(uint8_t row = 1; row <= 8; row++) 
		{				
			matrix_transmit(row, DIGIT_0);  // 0
			_delay_ms(100);
			matrix_transmit(row, DIGIT_1);  // 1
			_delay_ms(100);
			matrix_transmit(row, DIGIT_2);  // 2
			_delay_ms(100);
			matrix_transmit(row, DIGIT_3);  // 3
			_delay_ms(100);
			matrix_transmit(row, DIGIT_4);  // 4
			_delay_ms(100);
			matrix_transmit(row, DIGIT_5);  // 5
			_delay_ms(100);
			matrix_transmit(row, DIGIT_6);  // 6
			_delay_ms(100);
			matrix_transmit(row, DIGIT_7);  // 7
			_delay_ms(100);
			matrix_transmit(row, DIGIT_8);  // 8
			_delay_ms(100);
			matrix_transmit(row, DIGIT_9);  // 9
			_delay_ms(100);
			
			for(uint8_t col = 0; col <= 8; col++) 
			{
				matrix_transmit(col, 0);	
			}
		}		
		
	}
		
}
