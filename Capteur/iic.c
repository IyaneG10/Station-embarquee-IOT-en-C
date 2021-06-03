/************************
*  i2c minimal library  *
************************/

#include <stdint.h>
#include <avr/io.h>

#include "iic.h"

// Initialize i2c
void TWI_init(void)
{
	//set SCL to 400kHz
	TWSR = 0x00;
	TWBR = 0x0C;
	//enable TWI
	TWCR = (1<<TWEN);
}

// Send start signal
static void TWI_start(void)
{
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while ((TWCR & (1<<TWINT)) == 0);
}

// Send stop signal
static void TWI_stop(void)
{
	TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
}

// Get i2c bus status
static uint8_t TWIGetStatus(void)
{
	uint8_t status;
	//mask status
	status = TWSR & 0xF8;
	return status;
}

// Write single byte to bus
static void TWI_writebyte(uint8_t u8data)
{
	TWDR = u8data;
	TWCR = (1<<TWINT)|(1<<TWEN);
	while ((TWCR & (1<<TWINT)) == 0);
}

// Write i2c minimal message (command)
uint8_t TWI_writecmd(uint8_t address,uint8_t com_add)
{
	TWI_start();
	if (TWIGetStatus() != 0x08)
		return ERROR;
	TWI_writebyte(address<<1);
	if (TWIGetStatus() != 0x18)
		return ERROR;
	TWI_writebyte(com_add);
	if (TWIGetStatus() != 0x28)
		return ERROR;
	TWI_stop();
	return SUCCESS;
}

// Write i2c message (with a single data byte)
uint8_t TWI_writedata(uint8_t address,uint8_t com_add,uint8_t data)
{
	TWI_start();
	if (TWIGetStatus() != 0x08)
		return ERROR;
	TWI_writebyte(address<<1);
	if (TWIGetStatus() != 0x18)
		return ERROR;
	TWI_writebyte(com_add);
	if (TWIGetStatus() != 0x28)
		return ERROR;
	TWI_writebyte(data);
	if (TWIGetStatus() != 0x28)
		return ERROR;
	TWI_stop();
	return SUCCESS;
}
