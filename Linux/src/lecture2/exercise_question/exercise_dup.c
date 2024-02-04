#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#define BUFFER_SIZE 1024

int main(void){
    
    char buf[BUFFER_SIZE];
    char* fname = "dup_exercise.txt";
    int count;
    int fd1,fd2;
    if((fd1=open(fname,O_RDONLY,0644))<0){
        fprintf(stderr,"open error for %s \n",fname);
        exit(1);
    }
    if((count=lseek(fd1,0,SEEK_END))<0){
        fprintf(stderr,"lseek error \n");
        exit(1);
    }
    printf("%d\n",count);
    // 파일의 시작을 기준으로 오프셋 설정
    lseek(fd1,0,SEEK_SET);
  
    fd2 = dup(fd1);
    // lseek(fd2, 0, SEEK_SET);
    char c;
    while(1){
        if(read(fd2,&c,1)>0){ //  파일 디스크립터 fd2에서 1바이트를 읽어와 변수 c에 저장한 후, 읽은 바이트 수가 0보다 큰지를 비교하는 조건문
            // putchar(c);
            fprintf(stdout,"%c",c);
        }else{
            break;
        }
    }
    close(fd1);
    close(fd2);
    exit(0);
}