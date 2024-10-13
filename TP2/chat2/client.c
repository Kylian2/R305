#include <stdio.h>                  /* fichiers d'en-tête classiques */
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

#include <sys/socket.h>             /* fichiers d'en-tête "réseau" */
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORT_SERVEUR 5015           /* Numéro de port pour le serveur */
#define BUFFER_SIZE  1024           /* Taille maximum des messages */

int main(int argc, char *argv[]){

    int nbLus; 
    char message[BUFFER_SIZE];
    
    char pseudo[strlen(argv[2])+1];
    strcpy(pseudo, argv[2]); 

    char messageToSend[BUFFER_SIZE];


    /* 1. Création de la socket */

    int sclient = socket(AF_INET, SOCK_STREAM, 0);
    if(sclient == -1){
        perror("Erreur de création de la socket");
        return 1;
    }

    /* 2. Création de la structure d'adresse */

    struct sockaddr_in saddr = {0};
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(PORT_SERVEUR);
    saddr.sin_addr.s_addr = inet_addr(argv[1]);

    /* 3. Connexion au serveur */

    while(connect(sclient, (struct sockaddr *) &saddr, sizeof(saddr)) == -1);

    /* Lecture des messages envoyés par le serveur */
    if(fork() == 0){
        while(1){
            int nbLus = read(sclient, message, BUFFER_SIZE-1);
            if (nbLus == -1){
                perror("Erreur lors du read depuis le serveur");
            }
            if (nbLus == 0){
                break;
            }

            //Ecriture sur la sortie
            write(1,"\n", strlen("\n"));
            write(1,message, nbLus);
            write(1,"> ", strlen("> "));
        }
        shutdown(sclient, SHUT_RDWR);
        close(sclient);
        exit(0);
    }

    /* Envoie de message*/
    while(1){
        write(1, "> ", strlen("> "));
        nbLus = read(0, message, BUFFER_SIZE);

        if(nbLus == -1){
            perror("Erreur lors du read");
            return 1;
        }

        if(nbLus == 0){
            break;
        }
        
        message[nbLus] = '\0';
        sprintf(messageToSend, "%s : %s", pseudo, message);

        write(sclient, messageToSend, strlen(messageToSend));

    }
    close(sclient);
    shutdown(sclient, SHUT_RDWR);
    return 0;
}