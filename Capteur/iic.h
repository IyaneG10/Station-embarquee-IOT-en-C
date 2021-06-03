/******************************************
*  i2c minimal library public definitions *
******************************************/

#pragma once

#define		ERROR	0
#define		SUCCESS	1

void TWI_init(void);
uint8_t TWI_writecmd(uint8_t address,uint8_t com_add);
uint8_t TWI_writedata(uint8_t address,uint8_t com_add,uint8_t data);
