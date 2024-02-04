#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int main(void){
    char *fname = "open_text.txt";
    int fd;

    if((fd=open(fname,O_RDONLY))<0){
        fprintf(stderr,"open error for %s\n",fname);
    }else{
        printf("Success!\n Filename:%s\n Descriptor:%d\n",fname,fd);

    }
    exit(0);

}