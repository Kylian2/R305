#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>

void work(char c){
    time_t now;
    fork();
    for (int i = 0; i<50; i++){
        putc(c, stdout);
        fflush(stdout);
        sleep(1);
    }
    printf("\n %d finished working at %ld \n", getpid(), time(&now));
}

int main(){
    fork();
    work('.');
    return 0;
}