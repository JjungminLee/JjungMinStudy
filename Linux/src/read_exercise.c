#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(void){
    char c;
    int fd;
    int lineCount=0;
    if((fd=open("open_text.txt",O_RDONLY))<0){
        fprintf(stderr,"open error for %s \n","open_text.txt");
        exit(1);
    }
    while(1){
        if(read(fd,&c,1)>0){
            putchar(c);
        }else{
            break;
        }

        // if(read(fd,&c,1)>0){
        //     // putchar(c);
        //     if(c=='\n'){
        //         lineCount++;
        //     }
        // }else{
        //     break;
        // }
    }

    exit(0);

}