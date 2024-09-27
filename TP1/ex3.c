#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <signal.h>
#define TABSIZE 1000

volatile pid_t parentPID;
void affiche(unsigned char tab[], int size){
    printf("Affichage du tableau : ");
    for (int i = 0; i < size; i++){
        printf("%d ", tab[i]);
    }
    printf("\n");
}

int fouiller(unsigned char tab[], int depart, int fin){
    int i = depart;
    while( i < fin){
        putc('.', stdout); fflush(stdout); //Affiche un point (fflush permet de recharger la sortie), 
                                         //nous verrons s'afficher un point à chaque parcours de la boucle
        if (tab[i] == 0){
            return 1;
        }
        i++;
    }
    return 0;
}

void handleSIGTERM(int sig){
    if(getppid() == parentPID){
        printf("PID %d s'est arrêté sous ordre du père \n", getpid());
        exit(0);
    }
}

int main(int argc, char *argv[]){

    signal(SIGTERM, handleSIGTERM);
    parentPID = getpid();

    //Initialisation du tableau
    unsigned char arr[TABSIZE]; //Entier de 0 à 255
    srandom(time(NULL));

    int N = 1; //correspond au nombre de processus allant fouiller le tableau
    if (argc > 1) N = atoi(argv[1]); 
    if (N < 1 || N > 100) N = 1;

    printf("%d will share the task.\n", N);
    // entasser du foin
    for (int i = 0; i < TABSIZE; i++){
        arr[i] = (unsigned char) (random() % 255) + 1;
    }
    int index;
    // cacher l'aiguille
    printf("Enter a number between 0 and %d: ", TABSIZE-1);
    scanf(" %d", &index);
    if (index >= 0 && index < TABSIZE){
        arr[index] = 0;
    }else{
        //Vérification de l'entrée
        while (!(index >= 0 && index < TABSIZE)){
            printf("You must a number between 0 and %d: ", TABSIZE-1);
            scanf(" %d", &index);
        }
        arr[index] = 0;
    }

    //affiche(arr, TABSIZE);

    int rep_fils[N];
    int status;

    printf("Parent : %d \n", getpid());

    pid_t child = getpid();
    int i = 0;
    while (i < N && child > 0){
        child = fork();
        if(child == -1){
            perror("fork() error");
            exit(1);
        }
        if(child == 0){
            //Exécuté par le fils
            printf("Child : %d \n", getpid());
            exit(fouiller(arr, (TABSIZE/N)*i, (TABSIZE/N)*i+TABSIZE/N));
        }
        i += 1;
    }

    //Le père attends tout les enfants
    for (i = 0; i < N; i++){
        wait(&status); 
        rep_fils[i] = WEXITSTATUS(status);
        if (rep_fils[i] == 1){
            kill(0, SIGTERM);
        }
    }

    i = 0;
    bool found = false;
    while (!found && i < N){
        if (rep_fils[i]){
            printf("Needle found ! \n");
            found = true;
        }
        i += 1;
    }
    if(!found){
        printf("No needle found ! \n");
    }

    return 0;
}

//L'appel de kill(0, SIGTERM) tue tout les processus du groupe de l'appellant, y compris l'appellant lui-même, le programme est donc stoppé. 