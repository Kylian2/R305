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

int main(int argc, char *argv[]) {

  /* 1. On déroute les signaux */

  signal(SIGCHLD, SIG_IGN);

  /* 2. On crée la socket d'écoute. */

  int secoute = socket(AF_INET, SOCK_STREAM, 0);

  /* 3. On prépare l'adresse du serveur. */

  struct sockaddr_in saddr = {0};
  saddr.sin_family = AF_INET;
  saddr.sin_port = htons(PORT_SERVEUR);
  saddr.sin_addr.s_addr = inet_addr("127.0.0.1");

  /* 4. On attache la socket a l'adresse du serveur. */

  if(bind(secoute, (struct sockaddr *) &saddr, sizeof(saddr)) == -1){
    perror("Erreur de bind");
    return 1;
  }

  if(listen(secoute, MAX_CLIENTS) == -1){
    perror("Erreur de listen");
    return 1;
  }

  /* 5. Enregistrement auprès du système. */

  fd_set ensemble, temp;
  FD_ZERO(&ensemble);
  FD_SET(secoute, &ensemble);

  /* Variables de service*/

  int sservice;
  struct sockaddr_in caddr = {0};
  unsigned int caddrlen;
  int nbLus;

  char message[BUFFER_SIZE] = {0};
  int max = secoute;

  while (1) {
    printf("Serveur en attente de nouveaux clients ou messages.\n");

    temp = ensemble;
    if(select(max+1, &temp, NULL, NULL, NULL) == -1){
      perror("Signal reçu");
    }

    /* 6. Si on a reçu une demande sur la socket d'écoute... */

    if(FD_ISSET(secoute, &temp)){
      printf("Un client se connecte.\n");
      caddrlen = sizeof(caddr);
      sservice = accept(secoute, (struct sockaddr *) &caddr, &caddrlen);
      if (sservice > max) {
          max = sservice;
      }
      FD_SET(sservice, &ensemble);
    }

    for (int fd = 0; fd < max+1; fd++){

      if(!FD_ISSET(fd, &temp) || fd == secoute){
        continue;
      }
      
      nbLus = read(fd, message, BUFFER_SIZE-1);
      if(nbLus == -1){
        perror("Erreur de read");
        FD_CLR(fd, &ensemble);
        shutdown(fd, SHUT_RDWR);
        close(fd);
        continue;
      }
      if(nbLus == 0){
        FD_CLR(fd, &ensemble);
        shutdown(fd, SHUT_RDWR);
        close(fd);
        continue;
      }
      if(nbLus > 0){
        write(1, "Message recu : ", strlen("Message recu : "));
      }

    /* 7. Si on a reçu des données sur une socket de service... */
      for(int fdi = 0; fdi <max+1; fdi++){
        if(fdi == secoute || fdi == 0 || fdi == 2){
          continue;
        }
        write(fdi, message, nbLus);
      }

    }

  }

  /* 8. On termine et libère les ressources. */

  shutdown(secoute, SHUT_RDWR);
  close(secoute);

  return 0;
}
