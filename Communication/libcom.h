/** fichier libcom.h **/
#ifndef H_LIBCOM
#define H_LIBCOM
#include <sys/types.h>
#include <sys/socket.h>

/******************************************************************/
/** Ce fichier decrit les structures et les constantes utilisees **/
/** par les fonctions de gestion des sockets                     **/
/******************************************************************/

/**** Constantes ****/

/**** Fonctions ****/


int initialisationServeurTCP(char *service,int connexions);
int boucleServeurTCP(int s,int (*traitement)(int));  
int clientTCP(char *hote,short int *service );
  



#endif
