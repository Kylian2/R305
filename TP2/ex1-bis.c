// Dans ce schéma, qui est le client et qui est le serveur ?
// Le processus pi est le client (fils)
// Le processus tirage est le serveur (père)

#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

/* Taille maximale des messages */
#define BUFFER_SIZE 1024
#define MAX_ITERATIONS 10000000

int main (int argc, char *argv[]) {

    srandom(time(NULL));

    int n = 0, m = 0;
    int nbLus; //Nombre d'octets lus
    int nbEnvoyes; //Nombre d'octets envoyés
    char message[BUFFER_SIZE] = {0};     // Message reçu ou à envoyer
    int sockfd[2];
    char c = '\0';
    double x, y;

    int sp = socketpair(AF_UNIX, SOCK_STREAM, 0, sockfd /* les sockets créées sont placées ici */ );
    if(sp == -1){
        perror("socketpair() failure");
		return 1;
    }
    int cpid = fork();
    if (cpid == -1){
			perror("fork() failure");
			exit(1);
	}
    if(cpid > 0){ /* Processus père */
        close(sockfd[1]);
        while(1){
            nbLus = read(sockfd[0], message, BUFFER_SIZE);
            if (nbLus == -1){
                perror("erreur de read()");
                return 1;
            }
            if (nbLus == 0) break;
            double z = ((double)random()/RAND_MAX);
            nbEnvoyes = write(sockfd[0], &z, sizeof(z));
            if (nbEnvoyes == -1){
                perror("erreur de write()");
                return 1;
            }
        }
        // Terminer et libérer les ressources
        shutdown(sockfd[0], SHUT_RDWR);
        close(sp);
        return 0;
    }
    /* Processus fils */
    close(sockfd[0]);
    while(1){
        n++;
        // Envoyer '\0' pour récupérer x
        nbEnvoyes = write(sockfd[1], &c, sizeof(c));
        if (nbEnvoyes == -1){
            perror("erreur de write()");
            exit(1);
        }
        // Récupérer x
        nbLus = read(sockfd[1], &x, sizeof(x));
        if (nbLus == -1){
            perror("erreur de read()");
            return 1;
        }
        if (nbLus == 0) break;
        // Envoyer '\0' pour récupérer y
        nbEnvoyes = write(sockfd[1], &c, sizeof(c));
        if (nbEnvoyes == -1){
            perror("erreur de write()");
            exit(1);
        }
        // Récupérer y
        nbLus = read(sockfd[1], &y, sizeof(y));
        if (nbLus == -1){
            perror("erreur de read()");
            return 1;
        }
        if (nbLus == 0) break;
        // Si x^2 + y^2 <= 1 incrémenter m
        if (x*x + y*y <= 1){
            m++;
        }
        // Afficher la valeur de pi régulièrement
        if (n <= MAX_ITERATIONS && n % 100000 == 0){
            printf("Estimation de pi : %g\n", (double) (4*m)/n);
        }
    }
    // Terminer et libérer les ressources
    shutdown(sockfd[0], SHUT_RDWR);
    close(sp);

    return 0;
}