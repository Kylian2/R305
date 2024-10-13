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
#define ADRESSE "127.0.0.1"
#define BUFFER_SIZE 1024

int main(){

    /* Déclaration des variables nécessaires */

    int pile[BUFFER_SIZE];
    int compteur = 0;
    char recu[BUFFER_SIZE];
    int nbLus;
    char c = '\0';

    /* 1. Création des sokets */

    int secoute = socket(AF_INET, SOCK_STREAM, 0);
    int sservice;
    if(secoute == -1){
        perror("Erreur lors de la création de la socket secoute");
    }

    /* 2. Création des structures d'adresse */

    struct sockaddr_in saddr = {0};
    struct sockaddr_in caddr = {0};
    unsigned int caddrlen;

    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(PORT);
    saddr.sin_addr.s_addr = inet_addr(ADRESSE);

    /* 3. Liaison de la socket */

    if(bind(secoute, (struct sockaddr *) &saddr, sizeof(saddr)) == -1){
        perror("Erreur de liaison");
        return 1;
    }

    /* 4. Ouverture de l'écoute */

    if(listen(secoute, 5) == -1){
        perror("Erreur d'écoute");
    }

    while(1){
        /* 5. Acceptation */
        caddrlen = sizeof(caddr);
        sservice = accept(secoute, (struct sockaddr *) &caddr, &caddrlen);
        if(sservice == -1){
            perror("Erreur lors de la creation de la socket sservice");
        }
        /* Communication */
        printf("Un client est connecté \n");
        while(1){
            
            nbLus = read(sservice, recu, BUFFER_SIZE-1);
            if(nbLus == -1){
                perror("Erreur de read");
                return 1;
            }
            if(nbLus == 0){
                break;
            }
            write(1, recu, nbLus);

            recu[nbLus] = '\0';
            int isNumber = 1;
            if(nbLus == 2 && (recu[0] == '+' || recu[0] == '-') ){
                if(!(compteur > 1)){
                    printf("Pas assez de nombre dans la pile\n");
                    write(sservice, &c, sizeof(char));
                    continue;
                }
                int temp = compteur;
                if(recu[0] == '+'){
                    for (int i = temp-1; i > 0; i--){
                        pile[i-1] = pile[i] + pile[i-1];
                        compteur--;
                    }
                }
                if(recu[0] == '-'){
                    for (int i = temp-1; i >= 1; i--){
                        pile[i-1] = pile[i-1] - pile[i];
                        compteur--;
                    }
                }
                //Affichage du résutlat, on le convertis en tableau de caractère pour qu'il soit correctement interprété par le write
                char chaine[50];
                write(1,chaine, strlen(chaine));
                write(sservice, chaine, strlen(chaine));
                continue;
            }else{
                int i = 0;
                while(isNumber == 1 && i < nbLus-1){
                    if(!('0' <= recu[i] && recu[i] <= '9')){
                        isNumber = 0;
                    }
                    i++;
                }
            }

            if(!(isNumber == 1)){
                write(sservice, "Veuillez entrer un opérateur ou un nombre\n", strlen("Veuillez entrer un opérateur ou un nombre\n"));
                continue;
            }

            pile[compteur] = atoi(recu);
            compteur++;
            write(sservice, &c, sizeof(char));
            
        }
        shutdown(sservice, SHUT_RDWR);
        close(sservice);
    }
    close(secoute);
}