# ENONCE DU PROJET

+--------------------------------------------------------------------------+
| Tutorat système & Capteur de pollution TCP/IP {.titlemain}               |
| =============================================                            |
|                                                                          |
| ### Xavier Redon & Thomas Vantroys {.titlerest}                          |
+--------------------------------------------------------------------------+

1  Description générale du projet {#sec1 .section}
---------------------------------

### 1.1  Objectif {#sec2 .subsection}

L’objectif est de concevoir une station de vérification de la qualité
environnementale avec des capteurs de particules et des gaz connectés à
un réseau TCP/IP et un serveur Web permettant d’afficher ergonomiquement
les valeurs des capteurs.

2  Description des composants du projet {#sec3 .section}
---------------------------------------

### 2.1  Architecture générale {#sec4 .subsection}

Les capteurs sont réalisés à l’aide d’un Arduino complété par un capteur
du taux de particules ainsi que par un bouclier Ethernet à base de
circuit WIZnet W5100. Ces capteurs diffusent leurs valeurs sur le réseau
TCP/IP local.

Il est aussi possible de concevoir une carte électronique à base de
micro-contrôleur ATMega328p intégrant une puce W5100.

La collecte des valeurs des capteurs est assurée par un processus Unix
qui joue aussi le rôle de serveur Web pour permettre un accès par les
utilisateurs aux données. Une page affiche les valeurs instantanées et
une autre les graphiques de valeurs au cours du temps. Les éléments
graphiques sont implantés en utilisant la technologie canvas de la norme
HTML5.

### 2.2  Diffusions UDP {#sec5 .subsection}

Les dispositifs à base d’ATMega328p envoient les valeurs des capteurs en
utilisant le protocole UDP. Les processus Unix doivent récupérer et
archiver ces valeurs de façon permanente. Il est demandé que les
dispositifs diffusent un identifiant sous la forme d’une chaîne de
caractères avec leurs valeurs. Le format des messages UDP est laissé à
votre bon soin. Il est évident qu’un format unique doit être adopté par
la promotion.

Les valeurs sont envoyées lorsqu’un message UDP de sollicitation est
reçu. Cela permet aux processus Unix de découvrir les capteurs à l’aide
d’une diffusion UDP pour ensuite solliciter les dispositifs en unicast.
Ces derniers ne générent donc que des paquets UDP unicast.

### 2.3  L’affichage des données {#sec6 .subsection}

Pour afficher les données de l’ensemble des capteurs connectés, votre
processus Unix doit se comporter comme un serveur Web. Le port d’écoute
du serveur doit pouvoir être passé en paramètre. Il vous faut décoder
les requêtes HTTP envoyées à votre processus. Vous avez juste besoin de
trouver le nom de la page demandée mais il faut quand même lire toutes
les entêtes des requêtes. Vous devez gérer les entêtes de réponse
Server, Date et Content-Type.

La page d’affichage des valeurs instantanées peut être une simple page
HTML avec un dispositif de rafraichissement périodique. Prenez soin
d’identifier les valeurs en utilisant les chaînes envoyés par les
dispositifs micro-contrôleur. Vous pouvez agrémenter l’affichage en
utilisant des routines javascript pour simuler des afficheurs 7 ou 14
segments disponibles sur Internet.

La page d’affichage des graphiques doit être à base de javascript
utilisant la technologie canvas d’HTML5 (vous pouvez utiliser une
bibliothèque). Les données sont récupérées dans les fichiers de
sauvegarde et transcrites sous forme de tableaux javascript. Les graphes
sont tracés par une routine javascript. Un plus est de permettre de
spécifier la plage d’affichage (jour, semaine, mois ou année).

### 2.4  Le matériel {#sec7 .subsection}

Chaque binôme reçoit un Arduino mini avec un breakout Ethernet et un
capteur GP2Y1010AU0F pour récupérer le taux de particules. Il vous est
demandé de concevoir un circuit imprimé pour porter l’Arduino, le
breakout, le capteur ainsi que quelques LEDs indicatives. Personne ne
vous empêche de réaliser une carte intégrant le micro-contrôleur
ATMega328p, le contrôleur Ethernet W5100 et le capteur.

La programmation du micro-contrôleur se fait en utilisant le compilateur
avr-gcc. L’algorithme à implanter sur le micro-contrôleur consiste en la
lecture des valeurs des capteurs et en leur envoi dans des datagrammes
UDP.

3  Organisation du travail {#sec8 .section}
--------------------------

### 3.1  Généralités {#sec9 .subsection}

Il est fortement conseillé de suivre les étapes proposées ci-après pour
réaliser le travail.

### 3.2  Organisation modulaire {#sec10 .subsection}

Le projet est constitué du programme pour le micro-contrôleur, de 2
bibliothèques et du programme tournant sous Unix. Chacune de ces entités
est développée dans un répertoire propre. On gère donc les quatre
répertoires suivants :

 Capteur -
:   pour les sources du programme chargé sur la plate-forme ATMega328p ;
Communication -
:   pour les sources de la bibliothèque contenant les fonctions de
    gestion réseau libcom ;
Threads -
:   pour les sources de la bibliothèque contenant les fonctions de
    gestion des threads libthrd ;
Station -
:   pour l’application de récupération et d’affichage des données
    météorologiques.

Cette arborescence et quelques squelettes de fichiers sont disponibles
sous forme d’un fichier au format tar et compressé à l’URL
[http://rex.plil.fr/Enseignement/Systeme/Tutorat.IMA2a4.CapteurPollution/pollution.tgz](http://rex.plil.fr/Enseignement/Systeme/Tutorat.IMA2a4.CapteurPollution/pollution.tgz).
Transférez ce fichier dans votre compte Polytech’Lille et
décompressez-le avec la commande tar xvzf pollution.tgz. Vous pouvez
constater que très peu de code est fourni, principalement quelques
bibliothèques et le squelette du code pour le micro-contrôleur.

Cependant, le répertoire créé contient déjà un fichier Makefile global
et des Makefile annexes dans chaque sous-répertoire. Vous avez un
exemple de Makefile permettant de générer un exécutable et un exemple de
Makefile permettant de construire une bibliothèque. Appuyez vous sur ces
deux exemples pour compléter les autres Makefile. Le projet doit pouvoir
être généré par la simple commande make exécutée dans le répertoire
principal du projet. Il doit être aussi possible de regénérer
complètement le projet par la commande make clean all lancée du même
répertoire.

On prévoira de pouvoir compiler les différents sources avec un drapeau
DEBUG (option -DDEBUG de gcc), permettant un affichage conditionnel
d’informations de déverminage des programmes.

### 3.3  Programmation du micro-contrôleur {#sec11 .subsection}

Les deux principales difficultés pour la programmation d’un dispositif
micro-contrôleur sont décrites ci-après.

-   L’utilisation du capteur GP2Y1010AU0F ne présente pas de difficulté
    particulière, il suffit d’acquérir la valeur analogique.
-   Une difficulté consiste en la réception et l’envoi de paquets UDP. A
    ce niveau le travail est considérablement facilité par la fonction
    d’initialisation présente dans le fichier ethernet.c et les
    fonctions d’utilisation des sockets (en particulier UDP) du fichier
    socket.c.
-   Le message envoyé en utilisant UDP par les capteurs doit comprend un
    nombre magique pour assurer que le message provient d’un capteur, la
    valeur du capteur et enfin le nom du capteur.

### 3.4  Sockets et serveur TCP {#sec12 .subsection}

Il s’agit dans cette étape de réaliser un serveur TCP basique à l’aide
de l’interface de programmation des sockets.

Ecrivez dans le module libcom.c (répertoire Communication), les deux
fonctions suivantes :

-   int initialisationServeur(char \*service); Cette fonction prend en
    paramètre le service à implanter (par exempke le numéro du port sur
    lequel il faut écouter) et retourne la socket de lecture. Il vous
    est demandé d’activer l’option de réutilisation d’adresse sur la
    socket d’écoute (fonction setsockopt).
-   int boucleServeur(int, void (\*)(int)); Cette fonction effectue
    l’écoute sur la socket passée en premier argument et lors d’une
    connexion, exécute la fonction passée en second argument. Cette
    fonction passée en argument doit être une fonction qui prend une
    socket en unique paramètre. Lors d’une connexion de client, la
    fonction boucleServeur lance donc cette fonction avec la socket de
    dialogue en paramètre.

Testez cette bibliothèque en écrivant un programme station.c (répertoire
Station) qui l’utilise et dont la fonction de traitement des connexions
(celle appelée par boucleServeur) effectue juste une écriture de message
dans la socket et clôt la connexion. Pour écrire, et plus tard lire, sur
la connexion utilisez les fonctions classiques comme fgets ou fprintf.
Pour y arriver vous devez transformer le descripteur de socket en une
structure de fichier par la primitive fdopen.

Ce programme peut prendre des arguments : -p \<port\> ou --port \<port\>
pour spécifier un numéro de port différent de celui par défaut (port
80). Pour traiter les arguments, utilisez la fonction getopt\_long (voir
la page de manuel correspondante). Si les arguments sont incorrects, on
doit afficher un message qui précise la syntaxe. Pour plus de clarté,
l’analyse des arguments et l’affichage de la syntaxe seront écrits dans
des fonctions séparées.

Modifiez la fonction de traitement des connexions afin qu’elle ne ferme
la connexion qu’après avoir lu une commande en provenance du client.
Testez votre serveur avec plusieurs commandes nc simultanées. Conclusion
?

### 3.5  Un serveur Web à base de processus légers {#sec13 .subsection}

Pour que votre serveur Web puisse traiter plusieurs connexions
simultanément, vous allez lancer un processus léger (thread) par
connexion. Pour cela, implémentez la fonction publique de libthrd
(répertoire Threads) :

``` {.verbatim}
int lanceThread(void (*)(void *),void *,int);
```

Cette fonction doit avoir comme action de lancer un thread dans le mode
détaché. Ce thread doit exécuter la fonction passée en paramètre. La
dite fonction prenant elle même comme paramètre le second paramètre de
lanceThread dont la taille mémoire est donnée par le troisième
paramètre. Il vous est conseillé d’utiliser une fonction intermédiaire
récupérant un pointeur vers une structure comprenant les deux paramètres
de lanceThread.

Testez votre fonction lanceThread en créant une nouvelle fonction dans
station.c qui appelle votre fonction de traitement des connexions via
lanceThread. Utilisez la nouvelle fonction comme paramètre de
boucleServeur. Pour plus de clarté, ce serait une bonne idée de déplacer
les fonctions de traitement des commandes des clients TCP du fichier
station.c vers le fichier http.c.

Vérifiez que votre serveur est maintenant capable de gérer plusieurs
connexions simultanément (toujours avec l’utilitaire nc).

La fonction lanceThread est utilisée dans plusieurs situations
différentes. Tout d’abord comme expliqué plus haut pour gérer plusieurs
clients Web simultanément ; dans ce cas la donnée passée à la fonction
paramètre est un descripteur de socket de dialogue. La fonction
lanceThread est aussi utilisée pour gérer simultanément les messages UDP
envoyés par les capteurs ; dans ce cas la donnée passée à la fonction
paramètre est le message UDP. Enfin, la fonction lanceThread est aussi
utilisée pour démarrer le processus léger de gestion des messages UDP ;
ici il n’y a pas de donnée à passer à la fonction paramètre (appelez
lanceThread avec NULL en second paramètre et 0 en troisième paramètre).

### 3.6  Diffusion de messages par UDP {#sec14 .subsection}

Dans la description de l’architecture générale il est indiqué que le
processus Unix effectue une diffusion UDP pour découvrir les capteurs
puis sollicite des valeurs par envoi de paquets UDP unicast. Vous allez
donc ajouter dans la bibliothèque Communication les fonctions suivantes
:

-   void serveurMessages(char \*,void (\*)(unsigned char \*,int)); Cette
    fonction prend en paramètre le port sur lequel lancer un serveur UDP
    et la fonction de traitement des messages reçus.
-   int envoiMessage(struct sockaddr \*,int,unsigned char \*,int); Il
    s’agit de la fonction complémentaire, permettant d’envoyer un
    message UDP. Le premier argument donne l’adresse de socket
    destination. Cette adresse pouvant être une adresse de diffusion
    totale. Le second argument est le paquet à envoyer et enfin le
    dernier argument la taille du paquet. N’oubliez pas d’activer
    l’option permettant une diffusion totale (fonction setsockopt).
    Notez que le descripteur de socket utilisé est celui créé, en
    variable globale, par la fonction précédente.

Testez vos fonctions en créant un fichier capteurs.c. Vous y écrirez une
fonction messages permettant de lancer un processus léger réalisant
principalement un appel à la fonction serveurMessages. La fonction de
traitement associée se bornera à afficher le contenu des messages reçus
en considérant qu’il s’agit d’une chaîne de caractères.

Créez un exécutable annexe de test utilisant la fonction d’envoi de
message de façon périodique. Vérifiez avec cet exécutable de test que
votre exécutable principal reçoit bien les diffusions UDP.

### 3.7  Structure de données et fichiers {#sec15 .subsection}

Pour l’affichage des valeurs instantanées des capteurs vous utiliserez
une liste contigüe de structures stockant les adresses des capteurs et
leurs valeurs.

Pour l’affichage des courbes des valeurs, vous utiliserez les données
stockées dans des fichiers binaires. Il est conseillé d’utiliser un
fichier par capteur et un fichier listant le nom de tous les capteurs.

Définissez les structures de données nécessaires dans le fichier entête
station.h.

Déterminez quelles opérations nécessitent l’utilisation de sémaphores.

Implantez dans votre bibliothèque libthrd les deux fonctions publiques :

``` {.verbatim}
void P(int) ;
void V(int) ;
```

Ces fonctions cachent totalement le fait que vous utilisez des verrous
d’exclusion mutelle pour threads POSIX. En particulier, les verrous sont
représentés par une constante.

En implantant dans les divers fichiers, les fonctions nécessaires au
fonctionnement du processus Unix, prenez soin d’y ajouter les poses et
levées de verrous nécessaires.

### 3.8  Le serveur Web {#sec16 .subsection}

Vous pouvez maintenant vous attaquer à l’écriture du code concernant le
serveur Web. Il vous est rappelé qu’il vous a été conseillé d’utiliser
les fichiers suivants :

-   station.c : fonction principale et analyse des arguments ;
-   station.h : fichier des structures de données ;
-   capteurs.c : fonctions de réception de messages UDP, de stockage en     mémoire et d’écriture dans les fichiers binaires ;
-   http.c : fonctions d’analyse des requêtes HTTP.

Pour normaliser le serveur Web, il est demandé que la page d’affichage
des valeurs instantanées soit nommée valeurs.html et que celle
d’affichage des graphes soit nommée graphes.html. Une page principale
doit être prévue pour renvoyer l’utilisateur sur une des deux pages
décrites plus haut.

* * * * *

> *Ce document a été traduit de L^A^T~E~X
> par*[*H**^E^**V**^E^**A*](http://hevea.inria.fr)




# COMPTE-RENDU 


## INTRODUCTION

L’objectif est de concevoir une station de vérification de la qualité environnementale avec des capteurs de particules et des gaz connectés à un réseau TCP/IP et un serveur Web permettant d’afficher ergonomiquement les valeurs des capteurs. 

(cf: https://rex.plil.fr/Enseignement/Systeme/Tutorat.IMA2a4.CapteurPollution/pollution001.html)




## PROGRAMMATION MODULAIRE

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


## PARTIE ARDUINO

Aprés avoir effectué les configurations nécessaires grâce aux bibliothèques fournies, nous avons "pingé" l'arduino pour vérifier que le shield ethernet fonctionne bien et communique avec l'arduino grâce au protocole SPI.
Ainsi, récupère la valeur analogique du capteur sur la broche A0 et dés qu'ilreçoit un message de la part du serveur UDP dans le PC, il récupère son adresse ainsi que le numéro du service pour lui envoyer les données.
Les données en question sont formées de 5 octects dont les 2 de poids faibles sont les valeurs des capteurs en héxadécimal.

A cela, s'ajoute notre PCB que nous avons crée avec le logiciel Fritzing.

Le matériel complet sera mis dans la boite fournie avec l'indication: IMA2A4 PSR5

Le programme est téleversé sur un arduino nano avec 57600 bauds et ttyUSB0 pour l'arduino.




## GESTION DU SERVEUR UDP

Aprés avoir initialisé notre serveur, on se met dans une boucle d'attente de message de client. Ainsi un  broadcast est émis sur le réseau toutes les 5 secondes et dés qu'un message est reçu,une fonction lance un thread pour qu'une socket de dialogue soit ouverte et que les messages soient stockés à la fois dans un fichier sous format JSON et des fichiers textes avec les valeurs exactes des capteurs.
Pour la conversion, nous utilisons les éléments fournis dans la datasheet du capteur gp2y1010au0f . 
Toutes les données sont stockée dans le répertoire HTML.



## GESTION DU SERVEUR TCP

Cette partie concerne plus la gestion des requêtes http envoyées lorsque le client se connecte.En fonction de la requête, le serveur envoie les informations demandées afin que le navigateur web puisse accéder aux données des capteurs en afficher sur une page les valers instantannées et sur une autre l'évolution temporelle.
La programmation du client web s'est faite en html avec du code CSS pour la mise en forme et en couleur et du javascript pour rendre le site dynamique en récupérant les fichiers de mesures et les traitments requis.
Cependant, faute de temps, nous n'arrivons pas à mettre les valeurs les plus récentes des capteurs en début de fichier malgré l'utilisation des fonctions rewind et fssek. De ce, fait le changement des valeurs instantanées ne se fait pas. 

Vu que cette partie est susceptible d'avoir quelques dysfonctionnement lors du développemtn du code, nous avons utilisé la compilation conditionnelle avec la macro #if DEBUG pour afficher sur le terminal l'évolution du code.





## CONCLUSION

En somme, les parties les plus importantes ont été validées et les codes fonctionnent correctement à l'exception de quelques warning qui ne posent aucun problème à l'éxécution.
Dans, la fonction main, le serveur UDP et le serveur TCP sont lancés grâce à un fork pour qu'ils s'éxécutent en parallèle sans avoir à partager un espace mémoire.

Cependant, par manque de temps, nous avons dû faire certains choix et contourner la consigne de base afin d'assurer la fonctionnabilité du projet:

--> C'est ainsi que nous avons mis des valeurs aléatoires entre 0 et 1024 car le capteur ne semble pas fonctionner. Des essais ont été effectués avec l'IDE arduino (plus facilement programmable) mais on obtient toujours 0

--> Notre processus UDP interroge les capteurs tous les 5 en broadcast (même si cela pourrait saturer le réseau) et ces derniers répondent. Ainsi les valeur sont stockés sous format JSON mais aussi dans un fichier texte par capteur. Cependant, c'est le fichier .txt qui sera utilisé par notre navigateur pour afficher les valeurs instantanées et les graphes

--> Il est possible de compiler tout le code à partir du makefile de la racine, cependant nous lançons l'exécution avec [./station] dans le répertoire Station pour les serveurs et dans le répertoire Capteur nous téléversons le programme vers l'arduino avec [sudo make upload].

--> Enfin, nous n'utilisons pas de sémaphore pour la gestion des ressources partagées






