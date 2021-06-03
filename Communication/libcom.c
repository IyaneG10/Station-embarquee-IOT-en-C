/** fichier libcom.c **/

/*****************************************************************/
/** Ce fichier contient des fonctions utilisees par le serveur  **/
/** de jeu et concernant les sockets.                           **/
/*****************************************************************/

/**** Fichiers d'inclusion ****/

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

#include "libcom.h"

/**** Constantes ****/

/**** Variables globales *****/
extern  int s;
extern  struct sockaddr_in adresse;
extern  socklen_t taille=sizeof adresse;
/**** Fonctions de gestion des sockets ****/





int initialisationServeurTCP(char *service,int connexions){
	struct addrinfo precisions,*resultat,*origine; // liste chainée pour trouver des adresses
	int statut; 
	int s; // descripteur de fichier pour la socket serveur TCP

	/* Construction de la structure adresse */
	memset(&precisions,0,sizeof precisions);
	precisions.ai_family=AF_UNSPEC; // Famille d'adresse: IP => AF_UNSPEC = AF_INET
	precisions.ai_socktype=SOCK_STREAM; // Type de socket: TCP (mode flux, connecté) => SOCK_STREAM 
	precisions.ai_flags=AI_PASSIVE; // On veut un serveur (ne fait qu'écouter) => AI_PASSIVE (AI_ACTIVE pour client)
	statut=getaddrinfo(NULL,service,&precisions,&origine);
	if(statut<0){ perror("initialisationServeur.getaddrinfo"); exit(EXIT_FAILURE); }
	struct addrinfo *p;
	for(p=origine,resultat=origine;p!=NULL;p=p->ai_next)
	{
		if(p->ai_family==AF_INET6){ resultat=p; break; } // chercher des adresses jusqu'à IP v6 
	}  
	/* Creation d'une socket */
	s=socket(resultat->ai_family,resultat->ai_socktype,resultat->ai_protocol);
	if(s<0){ perror("initialisationServeur.socket"); exit(EXIT_FAILURE); }
	printf("Socket TCP crée avec succes\n"); 

	/* Options utiles */
	int vrai=1;
	// socket=s; niveau"reseau (sol_socket)"=; option= adresse reutilisable
	if(setsockopt(s,SOL_SOCKET,SO_REUSEADDR,&vrai,sizeof(vrai))<0){
		perror("initialisationServeur.setsockopt (REUSEADDR)");
		exit(EXIT_FAILURE);
	}
	// socket=s; niveau"couche de transport (mode TCP)"=; option= pas de retard (delay) pour envoyer les messages
	if(setsockopt(s,IPPROTO_TCP,TCP_NODELAY,&vrai,sizeof(vrai))<0){
		perror("initialisationServeur.setsockopt (NODELAY)");
		exit(EXIT_FAILURE);
	}

	/* Specification de l'adresse de la socket */
	statut=bind(s,resultat->ai_addr,resultat->ai_addrlen);
	if(statut<0) return -1;
	printf("Socket TCP \"bindé\" avec succes\n"); 

	/* Liberation de la structure d'informations */
	freeaddrinfo(origine); // libérer la liste chainée

	/* Taille de la queue d'attente */
	statut=listen(s,connexions); // écoute sur la file d'attentes de taille=[connecions]
	printf("En attente de connexion avec les clients TCP\n");
	if(statut<0) return -1;

	return s;
}



int boucleServeurTCP(int s,int (*multigestionClientTCP)(int))
{
	int dialogue; // socket pour le dialogue entre serveur et clients
	while(1){

		/* Attente d'une connexion */
		if((dialogue=accept(s,(struct sockaddr *)&adresse,&taille))<0) return -1;
	
		//printf("client connecté\n");
		
		/* Passage de la socket de dialogue a la fonction de traitement */
		if(multi_gestionClientTCP(dialogue)<0){ shutdown(s,SHUT_RDWR); return 0;}
	}
	return s;
}



