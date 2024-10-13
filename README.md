# Note de TP

# TP1 

## Compiler un fichier C

Pour compiler en C, il faut utiliser la commande suivante dans un terminal : 
```
gcc -Wall -Werror <source> -o <output>
```

## Rappel sur les sorties en C

Le langage C ne dispose pas du flot de sortie cout bien connu en C++. À sa place, on utilise une fonction spécifique nommée printf(). L’instruction `printf("La moyenne des %d entiers est %f.\n", i, moyenne)`;
affiche sur la sortie standard (l’écran) la chaîne de caractères comprise entre les guillemets (appelée un format) mais où %d a été remplacé par la valeur de la variable entière i, %f a été remplacé par la valeur de la variable réelle moyenne et \n n’a pas été affiché mais un retour à la ligne a été effectué.
%d et %f sont appelés des codes. Ils définissent la manière dont une variable d’un certain type doit
être affichée. Le nième code est destiné au nième paramètre après la chaîne de caractères. La carte de
référence vous donne plus de détails sur les codes. Le caractère spécial \n désigne le retour à la ligne.

Les différents formats sont listé dans le fichier carte_reference_c.pdf

## Faire un string en C

Pour faire un string en c, il faut définir un tableau de caractère. 
ex : 
```c
char prenom[] = "Kylian";
printf("hello %s ! \n", prenom);
```

## Rappel sur les pointeurs 

Dans le langage C, on peut accéder directement à des zones de la mémoire par l’intermédiaire de leurs adresses. Pour connaître l’adresse d’une variable, on utilise l’opérateur
`&`: par exemple, si `num` est une variable de type entier, alors `&num` est l’adresse à laquelle se trouve
cette variable. Pour connaître la taille en octets de la zone mémoire allouée à une variable, on utilise
l’opérateur `sizeof()`, par exemple, la place occupée par num est donnée par `sizeof(num)` ou encore
`sizeof(int)`. Pour afficher le résultat de sizeof() avec printf(), on utilise le code `%zu`.
**On appelle les variables qui contiennent les adresses les pointeurs** (on peut s’en servir pour
stocker &num, par exemple). On déclare un pointeur en ajoutant le caractère étoile `*`devant le nom
de la variable. Par exemple, l’instruction suivante :

```c
int *ptr;
```
déclare une variable nommée `ptr` destinée à contenir l’adresse de début d’une zone de mémoire
contenant un entier. On dit que `ptr` est **un pointeur sur entier**. Dans le reste du programme, on utilisera ptr pour désigner l’adresse d’un entier (par exemple, ptr aurait pour valeur l’adresse de num si on effectuait l’affectation ptr = &num), et *ptr pour désigner l’entier qui se trouve à cette
adresse (ainsi, pour changer la valeur de num, on pourrait exécuter *ptr = 2). Soit le code suivant :

```c
#include <stdio.h>

int main() {
    double valeur = 10.0;
    double *pv = &valeur;
    int nombre = 1, *pn;
    pn = &nombre;
    valeur = *pv + *pn;
    printf(" valeur = %f\n", valeur);
    printf("&valeur = %p\n", &valeur);
}
```
- pv : poiteur de double, il contient l'addresse de `valeur`.
- &pv : représente l'adresse du pointeur pv.
- *pv : représente la valeur contenue dans la variable pointée. (ici cela représente la valeur de `valeur`).

Résultat de l'éxécution : 
```
 valeur = 11.000000
&valeur = 0x7fffb7bc6698
```

Tailles des éléments : 

| Variable | Taille en mémoire |
| :--- | :--- |
| valeur | 8 octets | 
| pv | 4 octets pour x32 et 8 octets pour x64| 
| nombre | 4 octets| 
| pn | 4 octets pour x32 et 8 octets pour x64 | 

valeur et nombre étant des variables de type différents, leur taille est différente. En revance pv et pn sont tout les deux des pointeurs, il contienent une adresse mémoire de 4 ou 8 octets, ils ont la même taille même s'il ne pointe pas sur le même type. 

## Le passage de paramètres
En C, les paramètres des fonction sont des copies des arguments passés lors des appels. Cela signifie que l'on ne travaille jamais directement sur les éléments passé lors de l'appel mais avec des d'autres variables qui contiennent exactement les mêmes valeurs.
ex : 

```c
#include <stdio.h>
void echange1(float a, float b) {
    float temp = a;
    a = b;
    b = temp;
}
int main() {
    float pi = 2.71828, e = 3.14159;
    printf("Avant echange : pi = %.2f, e = %.2f.\n",pi,e);
    echange1(pi,e);
    printf("Apres echange : pi = %.2f, e = %.2f.\n",pi,e);
    return 0;
}
```
Ici, les variables ne seront pas échangées. 
Mais si on refait une fonction avec des pointeurs, on pourra manipuler les variables via leurs addresses : 

```c
void echange2(float *a, float *b) {
    float temp = *a;
    *a = *b;
    *b = temp;
}
```
On prend en paramètres les addresses des arguments et on manipule via les ces pointeurs les valeurs de pi et e. On stock dans `temp` la valeur de `a`(donc celle de `pi`), on donne à `a` la valeur de `b`(donc celle de `e`) et on donne à `b` la valeur de `temp`(la valeur d'origine de `a`). 