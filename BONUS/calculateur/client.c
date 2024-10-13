#include <stdio.h>             /* fichiers d'en-tête classiques */
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>

#include <sys/socket.h>        /* fichiers d'en-tête "réseau" */
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORT 5015
#define BUFFER_SIZE 1024

int main(int argc, char *argv[]){
    int nbLus;
    char message[BUFFER_SIZE];

    /* 1. Création de la socket */
    int sclient = socket(AF_INET, SOCK_STREAM, 0);
    if(sclient == -1){
        perror("Erreur lors de la création de la socket");
        return 1;
    }

    /* Création de la structure d'adresse du serveur */
    struct sockaddr_in saddr = {0};
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(PORT);
    saddr.sin_addr.s_addr = inet_addr(argv[1]);

    /* Connexion au serveur */

    while(connect(sclient, (struct sockaddr *) &saddr, sizeof(saddr)) == -1);
    printf("CALCULETTE, entrez un nombre ou un opérateur (+, -)\n");

    while(1){
        write(1, "> ", strlen("> "));
        nbLus = read(0, message, BUFFER_SIZE-1);
        message[nbLus] = '\0';
        if(nbLus == 0){
            break;
        }
        if(nbLus == -1){
            perror("Erreur lors du read sur l'entrée std");
        }

        write(sclient, message, nbLus);

        nbLus = read(sclient, message, BUFFER_SIZE-1);
        if(nbLus == 0){
            break;
        }
        if(nbLus == -1){
            perror("Erreur lors du read sur l'entrée std");
        }
        message[nbLus] = '\0';
        if(!(message[0] == '\0')){
            write(1, message, nbLus);
        }
    }
    shutdown(sclient, SHUT_RDWR);
    close(sclient);
}