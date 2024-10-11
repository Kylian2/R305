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

#include <time.h> /* Pour la fonction shakeMessage*/

void shakeMessage(char* message, int nbCaractères){
    srand(time(NULL));
    for (int i = 0; i < nbCaractères; i++){
        int pos1 = rand() % nbCaractères;
        int pos2 = rand() % nbCaractères;

        char temp = message[pos1];
        message[pos1] = message[pos2];
        message[pos2] = temp;

    }
}

int main(int argc, char *argv[]){

    int nbLus; 

    char message[BUFFER_SIZE];

    int secoute = socket(AF_INET, SOCK_STREAM, 0);
    if (secoute == -1){
        perror("Erreur de création de socket");
        return 1;
    }
    int sservice;

    struct sockaddr_in saddr = {0};
    struct sockaddr_in caddr = {0};
    unsigned int caddrlen;

    saddr.sin_family = AF_INET;
    saddr.sin_port = PORT;
    saddr.sin_addr.s_addr = inet_addr(argv[1]);

    if(bind(secoute, (struct sockaddr *) &saddr, sizeof(saddr))){
        perror("Erreur lord du bind");
        return 1;
    }

    if(listen(secoute, 5) == -1){
        perror("Erreur lors du listen");
    }

    while(1){
        caddrlen = sizeof(caddr);
        sservice = accept(secoute, (struct sockaddr *) &caddr, &caddrlen);
        if(sservice == -1){
            perror("Erreur lors de la création de la socket sservice");
        }
        while(1){
            
            nbLus = read(sservice, message, BUFFER_SIZE-1);
            if(nbLus == -1){
                perror("Erreur lors du read");
                return 1;
            }
            if(nbLus == 0){
                break;
            }

            message[nbLus] = '\0';  

            shakeMessage(message, nbLus-1);
            write(1, message, nbLus);
            write(sservice, message, nbLus);

        }
        shutdown(sservice, SHUT_RDWR);
        close(sservice);
    }
    shutdown(secoute, SHUT_RDWR);
    close(secoute);
    return 0;
}