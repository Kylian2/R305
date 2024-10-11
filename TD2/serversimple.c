#include <stdio.h>             /* fichiers d'en-tête classiques */
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>

#include <sys/socket.h>        /* fichiers d'en-tête "réseau" */
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORT 5015
#define BUFFER_SIZE 1024
#define ADRESSE "127.0.0.1"

int main (int argc, char *argv[]){
    int nbLus, sservice;
    unsigned int caddrlen;
    char message[BUFFER_SIZE] = {0};

    int secoute = socket(AF_INET, SOCK_STREAM, 0);
    if(secoute == -1){
        perror("Erreur de creation de socket");
    }

    struct sockaddr_in saddr, caddr = {0};
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(PORT);
    saddr.sin_addr.s_addr = inet_addr(ADRESSE);

    if (bind(secoute, (struct sockaddr *) &saddr, sizeof(saddr)) == -1){
        perror("Erreur de bind");
    }

    if(listen(secoute, 5) == -1){
        perror("Erreur de listen");
    }

    while(1){
        caddrlen = sizeof(caddr);
        sservice = accept(secoute, (struct sockaddr *) &caddr, &caddrlen);
        if(sservice == -1){
            perror("Erreur lors de la creation de socket de service");
        }
        nbLus = read(sservice, message, BUFFER_SIZE-1);
        if(nbLus == -1){
            perror("Erreur de read");
        }
        if(nbLus == 0){
            break;
        }
        message[nbLus] = '\0';
        write(1, message, strlen(message));
        write(sservice, message, strlen(message)+1);
    }
    write(1, "Connexion terminée \n", strlen("Connexion terminée \n"));
    shutdown(secoute, SHUT_RDWR);
    close(secoute);
}