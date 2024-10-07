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

int main (int argc, char *argv[]){
    int nbLus;
    char message[BUFFER_SIZE] = {0};

    int sclient = socket(AF_INET, SOCK_STREAM, 0);
    if(sclient == -1){
        perror("Erreur de creation de socket");
        return 1;
    }

    struct sockaddr_in saddr = {0};

    if(inet_addr(argv[1]) == INADDR_NONE){
        printf("Adresse ip invalide \n");
    }

    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(PORT);
    saddr.sin_addr.s_addr = inet_addr(argv[1]);

    while(connect(sclient, (struct sockaddr *) &saddr, sizeof(saddr)) == -1){
        //perror("Erreur");
    }
    printf("Connecté !\n");
    write(sclient, argv[2], strlen(argv[2]+1));

    while(1){
        nbLus = read(sclient, message, BUFFER_SIZE-1);
        if(nbLus == -1){
            perror("Erreur de read");
        }
        if(nbLus == 0){
            break;
        }
        printf("Message reçu : %s \n", message);
    }
    shutdown(sclient, SHUT_RDWR);
    close(sclient);
}