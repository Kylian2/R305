#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


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
        if (tab[i] == 0){
            return 1;
        }
        i++;
    }
    return 0;
}

int main(){

    //Initialisation du tableau
    const int TABSIZE = 1000;
    unsigned char arr[TABSIZE]; //Entier de 0 à 255
    srandom(time(NULL));
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

    pid_t child = fork(); //Création du processus fils

    int rep_pere;
    int rep_fils;
    int status;

    if(child == -1){
        perror("fork() error");
        exit(1);
    }

    if(child > 0){ 
        //Exécuté par le père
        rep_pere = fouiller(arr, 0, TABSIZE/2);
        wait(&status);
    }else{
        //Exécuté par le fils
        exit(fouiller(arr, TABSIZE/2, TABSIZE));
    }

    rep_fils = WEXITSTATUS(status);
    if (rep_pere || rep_fils){
        printf("Needle found ! \n");
    }else {
        printf("No needle found... \n");
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