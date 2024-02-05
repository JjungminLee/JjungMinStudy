#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>



int main(int argc,char* argv[]){
    struct stat statbuf;
    char* fname = "umask_text1.txt";
    int fd;
    if((fd=open(fname,O_RDWR | O_CREAT,600))<0){
        fprintf(stderr,"open error for %s \n",fname);
        exit(1);
    }
    close(fd);
    stat(fname,&statbuf);
    printf("# 1st stat call #UID :%d GID:%d\n",statbuf.st_uid,statbuf.st_gid);
    if(chown(fname,501,300)<0){
        fprintf(stderr,"chown error for %s \n",fname);
        exit(1);
    }
    stat(fname,&statbuf);
    printf("# 2nd stat call #UID :%d GID:%d\n",statbuf.st_uid,statbuf.st_gid);
    if(unlink(fname)<0){
        fprintf(stderr,"unlink error for %s \n",fname);
        exit(1);
    }

    exit(0);



}