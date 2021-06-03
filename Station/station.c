/** Fichier station.c **/

/******************************************************************/
/** Serveur pour la station de pollution                         **/
/******************************************************************/

/** Fichiers d'inclusion **/
#include <getopt.h>
#include "libcom.h"
#include "libthrd.h"
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

/** Constantes **/
#define MAX_LIGNE 2048
#define MAX_TAMPON 1024
#define MAX_UDP_MESSAGE 256


/** Variables publiques **/

/** Variables statiques **/

/** Procedure principale **/

int adresse=0;

void EnvoiFichier(char * chemin_fichier,FILE * dialogue)
{
	FILE * fichier;
	fichier = fopen(chemin_fichier, "r");
	long taille_fichier;
	char * buffer;
	size_t comp_texte;
	char req_http[50]="HTTP/1.1 200 OK\n\n";

	fseek (fichier , 0 , SEEK_END);
	taille_fichier = ftell (fichier);
	rewind (fichier);

	// allocate memory to contain the whole file:
	buffer = (char*) malloc (sizeof(char)*taille_fichier);
	if (buffer == NULL) {fputs ("http:Probleme mémoire (malloc)",stderr); exit (2);}

	// copy the file into the buffer:
	comp_texte = fread (buffer,1,taille_fichier,fichier);
	if (comp_texte != taille_fichier) {fputs ("http: Probleme lecture fichier",stderr); exit (3);}
	fprintf(dialogue,"%s",req_http);
	fwrite(buffer,1,taille_fichier,dialogue);
	fclose (fichier);
	free (buffer);

}


int initialisationSocketUDP(char *service)
{
	//printf("debut code");
	struct addrinfo precisions,*resultat,*origine;

	int statut;
	int s;

	/* Construction de la structure adresse */
	memset(&precisions,0,sizeof precisions);
	precisions.ai_family=AF_UNSPEC;
	precisions.ai_socktype=SOCK_DGRAM;
	precisions.ai_flags=AI_PASSIVE;
	statut=getaddrinfo(NULL,service,&precisions,&origine);
	if(statut<0){ perror("initialisationSocketUDP.getaddrinfo"); exit(EXIT_FAILURE); }
	struct addrinfo *p;
	for(p=origine,resultat=origine;p!=NULL;p=p->ai_next)
		if(p->ai_family==AF_INET6){ resultat=p; break; }

	/* Creation d'une socket */
	s=socket(resultat->ai_family,resultat->ai_socktype,resultat->ai_protocol);
	if(s<0){ perror("initialisationSocketUDP.socket"); exit(EXIT_FAILURE); }

	/* Options utiles */
	int vrai=1;
	if(setsockopt(s,SOL_SOCKET,SO_REUSEADDR,&vrai,sizeof(vrai))<0){
		perror("initialisationServeurUDPgenerique.setsockopt (REUSEADDR)");
		exit(-1);
	}
	int broadcast=1;
	setsockopt(s, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast));

	/* Specification de l'adresse de la socket */
	statut=bind(s,resultat->ai_addr,resultat->ai_addrlen);
	if(statut<0) {perror("initialisationServeurUDP.bind"); exit(-1);}

	/* Liberation de la structure d'informations */
	freeaddrinfo(origine);

	// printf("test %d\n",addr_broadcast->ai_addrlen);

	return s;
}


void versFichierTXT(unsigned char *data)
{
	FILE * fp;
	unsigned char *message = data;
	char capt =message[2];

	switch (capt)
	{

		case '1':
			fp = fopen ("../HTML/data/capteur1.txt","a+");
			break;

		case '2':
			fp = fopen ("../HTML/data/capteur2.txt","a+");
			break;

		case '3':
			fp = fopen ("../HTML/data/capteur3.txt","a+");
			break;

		case '4':
			fp = fopen ("../HTML/data/capteur4.txt","a+");
			break;

		case '5':
			fp = fopen ("../HTML/data/capteur5.txt","a+");
			break;

		case '6':
			fp = fopen ("../HTML/data/capteur6.txt","a+");
			break;
	}
	//rewind(fp);
	int cap1=(int)message[1] - 48; // car ascii 0
	int cap0=(int)message[0] - 48;
	int cap=(255*cap1 + 1*cap0);
	float valeur_effective_capteur= ((cap*5.0/1024.0)*0.17)-0.1 ;// conversion de la valeur arduino (source internet)
	//int val_cap=cap0;
	fseek( fp, 0, SEEK_SET );
	fprintf (fp, "%f\n",valeur_effective_capteur);
	fclose (fp);

}



void versFichierJSON(unsigned char *data)
{
	FILE * fp;
	unsigned char *message = data;

	char capt =message[2];

	switch (capt)
	{

		case '1':
			fp = fopen ("../HTML/data/JSON/capteur1.json","at");
			break;

		case '2':
			fp = fopen ("../HTML/data/JSON/capteur2.json","at");
			break;

		case '3':
			fp = fopen ("../HTML/data/JSON/capteur3.json","at");
			break;

		case '4':
			fp = fopen ("../HTML/data/JSON/capteur4.json","at");
			break;

		case '5':
			fp = fopen ("../HTML/data/JSON/capteur5.json","at");
			break;

		case '6':
			fp = fopen ("../HTML/data/JSON/capteur6.json","at");
			break;
	}


	fprintf (fp, "{");
	fprintf (fp, "\r\t \"Magic 2/2\": %c,",message[4]);
	fprintf (fp, "\r\t \"Magic 1/2\": \"%c\",",message[3]);
	fprintf (fp, "\r\t \"ID capteur\": %c,",message[2]);
	fprintf (fp, "\r\t \"Octet 2/2\": %d,",message[1]);
	fprintf (fp, "\r\t \"Octet 1/2\": %d",message[0]);
	fprintf (fp, "\r},\r");

	fclose (fp);

}


void gestionClientUDP(unsigned char *message,int nboctets) //
{

	/* Obtient une structure de fichier */
	int s=3; // pour forcer

	FILE *dialogue=fdopen(s,"a+");
	if(dialogue==NULL){ perror("gestionClientUDP.fdopen"); exit(EXIT_FAILURE); }
	/* Echo */
	char ligne[MAX_LIGNE];
	//if(nboctets>=5)
	//if(message[2] == 5)
	// {
	printf("\n\rMessage arduino: %c %c %c %c %c\n",message[4],message[3],message[2],message[1],message[0]);//

	//}
	//if (nboctets==5)
	{
		versFichierJSON(message);
		versFichierTXT(message);
	}
}

int multi_gestionClientUDP(unsigned char *message)
{
	lanceThread(gestionClientUDP,(unsigned char *) message,5);
	#if DEBUG
	printf("\n\r***********THREAD UDP  OK********\n");
	#endif
	return 0;
}

void broadcast_UDP(int s)
{
	struct addrinfo precisions_broadcast,*resultat_broadcast,*origine_broadcast;
	int statut;

	memset(&precisions_broadcast,0,sizeof precisions_broadcast);
	precisions_broadcast.ai_family=AF_UNSPEC;
	precisions_broadcast.ai_socktype=SOCK_DGRAM;
	statut=getaddrinfo("172.26.145.255","1234",&precisions_broadcast,&origine_broadcast);
	if(statut<0){ perror("initialisationSocketUDP.getaddrinfo"); exit(EXIT_FAILURE); }
	struct addrinfo *addr_broadcast;
	for(addr_broadcast=origine_broadcast,resultat_broadcast=origine_broadcast;addr_broadcast!=NULL;addr_broadcast=addr_broadcast->ai_next)
		if(addr_broadcast->ai_family==AF_INET){ resultat_broadcast=addr_broadcast; break; }

	while(1)
	{
		sendto(s,"x7",strlen("x7"),0,addr_broadcast->ai_addr,addr_broadcast->ai_addrlen);
		#if DEBUG
		printf("\n\r***********THREAD BROADCAST OK********\n");
		#endif
		sleep(5);
	}
}

int boucleServeurUDP(int s,int (*copie_fichier)(unsigned char *,int))
{
	lanceThread(broadcast_UDP,(int) s,0);
	while(1)
	{
		struct sockaddr_storage adresse;
		socklen_t taille_fichier_fichier=sizeof(adresse);
		unsigned char message[MAX_UDP_MESSAGE];
		int nboctets=recvfrom(s,message,MAX_UDP_MESSAGE,0,(struct sockaddr *)&adresse,&taille_fichier_fichier);
		if(nboctets<0) return -1;
		if(copie_fichier(message,nboctets)<0) break;
	}
	return 0;
}









void gestionClientTCP(int s)
{
	/* Obtient une structure de fichier */
	FILE *dialogue=fdopen(s,"a+"); // acces à un fichier en mode lecture et écriture et append (ne pas écraser)
	if(dialogue==NULL){ perror("gestionClientTCP.fdopen"); exit(EXIT_FAILURE); }


	char ligne[MAX_LIGNE];
	char Acceuil[1]="/";
	char chemin_fichier[MAX_LIGNE]="../HTML";
	char chaine1[10000]="";
	char Content_type[50]="\nContent-Type: ";
	char cmd[MAX_LIGNE],page[MAX_LIGNE],protocole[MAX_LIGNE],temp[MAX_LIGNE];

	fgets(ligne,MAX_LIGNE,dialogue);
	sscanf(ligne,"%s %s %s",cmd,page,protocole);
	#if DEBUG
	printf("%s %s %s\n",cmd,page,protocole );
	#endif
	strcat(chemin_fichier,page);


	while(fgets(ligne,MAX_LIGNE,dialogue) !=NULL)
	{
		if (strlen(ligne)<3)
		{
			#if DEBUG
			printf("\n BREAK\n");
			#endif
			break;
		}
	}
	#if DEBUG
	printf("ligne=%s\n",ligne);
	#endif
	if (strncmp(Acceuil,page,strlen(page)) == 0 )
	{
		strcpy(chemin_fichier,"../HTML/acceuil.html");
	}
	#if DEBUG
	printf("chemin_fichier : %s\n",chemin_fichier);
	#endif

	EnvoiFichier(chemin_fichier,dialogue);

	#if DEBUG
	printf("*************ligne2>**********\n %s\n",ligne);
	#endif

	/* Termine la connexion */
	fclose(dialogue);
	#if DEBUG
	printf("client déconnecté\n");
	#endif

}


int multi_gestionClientTCP(int s)
{
	lanceThread(gestionClientTCP,(int) s,0);
	return 0;
}


void analyseArguments(int argc,char *argv[])
{
	if (argc<3) 
	{
		fprintf(stderr,"respecter la syntaxe suivante: %s -p <port> OU --p <port> \n",argv[0]); 
		exit(EXIT_FAILURE);
	}
}



void serveur_UDP(void)
{
	int s_udp=0;
	s_udp=initialisationSocketUDP("1234");
	boucleServeurUDP(s_udp,multi_gestionClientUDP);
	close(s_udp);

}


void serveur_TCP(void)
{
	int s=0;
    
	//   /* Lecture des arguments de la commande */
	//   //analyseArguments(argc,argv[0]); // à améliorer

	//char *port= argv[2]; 
	char *machine="localhost"; // equivalent de 127.0.0.1
	char *port="4444"; // faire attention à ne pas choisir un port utilisé par le système
	int connexions= 6; //maximum 6 clients dans la file d'attente TCP


	/* Initialisation du serveur */
	s=initialisationServeurTCP(port,connexions);

	/* Lancement de la boucle d'ecoute */
	boucleServeurTCP(s,multi_gestionClientTCP);

	/* On termine la connexion */
	shutdown(s,SHUT_RDWR);
	
}

int main(int argc,char *argv[])
{
	pid_t idFork;
	idFork = fork();

	/* Processus fils*/
	if        (idFork == 0) 
    {
		serveur_TCP();
	} 
	/* Processus père*/	
	else if (idFork != -1) 
    {
		serveur_UDP();
	} 
	else 
    {
		perror("Problème de crétation de fork");
	}

	return 0;
}
