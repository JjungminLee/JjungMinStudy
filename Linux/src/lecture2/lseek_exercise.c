#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

int main(void){
    char* fname = "open_text.txt";
    off_t fsize;
    int fd;
    if((fd=open(fname,O_RDONLY))<0){
        fprintf(stderr,"open error for %s\n",fname);
        exit(1);
    }
    if((fsize=lseek(fd,0,SEEK_END))<0){
        fprintf(stderr,"lseek error\n");
        exit(1);
    }
    printf("The size of <%s> is %ld bytes\n",fname,(long)fsize);
    exit(0);

}