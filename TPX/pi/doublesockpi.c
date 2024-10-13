#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#define BUFFER_SIZE 1024
#define MAX_TIRAGE 1000000

int main(){

    int nbLus = 0;
    double x;
    double y;
    double pi;
    int n = 0;
    int m = 0; //le nombre tirage tq x^2+y^2 <= 1
    char c = '\0';
    /* 1. Création de la socketpair */

    int sockfd[2]; //Le tableau contenant des descripteurs de la socket pair
    int spair = socketpair(AF_UNIX, SOCK_STREAM, 0, sockfd);
    if(spair == -1){
        perror("Erreur lors de la creation de la socket");
        return 1;
    }
    /* Le fils */
    if(fork() == 0){
        //shutdown(sockfd[0], SHUT_RDWR);
        close(sockfd[0]);
        while (1){
            n++;
            write(sockfd[1], &c, sizeof(char));
            nbLus = read(sockfd[1], &x, sizeof(double));
            if(nbLus == -1){
                perror("Erreur de read X");
                return 1;
            }
            if(nbLus == 0){
                break;
            }

            write(sockfd[1], &c, sizeof(char));
            nbLus = read(sockfd[1], &y, sizeof(double));

            if(nbLus == -1){
                perror("Erreur de read Y");
                return 1;
            }
            if(nbLus == 0){
                break;
            }

            if(x*x + y*y <= 1){
                m++;
            }

            if(n%10000 == 0){
                pi = 4.0*m/n;
                printf("Valeur de pi %g \n", pi);
            }

            if(n >= MAX_TIRAGE){
                pi = 4.0*m/n;
                printf("Valeur finale de pi %g \n", pi);
                break;
            }
        }
        shutdown(sockfd[1], SHUT_RDWR);
        close(sockfd[1]);
        exit(0);
    }
    /* Le père - effectue les tirages*/
    //shutdown(sockfd[1], SHUT_RDWR);
    close(sockfd[1]);

    srand(time(NULL));
    double z = (double) random()/RAND_MAX;

    while(1){
        nbLus = read(sockfd[0], &c, sizeof(char));
        if(nbLus == -1){
            perror("Erreur de read demande");
            return 1;
        }
        if(nbLus == 0){
            break;
        }
        z = (double) random()/RAND_MAX;
        write(sockfd[0], &z, sizeof(double));
    }

    shutdown(sockfd[0], SHUT_RDWR);
    close(sockfd[0]);
}   