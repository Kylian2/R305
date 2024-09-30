/* CLIENT. Donner l'adresse IP et un pseudo en paramètre */
/* exemple "client 127.0.0.1 dr.ced", lancer en dernier. */

/* snprintf(message_a_envoyer, BUFFERSIZE, "%s >", argv[2])*/

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

#define PORT_SERVEUR 5015      /* Numéro de port pour le serveur */
#define BUFFER_SIZE  1024      /* Taille maximum des messages */

int main(int argc, char *argv[]) {

  char message[BUFFER_SIZE] = {0};

  /* 1. On crée la socket du client. */

  int sclient = socket(AF_INET, SOCK_STREAM, 0);
  if(sclient ==-1){
    perror("Erreur de socket sclient");
    return -1;
  }

  struct sockaddr_in saddr = {0};

  /* 2. On prépare l'adresse du serveur. */

  saddr.sin_family = AF_INET;
  saddr.sin_addr.s_addr = inet_addr(argv[1]);
  saddr.sin_port = htons(PORT_SERVEUR);

  /* 3. On demande une connexion au serveur, tant qu'on y arrive pas. */

  printf("Connexion...");
  while(connect(sclient, (struct sockaddr *) &saddr, sizeof(saddr)) == -1){printf(".");};

  if(fork() == 0){
    while(1){
      int nbLus = read(sclient, message, BUFFER_SIZE-1);
      if (nbLus == -1){
        perror("Erreur lors du read fils");
      }
      if (nbLus == 0){
        break;
      }

      //Ecriture sur la sortie
      write(1,message, BUFFER_SIZE);

    }
    shutdown(sclient, SHUT_RDWR);
    close(sclient);
    exit(0);
  }

  while(1){
    int nbLus = read(0, message, BUFFER_SIZE-1); 
    if (nbLus == 0){ //Connexion fermé
        break;
    }
    if (nbLus == -1){
        perror("Erreur lors du readB");
        return 1;
    }
    //printf("%s > ", argv[2]);
    snprintf(message, BUFFER_SIZE-1, "%s >", argv[2]);
    write(sclient, &message, BUFFER_SIZE-1);
  }

  shutdown(sclient, SHUT_RDWR);
  close(sclient);

  /* 4. On communique. */

  /* Le fils lit les données reçus depuis le serveur et les affiches sur la sortie standard*/

  /* Le père lit les données de l'entrée standard et les envoie au serveur*/

  /* 5. On termine et libère les ressources. */

  return 0;
}
