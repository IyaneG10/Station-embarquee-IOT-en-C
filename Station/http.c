#include <getopt.h>
#include "libcom.h"
#include "libthrd.h"
#include "http.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <fcntl.h>
#include <errno.h>
#include <netdb.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <poll.h>

void EnvoiFichier(char * chemin_fichier,FILE * dialogue)
{
	FILE * fichier;
	fichier = fopen(chemin_fichier, "r");
	long taille_fichier;
	char * buffer;
	size_t copie_fichier;
	char entete[50]="HTTP/1.1 200 OK\n\n";

	// allocate memory to contain the whole file:
	buffer = (char*) malloc (sizeof(char)*taille_fichier);

	fseek (fichier , 0 , SEEK_END);
	taille_fichier = ftell (fichier);
	rewind (fichier);


	// copy the file into the buffer
	if ((fread (buffer,1,taille_fichier,fichier)) != taille_fichier) {fputs ("http: Probleme lecture fichier",stderr); exit (3);}
	fprintf(dialogue,"%s",entete);
	fclose (fichier);
	free (buffer);

}
void gestionClientTCP(int s)
{
	//int s=*(int *)arg;
	/* Obtient une structure de fichier */
	FILE *dialogue=fdopen(s,"a+"); // acces à un fichier en mode lecture et écriture et append (ne pas écraser)
	if(dialogue==NULL){ perror("gestionClientTCP.fdopen"); exit(EXIT_FAILURE); }

	/* Echo */
	char ligne[MAX_LIGNE];
	char Acceuil[1]="/";
	char chemin_fichier[MAX_LIGNE]="../HTML";
	char chaine1[10000]="";
	char Content_type[50]="\nContent-Type: ";
	char cmd[MAX_LIGNE],page[MAX_LIGNE],protocole[MAX_LIGNE],temp[MAX_LIGNE];

	fgets(ligne,MAX_LIGNE,dialogue);
	sscanf(ligne,"%s %s %s",cmd,page,protocole);//a finir
	printf("%s %s %s\n",cmd,page,protocole );
	strcat(chemin_fichier,page);


	while(fgets(ligne,MAX_LIGNE,dialogue) !=NULL)//while(fgets(ligne,MAX_LIGNE,dialogue)) //while(fgets(ligne,MAX_LIGNE,dialogue) !=NULL && strlen(ligne) < 3)
	{
		//printf("\n LA COMMANDE : %s\n",page );
		// if (strncmp("/favicon.ico",page,strlen(page))){
		//     strcpy(Content_type,"image/x-icon");
		//     }
		// else if (strncmp("/images/affich.jpeg",page,strlen(page))){
		//     strcpy(Content_type,"image/jpeg");
		//     } 

		if (strlen(ligne)<3)
		{
			printf("\n BREAK\n");
			break;
		}
	}

	printf("ligne=%s\n",ligne);

	if (strncmp(Acceuil,page,strlen(page)) == 0 )
	{
		strcpy(chemin_fichier,"../HTML/acceuil.html");
	}
	printf("chemin_fichier : %s\n",chemin_fichier);

	EnvoiFichier(chemin_fichier,dialogue);



	printf("*************ligne2>**********\n %s\n",ligne);


	/* Termine la connexion */
	fclose(dialogue);
	printf("client déconnecté\n");

}


int multi_gestionClientTCP(int s)
{
	//int dialogue= s;
	lanceThread(gestionClientTCP,(int) s,0);
	return 0;
}


void analyseArguments(int argc,char *argv[])
{
	if (argc<2) 
	{
		fprintf(stderr,"respecter la syntaxe suivante: %s [nom de machine][numero_de_port] \n",argv[0]); 
		exit(EXIT_FAILURE);
	}
}


void serveur_TCP(void)
{
	int s=0;
	//   /* Lecture des arguments de la commande */
	//   //analyseArguments(argc,argv[0]); // à améliorer


	//char * machine = argv[0];
	//char *port= argv[1]; 
	char *machine="localhost"; // equivalent de 127.0.0.1
	char *port="4444"; // faire attention à ne pas choisir un port utilisé par le système
	int connexions= 6; //maximum 6 clients dans la file d'attente TCP


	/* Initialisation du serveur */
	s=initialisationServeurTCP(port,connexions);

	/* Lancement de la boucle d'ecoute */
	//boucleServeurTCP(s,gestionClientTCP);
	boucleServeurTCP(s,multi_gestionClientTCP);


	/* On termine la connexion */
	shutdown(s,SHUT_RDWR);
	/* PARTIE UDP EN COMMENTAIRE CAR DOIT ETRE EXECUTEE EN PARALELLE */


}
