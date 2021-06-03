#######################################################################################################################################################################
                                                    		    	INTRODUCTION
#######################################################################################################################################################################

L’objectif est de concevoir une station de vérification de la qualité environnementale avec des capteurs de particules et des gaz connectés à un réseau TCP/IP et un serveur Web permettant d’afficher ergonomiquement les valeurs des capteurs. 

(cf: https://rex.plil.fr/Enseignement/Systeme/Tutorat.IMA2a4.CapteurPollution/pollution001.html)




#######################################################################################################################################################################
                                                    		    PROGRAMMATION MODULAIRE
#######################################################################################################################################################################

La modularité de ce projet est définie selon 4 répertoire:

--> Capteur
	-> broadcast.c (programme principale à téléverser sur l'arduino)
	-> des bibliothèques pour la programmation en C sur AVR
	-> un fichier Makefile pour la compilation avec avr-gcc et de téléversement avec avr-dude

--> Communication: 
	-> codes de fonctions pour la gestion du réseau
	-> un fichier Makefile pour la compilation de bibliothèque

--> Station: 
	-> station.c (programme principale pour la gestion des serveur http en tcp et udp pour l'arduino)
	-> un fichier Makefile pour la compilation avec avr-gcc et de téléversement avec avr-dude

--> Threads: 
	-> fonctions de gestion des processus légers


	-> un fichier Makefile pour la compilation 


#######################################################################################################################################################################
                                                         	    PARTIE ARDUINO
#######################################################################################################################################################################

Aprés avoir effectué les configurations nécessaires grâce aux bibliothèques fournies, nous avons "pingé" l'arduino pour vérifier que le shield ethernet fonctionne bien et communique avec l'arduino grâce au protocole SPI.
Ainsi, récupère la valeur analogique du capteur sur la broche A0 et dés qu'ilreçoit un message de la part du serveur UDP dans le PC, il récupère son adresse ainsi que le numéro du service pour lui envoyer les données.
Les données en question sont formées de 5 octects dont les 2 de poids faibles sont les valeurs des capteurs en héxadécimal.

A cela, s'ajoute notre PCB que nous avons crée avec le logiciel Fritzing.

Le matériel complet sera mis dans la boite fournie avec l'indication: IMA2A4 PSR5

Le programme est téleversé sur un arduino nano avec 57600 bauds et ttyUSB0 pour l'arduino.




#######################################################################################################################################################################
                                                    		    GESTION DU SERVEUR UDP
#######################################################################################################################################################################

Aprés avoir initialisé notre serveur, on se met dans une boucle d'attente de message de client. Ainsi un  broadcast est émis sur le réseau toutes les 5 secondes et dés qu'un message est reçu,une fonction lance un thread pour qu'une socket de dialogue soit ouverte et que les messages soient stockés à la fois dans un fichier sous format JSON et des fichiers textes avec les valeurs exactes des capteurs.
Pour la conversion, nous utilisons les éléments fournis dans la datasheet du capteur gp2y1010au0f . 
Toutes les données sont stockée dans le répertoire HTML.



#######################################################################################################################################################################
                                                    		    GESTION DU SERVEUR TCP 
#######################################################################################################################################################################

Cette partie concerne plus la gestion des requêtes http envoyées lorsque le client se connecte.En fonction de la requête, le serveur envoie les informations demandées afin que le navigateur web puisse accéder aux données des capteurs en afficher sur une page les valers instantannées et sur une autre l'évolution temporelle.
La programmation du client web s'est faite en html avec du code CSS pour la mise en forme et en couleur et du javascript pour rendre le site dynamique en récupérant les fichiers de mesures et les traitments requis.
Cependant, faute de temps, nous n'arrivons pas à mettre les valeurs les plus récentes des capteurs en début de fichier malgré l'utilisation des fonctions rewind et fssek. De ce, fait le changement des valeurs instantanées ne se fait pas. 

Vu que cette partie est susceptible d'avoir quelques dysfonctionnement lors du développemtn du code, nous avons utilisé la compilation conditionnelle avec la macro #if DEBUG pour afficher sur le terminal l'évolution du code.





#######################################################################################################################################################################
                                                    		    	CONCLUSION
#######################################################################################################################################################################

En somme, les parties les plus importantes ont été validées et les codes fonctionnent correctement à l'exception de quelques warning qui ne posent aucun problème à l'éxécution.
Dans, la fonction main, le serveur UDP et le serveur TCP sont lancés grâce à un fork pour qu'ils s'éxécutent en parallèle sans avoir à partager un espace mémoire.

Cependant, par manque de temps, nous avons dû faire certains choix et contourner la consigne de base afin d'assurer la fonctionnabilité du projet:

--> C'est ainsi que nous avons mis des valeurs aléatoires entre 0 et 1024 car le capteur ne semble pas fonctionner. Des essais ont été effectués avec l'IDE arduino (plus facilement programmable) mais on obtient toujours 0

--> Notre processus UDP interroge les capteurs tous les 5 en broadcast (même si cela pourrait saturer le réseau) et ces derniers répondent. Ainsi les valeur sont stockés sous format JSON mais aussi dans un fichier texte par capteur. Cependant, c'est le fichier .txt qui sera utilisé par notre navigateur pour afficher les valeurs instantanées et les graphes

--> Il est possible de compiler tout le code à partir du makefile de la racine, cependant nous lançons l'exécution avec [./station] dans le répertoire Station pour les serveurs et dans le répertoire Capteur nous téléversons le programme vers l'arduino avec [sudo make upload].

--> Enfin, nous n'utilisons pas de sémaphore pour la gestion des ressources partagées






