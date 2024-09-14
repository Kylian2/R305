#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>

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
        putc('.', stdout); fflush(stdout); sleep(1); //Affiche un point toutes les secondes
        if (tab[i] == 0){
            //printf("Child process: %d has found %d at %d\n", getpid(), tab[i], i);
            return 1;
        }
        i++;
    }
    return 0;
}

int main(int argc, char *argv[]){

    //Initialisation du tableau
    const int TABSIZE = 1000;
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

    pid_t child = getpid();
    int i = 0;
    while (i < N && child > 0){
        child = fork();
        if(child == -1){
            perror("fork() error");
            exit(1);
        }
        if(child > 0){ 
            //Exécuté par le père
            
            
        }else{
            //Exécuté par le fils
            //printf("Child process: %d \n", getpid());
            exit(fouiller(arr, (TABSIZE/N)*i, (TABSIZE/N)*i+TABSIZE/N));
        }
        i += 1;
    }

    //Le père attends tout les enfants
    for (i = 0; i < N; i++){
        wait(&status); 
        rep_fils[i] = WEXITSTATUS(status);
    }

    i = 0;
    bool found = false;
    while (!found){
        if (rep_fils[i]){
            printf("Needle found by %d ! \n", i);
            found = true;
        }
        i += 1;
    }
    if(!found){
        printf("No needle found ! \n");
    }
    
    
    //Q1 : srandom(time(NULL)) initalise une seed pour la fonction random() à partir de la date actuelle
    //Q2 : Il ne peut pas y avoir de 0 à la fin de l'entassement du foin car on ajoute toujours 1 en plus au nombre généré
    //Q3 : Pour passer une information du fils au père, on peut se servir du status renvoyé par le fils lors de sa mort,
    //     le père pourra le récupérer avec WEXITSTATUS

    //Le programme peut etre tester avec : 
    //0 -> premier indice (cherché par le père)
    //999 -> dernier indice (cherché par le fils)
    //500 -> TABSIZE/2 (moment de la séparation entre le père et le fils)

    return 0;
}