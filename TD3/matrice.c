#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NB_LIGNES 10
#define NB_COLONNES 10

pthread_t tid[NB_LIGNES];
int argi[NB_LIGNES];

int A[NB_LIGNES][NB_COLONNES];  
int X[NB_LIGNES];
int Y[NB_LIGNES] = {0};

void display(int matrice[NB_LIGNES][NB_COLONNES]){
    for (int i = 0; i < NB_LIGNES; i++){
        printf("|");
        for (int j = 0; j < NB_COLONNES; j++){
            printf("%d", matrice[i][j]);
            if(j != NB_COLONNES-1){
                printf(", ");
            }
        }
        printf("|\n");
    }
}

void displayColonne(int matrice[NB_LIGNES]){
    for (int i = 0; i < NB_LIGNES; i++){
        printf("|");
        printf("%d", matrice[i]);
        printf("|\n");
    }
}

void *routine(void *x){
    int ligne = *((int *) x);
    int sum = 0;
    for (int i = 0; i < NB_COLONNES; i++){
        sum += A[ligne][i]*X[i];
    }
    Y[ligne] = sum;
    return NULL;
}

int main(){

    for (int i = 0; i < NB_LIGNES; i++){
        for (int j = 0; j < NB_COLONNES; j++){
            if(i == NB_COLONNES-j-1){
                A[i][j] = 1;
            }else{
                A[i][j] = 0;
            }
        }
    }

    for (int i = 0; i < NB_LIGNES; i++){
        X[i] = i;
    }

    display(A);
    displayColonne(X);
    int i;
    for (i = 0; i < NB_LIGNES; i++){
        argi[i] = i;
        if(pthread_create(&tid[i], NULL, routine, (void *) &argi[i]) != 0){
            perror("Erreur de création de thread");
        }
    }
    for (i = 0; i < NB_LIGNES; i++){
        pthread_join(tid[i], NULL);
    }
    printf("Résultat : \n");
    displayColonne(Y);
    return 0;
}
