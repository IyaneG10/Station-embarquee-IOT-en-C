#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>

#include "serial.h"
#include "ethernet.h"
#include "w5100.h"
#include "socket.h"
#include "string.h"

#include "analog.h"
#include "digital.h"
#include "spi.h"
#include "iic.h"

#define RAND_MAX	3FF // valeur maximale capteur de 1023
#define MAC_SIZE	6
#define IPV4_SIZE	4
#define DATA_SIZE	5


/* 		---------------------------------------------	Variables globales		---------------------------------------------	*/	
unsigned char data[DATA_SIZE];
int valeur_capteur;
/*-----------------------------------------------------------------------------------------------------------------------------------------	*/




int main(void)
{

	int valeur_capteur= 0;

	/* 		---------------------------------------------	GESTION DES PINS DEL'ATMEGA328P	---------------------------------------------	*/	
	ad_init(0);// initialiser CAN sur la broche AO de l'arduino = pin 14 atmega
	io_init();// pin 8 à 11 en sortie & pin 2 à 7 et pin 12 (MISO) en entrée (car j'utilise un arduino Uno pour mes test)
	output_set(8,0); // mettre led capteur en ON ==> set_led_capteur (by Malick)
	//input_get(pin);
	/* 	-------------------------------------------------------------------------------------------------------------------------------	*/		



	/*		------------------------------------	GESTION DU MODULE ETHERNET A BASE DE W51000	------------------------------------*/			
	//169.254.114.32 mon fichu adaptateur 
	uint8_t mac[] = { 0xD0, 0xAD, 0xBE, 0x00, 0x00, 0x00 };
	uint8_t ip[] = {172,26,145,55}; // sur reseau de  bridge
	//uint8_t ip[] = {10,10,10,5}; // machie Hamza
	uint8_t mask[] = {255,255,255,0};
	uint8_t gateway[] = {172,26,145,100}; // doit etre sur le meme reseau mais different de l'ip du truc
	//uint8_t gateway[] = {10,10,10,111}; // toujours machine Hamza

	ethernet_init( mac, ip, gateway, mask); // initialiser le module ethernet grâce à ethernet.h (appel à w5100.h pour comléments)

	/*		-------------------------------------------------------------------------------------------------------------------------------------	*/



	/* ---------------------------------------------      GESTION DU CLIENT UDP ARDUINO   ---------------------------------------------*/	

	uint8 protocol=Sn_MR_UDP; //0x02 ==> client UDP 
	uint8_t  addr[IPV4_SIZE]; // adresse de la machine distante	
	uint16 port=1234;
	uint8 flag=0;
	uint8 buf[DATA_SIZE];
	SOCKET s=3;
	socket( s, protocol,  port, flag); 

	while(1)
	{
		if (recvfrom( s, buf, sizeof(buf), addr,  &port) > 0) // Attente de réception de message de la part du PC
		{
			{
				/* 		---------------------------------	GESTION DATA   UDP		-----------------------------------	*/		
				uint8_t data[DATA_SIZE];
				//valeur_capteur=	ad_sample(); 
				valeur_capteur=	rand(); // car notre capteur ne semble pas fonctionner correctement
				data[0] = (valeur_capteur) & 0xFF;
				data[1] = (valeur_capteur >> 8)  & 0xFF;
				data[2] = 5; // identifiant capteur 1 octet
				data[3] = 'x'; // nombre magique 1/2 octets
				data[4] = '7'; // nombre magique 2/2 octets
				uint16 len=sizeof(data);
				/*		----------------------------------------------------------------------------------------	*/
				sendto(s,data,len,addr,port);
			}
		}
	}
	close(s);
	return 0;
}



