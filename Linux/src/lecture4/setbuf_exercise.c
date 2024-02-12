#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main(void){

    char buf[BUFFER_SIZE];

    setbuf(stdout,buf);
    printf("Helllo, ");
    sleep(1);
    printf("WORLD!! ");
    sleep(1);
    printf("\n");
    sleep(1);
    
    setbuf(stdout,NULL);
    printf("How");
    sleep(1);
    printf(" are");
    sleep(1);
    printf(" you?");
    sleep(1);
    printf("\n");
    exit(0);
}