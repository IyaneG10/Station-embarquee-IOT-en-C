#include <avr/io.h>

/** Définition des entrés/sorties numériques  **/


/*
   Quelques macros utilisées
   Positionner bit à bit
#define bit_set(var,bitno) ((var) |= 1 << (bitno))
#define bit_clr(var,bitno) ((var) &= ~(1 << (bitno)))

Positionner plusieurs bits
#define bits_on(var,mask) var |= mask
#define bits_off(var,mask) var &= ~0 ^ mask

Mettre un bit à 1 avec la mcro _BV
_BV(x)

Tester un bit
#define testbit_on(data,bitno) ((data>>bitno)&0x01)
*/

void io_init(void)
{
	DDRB |= 0x0f; // Pins 8 to 11 as output et pin 12 mettre pin 12 == PB4 en entrée pour MISO
	DDRD &= 0x83;  // Pins 2 to 6 as input
	PORTD |= 0x0c; // Pull-up activated on pins 2 and 3
}

void output_set(int pin,unsigned char value)
{
	unsigned char mask=(1 << (pin-8));
	if(value==0) PORTB &= ~mask ; else PORTB |= mask;
}


unsigned char input_get(int pin)
{
	if(pin<2 || pin>6) return 0xff;
	unsigned char mask=(1 << pin);
	return ((PIND&mask)!=0)?1:0;
}



