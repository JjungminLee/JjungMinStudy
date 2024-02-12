## 표준 입출력 라이브러리

- 이전까진 저수준 파일 입출력을 공부함. 고수준 파일 입출력은 표준 입출력 라이브러리로 부르기도함.
- 데이터를 바이트 단위로 한정하지 않고, 버퍼를 이용해 한꺼번에 파일을 읽고 쓸 수 있는 기능을 제공하기에 문자단위, 행단위, 버퍼 단위의 입출력이 가능하며 포맷지정도 가능

## fopen(),freopen(),fdopen()

```c
#include <stdio.h>

FILE* fopen(const char* pathname,const char* mode);
FILE* fdopen(int filedes,const char* mode);
FILE* freopen(const char* pathname,const char* mode,FILE* fp);
```

- 파일과 연관된 표준 입출력 스트림을 오픈하는 라이브러리 함수들.
- open()호출과 비슷하게 fopen()함수 호출이 완료되면 FILE구조체 변수 생성후 그 구조체의 포인터를 리턴함.
- 모든 작업에서 파일을 사용하기 위해 파일 스트림 포인터를 이용하게 된다.
- fopen()의 pathname인자는 오픈하고자하는 파일경로를 의미.
  - 경로이름은 절대경로,상대경로 모두 사용가능
  - fopen()은 메모리 상에 pathname인자와 연관된 FILE형 구조체 영역과 파일에 대한 입출력 버퍼를 확보후 FILE 형 구조체 영역의 시작주소를 함수값으로 되돌림.
  - FILE형 구조체의 멤버변수는 오픈한 파일을 처리하는데 필요한 각종 정보가 자동적으로 기록됨.
  - FILE구조체는 각 파일 입출력 함수에 의해 사용되며 사용자가 FILE구조체의 각 멤버변수를 직접 사용하지는 않는다.
- fdopen()은 파일 디스크립터를 이용하여 파일 스트림 포인터를 리턴하는 역할
  - 파이프나 네트워크 같이 fopen()으로 오픈할 수 없는 특수 파일의 파일 디스크립터를 파일 포인터와 연관시켜 입출력 프로그래밍을 쉽게할수 있게 함.
- freopen()는 fp 인자로 지정된 파일의 파일 스트림 포인터로 다시 오픈하는 함수
  - freopen() 호출되면 이미 오픈한 fp를 flcose()호출한것 처럼 닫고 pathname인자로 지정한 파일을 fopen() 호출한것 처럼 오픈

```c
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
```

- 결과값
  Success!
  Filename:<fopen_text.txt>, mode:<r>
- 파일을 읽기권한으로 오픈하여 접근모드 확인하는 코드

## fclose(),fcloseall()

```c
#include <stdio.h>

int fclose(FILE* fp);

#define _GNU_SOURCE
#include <stdio.h>
int fcloseall(void);
```

- 오픈된 파일 스트림을 닫는 라이브러리함수.
- 만약 닫고자하는 파일 스트림이 출력을 위해 오픈된 스트림이라면 fflush()를 이용해 버퍼에 남아있는 데이털르 모두 써야한다.
- fclose()는 오픈되어 있는 파일 스트림을 닫는다.
- fcloseall()은 표준 입력, 표준 출력, 표준 에러를 포함해 현재 프로세스와 관련있는 모든 스트림을 닫고 닫기전에 버퍼에 남아있는 내용을 모두 스트림에 쓰고 0을 리턴

## setbuf(), setvbuf()

```c
#include <stdio.h>

void setbuf(FILE* fp,char* buf);
int setvbuf(FILE* fp,char* buf,int mode, size_t size);
```

- 표준 입출력 라이브러리가 제공하는 버퍼링의 종류를 정하는 라이브러리 함수들
- 표준 입출력 라이브러리가 제공하는 버퍼링은 읽기 시스템호출과 쓰기 시스템호출의 횟수를 최소화 하는것,
- 표준 입출력 라이브러리는 세가지 종류의 버퍼링을 지원
  - full-buffered : 표준 입출력 버퍼가 꽉 찼을 때 실제 입출력이 일어남. (\_IOFBF)
    - 디스크에 있는 파일들이 적용됨.
    - full-buffered에 사용되는 버퍼는 주어진 한 스트림에 대하여 입출력 함수가 처음 수행될 시 malloc() 호출해서 할당
  - line-buffered : 입력이나 출력에서 개행문자를 만났을 때 입출력을 수행하고, 주로 터미널을 가리키는 스트림에 쓰인다. (\_IOLBF)
  - non-buffered: 문자들을 버퍼링하지 않으며 즉시 출력 (\_IONBF)
    - 표준 에러 스트림
- setbuf()는 fopen()나 freopen()에 의해 오픈된 파일에서 자료의 입출력이 수행되기 전에 호출되어 버퍼의 사용방법 결정.
  - buf인자를 NULL로 설정하면 버퍼 사용하지 않겠다는것
  - buf인자를 NULL이 아닌것으로 설정하면 BUFSIZE만큼의 char배열을 버퍼로 사용한다.

```c
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
```

- 버퍼가 설정된 애는 버퍼에 넣어서 한꺼번에 출력되지만 버퍼가 설정되지 않은 애는 즉시 출력 (1초에 한번씩 출력)
