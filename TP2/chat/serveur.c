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

  signal(SIGCHL, )

  /* 2. On crée la socket d'écoute. */

  /* 3. On prépare l'adresse du serveur. */

  /* 4. On attache la socket a l'adresse du serveur. */

  /* 5. Enregistrement auprès du système. */

  while (1) {
    printf("Serveur en attente de nouveaux clients ou messages.\n");

    /* 6. Si on a reçu une demande sur la socket d'écoute... */

    /* 7. Si on a reçu des données sur une socket de service... */

  }

  /* 8. On termine et libère les ressources. */

  return 0;
}
