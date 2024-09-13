#include <stdio.h>

void echange1(float a, float b) {
    float temp = a;
    a = b;
    b = temp;
}

void echange2(float *a, float *b) {
    float temp = *a;
    *a = *b;
    *b = temp;
}

int main() {

    printf("----- Essaie des printf -----\n");

    printf("hello, world\n");
    //Avec string
    char prenom[] = "Kylian";
    printf("hello %s ! \n", prenom);

    //Avec int 
    int age = 19;
    printf("hello je m'appelle %s, j'ai %d ans.\n", prenom, age);

    //Trop de code pour pas assez de paramètres 
    printf("hello je m'appelle %s, j'ai %d ans.\n");
    //Il affiche un warning et le résultat est imprévisible

    //Pas assez de code pour le nombre de paramètre 
    printf("hello je m'appelle, j'ai ans.\n", prenom, age);
    //Il mets des warnings et affiche la chaine brute.

    printf("----- Essaie des pointeurs -----\n");

    double valeur = 10.0;
    double *pv = &valeur;
    int nombre = 1, *pn;
    pn = &nombre;
    valeur = *pv + *pn;
    printf(" valeur = %f\n", valeur);
    printf("&valeur = %p\n", &valeur);

    printf("----- Passage de paramètre -----\n");
    float pi = 2.71828, e = 3.14159;
    printf("Avant echange : pi = %.2f, e = %.2f.\n",pi,e);
    echange2(&pi,&e);
    printf("Apres echange : pi = %.2f, e = %.2f.\n",pi,e);
    //Les variables de sont pas échangés
    return 0;
}