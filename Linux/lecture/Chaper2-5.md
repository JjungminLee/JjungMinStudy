## lseek()

```c
#include <sys/types.h>
#include <unistd.h>

off_t_lseek(int filedes,off_t offset,int whence);
// 성공시 새 파일 오프셋, 에러시 -1 리턴하고 errno설정
```

- 오픈된 파일의 오프셋 위치를 명시적으로 변경할때 사용하는 시스템 호출 함수
- l이 long integer를 의미.
- off_t데이터 타입이 정의되기 전 offset인자와 리턴 데이터 타입은 Long형
- 커널 내부에서 현재 파일의 오프셋 위치만을 변경 가능
  - 파일 오프셋 : **파일의 시작지점부터 현재 커서의 위치까지 얼마나 떨어져 있는지 정수로 보여주는 것이 offset 값**
- 별도의 입출력을 하지 않으며 변경된 오프셋은 read()와 write()가 호출될 때 사용됨
- 파일에 대한 입출력은 바이트 단위의 순차적인 스트림으로 진행.
- read()와 write()는 파일의 현재 오프셋 위치부터 수행되며 입출력 동작 후 새로운 오프셋 위치는 이전 오프셋 위치에 지정된 바이트 수만큼 더해진 값으로 갱신
- 오프셋 위치의 기본값은 O_APPEND상태 플래그가 설정되어있지 않는 한 파일이 오픈될때 0으로 초기화
  - O_APPEND상태 플래그가 설정되어있다면 lseek() 사용하지 않더라도 write()는 항상 파일 오프셋의 위치를 파일 끝으로 이동
- 파일의 현재 위치와 다른 위치에 입출력하기 위해서는 파일의 오프셋 위치를 변경해야하는데 인자로 지정한 whence값에 따라 동작이 다르게 이어짐
- whence로 지정할 수 있는 값
  - SEEK_SET: 파일의 오프셋 위치를 파일의 처음으로 지정
  - SEEK_CUR: 파일의 오프셋은 현재 오프셋위치에 offset인자 더한 값으로 설정, 오프셋 설정 시의 기준위치가 파일의 현재 오프셋이됨.
  - SEEK_END: 파일의 오프셋은 파일 크기에 Offset이 더해져서 인자로 설정.
- 파일의 오프셋은 그 파일의 실제 크기보다 클수 있다.
- 이런 경우 다음 write()에서 초과된 오프셋 만큼 파일의 크기가 커진다.
- 현재 오프셋 위치를 알려면 현재 오프셋 위치로부터 0바이트 지정하면된다.
  - 이 방법은 어떤 파일이 lseek()가 호출가능한지 확인하는데도 사용 가능
  ```c
  off_t currops;
  currops = lseek(fd,0,SEEK_CUR);
  ```
- 파일 끝에 내용을 추가하려면
  ```c
  filedes = open(filename,O_RDWR);
  lseek(filedes,(off_t)0,SEEK_END);
  write(filedes,buf,BUFSIZE);
  ```
- 파일 크기를 알아내려면
  ```c
  off_t filesize;
  filesize = lseek(filedes,(off_t)0,SEEK_END);
  ```
- 파일을 한번 오픈할 때 마다 새로운 파일 디스크립터를 할당받음
  - 단지 파일 디스크립터만 할당받는게 아닌 파일의 테이블 상에서도 새로운 항목을 할당받음
  - 하나의 파일이 서로 다른 파일디스크립터와 오프셋을 가지고 접근될 수 있음을 의미
- 첫번째 디스크립터를 통해 파일을 읽고나면 파일의 현재 오프셋 위치가 갱신
- 두번째 파일 디스크립터는 자신의 파일 오프셋 위치를 따로 가지고 있으므로 앞의 읽기 연산에 전혀 영향 받지 않음
- 두 파일 디스크립터는 각기 독립적으로 동작.

```c
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

int main(void){
    char* fname = "open_text.txt";
    off_t fsize; //off_t는 파일 오프셋(offset)을 나타내는 데이터 형식
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
```

- 결과값

The size of <open_text.txt> is 25 bytes

+) exit(0)과 exit(1)의 차이

프로그램이 모두 실행되고 **정상 종료**되는 것을 의미할 땐 **exit(0)**을 사용한다.

하지만 실행 중간의 에러가 발생할 시 에러로 인한 **강제종료는 exit(1)**을 해주어야 한다

## read()

```c

#include <unistd.h>

sszie_t_read(int filedes,void* buf,size_t nbytes);

```

- 오픈된 파일에서 데이터를 읽을 때 사용하는 시스템 호출함수
- 읽기전용 또는 읽기,쓰기 혼용 접근 권한으로 오픈된 파일로부터 지정된 메모리로 정해진 바이트 만큼의 데이터를 일겅온다.
- read()호출이 성공하면 실제로 읽은 바이트 수가 리턴되며 파일의 끝을 읽으려는 경우에는 0이 리턴.
- read()호출이 완료되면 파일의 현재 오프셋 위치는 이전 오프셋 위치에 실제 읽은 바이트 수만큼이 더해진 값으로 갱신
- 현재 오프셋 위치에서 남은 바이트가 50바이트인데 100바이트를 읽어야 한다면 50 바이트 만큼의 데이터만 메모리로 읽고 리턴값도 50이 된다.
- 한 프로세스가 read()를 호출하게 되면 커널은 호출이 수행되는 동안 i-node에 락을 걸고 해당 파일에 접근하려는 다른 모든 연산을 블록 시킨다.
  - 다른 연산이 블록되는 이유는 read()가 즉시 리턴하지 못하고 블록되는 경우, 다른 프로세스가 그 사이에 먼저 자신의 데이터를 쓸 수 있으며 이럴 경우 데이터 일관성이 깨질 수 있기 때문.
- read()가 완료되기 전까지 해당 i-node에 대한 어떠한 접근도 허용되지 않는다.
- open_text.txt의 내용을 읽는 예제

```c
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
```

- 결과값

I hate school
I hate LSP!%
