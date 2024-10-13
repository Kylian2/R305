/* SERVEUR. Lancer ce programme en premier (pas d'arguments). */

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
#define MAX_CLIENTS   128           /* Nombre maximum de clients */
#define BUFFER_SIZE  1024           /* Taille maximum des messages */
#define ADRESSE "127.0.0.1"

int main(int argc, char *argv[]) {

    signal(SIGCHLD, SIG_IGN);

    int nbLus;
    char message[BUFFER_SIZE];

    /* 1. Création de la socket d'écoute */

    int secoute = socket(AF_INET, SOCK_STREAM, 0);
    if(secoute == -1){
        perror("Erreur de création de la socket");
    }

    int sservice;

    /* 2. Création de la structure d'adresse */

    struct sockaddr_in saddr = {0};
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(PORT_SERVEUR);
    saddr.sin_addr.s_addr = inet_addr(ADRESSE);

    struct sockaddr_in caddr = {0};
    unsigned int caddrlen;
    
    /* 3. On attache l'adresse et on ouvre l'écoute */

    if(bind(secoute, (struct sockaddr *) &saddr, sizeof(saddr)) == -1){
        perror("Erreur lors du bind");
        return 1;
    }

    if(listen(secoute, MAX_CLIENTS) == -1){
        perror("Erreur lors du listen");
        return 1;
    }

    /* 4. Création du set de descripteur */

    fd_set ensemble, temp;
    FD_ZERO(&ensemble);
    FD_SET(secoute, &ensemble);

    int max = secoute; //le descripteur max
    printf("Serveur en attente de nouveaux clients ou messages. \n");
    while(1){

        temp = ensemble;
        if(select(max+1, &temp, NULL, NULL, NULL) == -1){
            printf("Signal reçu \n");
        }

        //* 5. Vérification de l'arrivée de client */

        if(FD_ISSET(secoute, &temp)){
            caddrlen = sizeof(caddr);
            sservice = accept(secoute, (struct sockaddr *) &caddr, &caddrlen);
            if(sservice > max){
                max = sservice;
            }
            FD_SET(sservice, &ensemble);
            printf("Un nouveau client est connecte \n");
        }

        /* 6. Gestion des clients */
        for (int fd = 0; fd < max+1; fd++){

            /* On ne lit pas sur la socket d'écoute, si la socket fd est inactive, il n'y a rien à lire donc on saute*/
            if(!FD_ISSET(fd, &temp) || fd == secoute){
                continue;
            }
      
            nbLus = read(fd, message, BUFFER_SIZE-1);

            if(nbLus == -1){
                perror("Erreur de read");
                FD_CLR(fd, &ensemble); //Retire le descripteur du set
                shutdown(fd, SHUT_RDWR);
                close(fd);
                continue;
            }
            if(nbLus == 0){
                printf("Déconnexion d'un client \n");
                FD_CLR(fd, &ensemble); //Retire le descripteur du set
                shutdown(fd, SHUT_RDWR);
                close(fd);
                continue;
            }

            /* Affichage du message sur la sortie standard */
            if(nbLus > 0){
                write(1, "Message reçu de ", strlen("Message reçu de "));
                write(1, message, nbLus);
            }
            
            /* Affichage du message reçu chez tout les clients*/
            for (int fdi = 3; fdi <max+1; fdi++){
                if(fdi == fd){
                    continue;
                }
                write(fdi, message, nbLus);
            }
        }
    }
    shutdown(secoute, SHUT_RDWR);
    close(secoute);
    return 0;

}