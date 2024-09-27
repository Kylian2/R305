#include<signal.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

volatile unsigned int count = 0;
volatile int step = 1;

void handleSR1 (int sig){
    count = 1;
}

void handleSR2(int sig){
    step -= step;
}

void handleALARM(int sig){
    printf("%d \n", count);
    alarm(1);
}

int main(){

    time_t start;
    time_t end;

    signal(SIGUSR1, handleSR1);
    signal(SIGUSR2, handleSR2);
    signal(SIGALRM, handleALARM);
    alarm(1);
    start = time(NULL);
    for(count = 1; count > 0; count += step);
    end = time(NULL);
    printf(" \n Temps d'éxécution : %.2fs \n", difftime(end, start));

    return 0;
}