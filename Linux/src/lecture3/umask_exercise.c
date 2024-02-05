#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>

#define RW_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)

int main(int argc,char* argv[]){
    char* fname1 = "umask_text1.txt";
    char* fname2 = "umask_text2.txt";

    umask(0);

    if(creat(fname1,RW_MODE)<0){
        fprintf(stderr,"create error for %s \n",fname1);
        exit(1);
    }
    umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    if(creat(fname2,RW_MODE)<0){
        fprintf(stderr,"create error for %s \n",fname2);
        exit(1);
    }

    exit(0);



}