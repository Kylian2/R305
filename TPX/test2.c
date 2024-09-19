#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>

int main(){
    fork();
    wait(NULL);
    printf("Hello \n");
    return 0;
}