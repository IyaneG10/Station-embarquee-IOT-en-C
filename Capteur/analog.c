#include <avr/io.h>

/** Definitions pour le Convertisseur Analogique Numérique **/

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

void ad_init(unsigned char channel)   
{   
	ADCSRA|=(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);//prescale = 128 (16MHz/128 = 125kHz) 
	ADMUX|=(1<<REFS0)|(1<<ADLAR);//select AVCC as reference // alignement à gauche du résultat 
	ADMUX=(ADMUX&0xf0)|channel;  //choix du convertisseur e (choix de la broche sur laquelle effectuer la conversion)
	ADCSRA|=(1<<ADEN);//autoriser la conversion
}   

unsigned int ad_sample(void)
{
	ADCSRA|=(1<<ADSC); // démarrer la conversion 
	while(bit_is_set(ADCSRA, ADSC)); // attendre la fin de la conversion
	return ADCH; // résultat de la conversion A/N (bits de poids faible car alignement à gauche, du coup on perd de la precision mais osef )
}


