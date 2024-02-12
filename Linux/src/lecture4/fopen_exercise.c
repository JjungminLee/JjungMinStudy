#include <stdio.h>
#include <stdlib.h>

int main(){
    char* fname= "fopen_text.txt";
    char* mode ="r";

    if(fopen(fname,mode)==NULL){
        fprintf(stderr,"fopen error for %s \n",fname);
        exit(1);
    }
    else{
        printf("Success! \n Filename:<%s>, mode:<%s> \n",fname,mode);
    }
    exit(0);

}