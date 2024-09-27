#include <sys/socket.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#define BUFFER_SIZE 1024
#define ITERATION 10000000

int main(){

    //Initialisation de la graine
    srandom(time(NULL));
    double z = (double) random()/RAND_MAX;
    double x, y = 0;
    char c = '\0';
    //Creation de la socket double
    int sockfd[2];
    int spair = socketpair(AF_UNIX, SOCK_STREAM, 0, sockfd);

    if(spair == -1){
        perror("La creation de la scoketpair à échoué");
        return 1;
    }

    //Pas besoin de sockaddr car on est en lode AF_UNIX

    int n = 0;
    int m = 0;

    while(1){
        if (fork() == 0){
            //Le fils client pi
            close(sockfd[0]);
            while (1)
            {
                n++;
                //Demande x
                write(sockfd[1], &c, sizeof(char));
                int nbLus = read(sockfd[1], &x, sizeof(double));
                if (nbLus == 0){ //Connexion fermée
                    break;
                }
                if (nbLus == -1){
                    perror("Erreur lors du readA");
                    return 1;
                }
                //printf("x = %g \n", x);
                //Demande y
                write(sockfd[1], &c, sizeof(char));
                nbLus = read(sockfd[1], &y, sizeof(double));
                if (nbLus == 0){ //Connexion fermé
                    break;
                }
                if (nbLus == -1){
                    perror("Erreur lors du readB");
                    return 1;
                }
                //printf("y = %g \n", y);
                //printf("puissance = %g \n", pow(x, 2) + pow(y,2));
                if(pow(x, 2) + pow(y,2) <= 1){
                    m++;
                }
                if(n % 100000 == 0){
                    double pi = (4.0*m)/n;
                    printf("Valeur de pi : %g \n",pi);
                }
                if(n >= ITERATION){
                    double pi = (4.0*m)/n;
                    printf("Valeur finale de pi : %g \n",pi);
                    break;
                }
            }
            shutdown(sockfd[1], SHUT_RDWR);
            close(sockfd[1]);
            exit(0);
        }
        //Le père, serveur tirage
        close(sockfd[1]);

        z = (double) random()/RAND_MAX;
        char c;
        int nbLus = read(sockfd[0], &c, sizeof(char));  
        if (nbLus == 0){ //Connexion fermée
            break;
        }
        if (nbLus == -1){
            perror("Erreur lors du readC");
            return 1;
        }
        write(sockfd[0], &z, sizeof(double));
    }
    shutdown(sockfd[1], SHUT_RDWR);
    close(sockfd[0]);

    return 0;
}