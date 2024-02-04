#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(void){
    char *fname = "creat_text.txt";
    int fd;

    if((fd=creat(fname,0666))<0){
        fprintf(stderr,"creat error for %s\n",fname);
    }else{
        printf("Success!\n Filename:%s\n Descriptor:%d\n",fname,fd);
        close(fd);
    }
    exit(0);

}