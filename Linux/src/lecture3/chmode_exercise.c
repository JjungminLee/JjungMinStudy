#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>



int main(int argc,char* argv[]){
    struct stat statbuf;
    char* fname1 = "umask_text1.txt";
    char* fname2 = "umask_text2.txt";

    if(stat(fname1,&statbuf)<0){
        fprintf(stderr,"stat error %s \n",fname1);
    }
    if(chmod(fname1,(statbuf.st_mode &~S_IXGRP)|S_ISUID)<0) {
        //파일에 설정되면 해당 파일이 실행될 때 파일 소유자의 권한으로 실행되도록
        fprintf(stderr,"chmod error %s \n",fname1);
    }
    if(chmod(fname2,S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH| S_IXOTH)<0) {
        //파일에 설정되면 해당 파일이 실행될 때 파일 소유자의 권한으로 실행되도록
        fprintf(stderr,"chmod error %s \n",fname2);
    }

    exit(0);



}