# 파일과 파일 디스크립터

### 리눅스에서의 파일

- 리눅스/유닉스 계열에서 `파일`은 `데이터를 읽을 수` 있거나 `데이터를 쓸 수 있는` 모든 객체를 말한다.
- 파일의 구분
  - 일반파일 : 사용자 프로그램,시스템 유틸리티 프로그램에 의해 입력된 정보를 포함
  - 디렉토리 파일 : 일반 파일을 조직하고 접근할 수 있는 정보를 포함하는 파일
  - 특별한 파일 : 터미널이나 프린터와 같이 입출력 디바이스의 접근을 위해 사용

### 파일 디스크립터

- 앞서 본 파일들을 시스템 프로그래밍 차원에서 `바이트 단위로 입출력을` 다룰 수 있게 하고, 커널 내부의 자료구조들과 연결통로 역할을 할 수 있게 하는것이 파일 디스크립터
- open(), creat(), dup()의 리턴값
- 리턴 값은 read(), write(), fsync(), fdatasync()의 첫번째 인자로 사용됨
- 파일 디스크립터의 데이터 타입은 int로 음수가 아닌 정수값 가짐.
- 0 : 프로세스의 표준 입력
- 1: 프로세스의 표준 출력
- 2: 프로세스의 표준 에러

### 파일디스크립터 테이블

- 파일 디스크립터를 위한 자료구조
- 구조체로 되어있음!(사진 참고 할것)
- 한 프로세스가 오픈한 파일들의 정보가 포함되어 있기 때문에 오픈 파일 테이블이라고 함.
- open()함수 호출하여 리턴 받는 파일 디스크립터는 프로세스가 오픈한 파일 목록을 관리하는 파일 디스크립터 테이블의 인덱스가 됨
- 각 프로세스들은 open()을 호출하여 서로 다른 파일을 오픈한다고 해도 동일한 파일디스크립터 번호를 리턴받음
  - 왜냐면 파일디스크립터 테이블은 프로세스마다 하나씩 할당되기 때문
- open()을 호출하여 리턴 받은 파일 디스크립터를 통해 파일을 접근할 때 커널은 file테이블을 통해 파일에 대한 모든 정보가 담긴 i-node계속 참조.
- i-node는 디스크에 있기에 읽는 오버헤드를 줄이기 위해 먼저 i-node정보를 메모리에 올려 사용

## open

```c
#include <sys/types.h>
#include <sys/stat.>
#include <fcntl.h>

int open(const char* pathname,int oflag);
int open(const char* pathname,int oflag,mode_t mode);
```

- 파일을 오픈하거나 생성할 때 사용하는 시스템 호출 함수
- 모든 파일은 사용되기 전 반드시 open()을 호출해야!
- open호출시 일어나는 일
  1. 동일한 파일이 시스템에 존재하고 있는 지 확인
  2. 파일에 대한 올바른 접근권한 검사하고 파일 디스크립터 리턴
- Open의 pathname 인자로 현재 작업디렉토리부터 시작하는 상대경로나 절대경로 모두 사용 가능
- open의 oflag 인자는 open()의 작동방식, 즉 해당파일을 어떤 영도로 사용될 것인지 결정하는 것으로 오픈할 파일의 특성을 나타냄.
- 인자는 <fnctl.h>에 정의된 함수들 중 하나를 여러개 논리합 연산으로 결합한 값이 사용됨.
- 반드시 사용해야하는 플래그
  - O_RDONLY : 읽기전용
  - O_WRONLY : 쓰기 전용
  - O_RDWR : 읽기와 쓰기 모두 가능하게
  - 얘네 셋은 동시에 두가지 이상의 플래그를 함께 지정하면 안됨
- 선택적으로 사용해도 되는 플래그
  - O_APPEND : 파일 기록시 해당 파일의 오프셋 위치를 자동으로 파일의 끝으로 보냄
  - O_CREAT : 파일이 존재하지 않을 경우 같은 이름의 파일을 새로 생성
  - O_TRUNC : 해당 파일이 이미 존재할 경우 파일의 길이가 0이 되면서 이전 데이터를 전부 잃게됨.

```c
fd=open("file.txt",O_CREAT|O_RDWR|O_TRUNC, 0666);
```

- 입출력 동기화를 위해 옵션으로 사용하는 플래그
  - O_DSYNC : 각 쓰기 연산이 물리적 입출력 연산의 완료를 기다림, 파일의 속성값은 바로 갱신되지 않음
  - O_RSYNC : 파일 디스크립터에 각 읽기 연산이 파일의 동일한 부분에 대해 보류된 쓰기의 완료 기다림
  - O_SYNC : 각 쓰기 연산이 물리적 입출력 연산의 완료 기다림
- 리눅스와 유닉스에서 디스크 쓰기를 시도하면 시스템의 효율을 높이기 위해 해당 데이터는 메모리의 일정 영역(버퍼 캐쉬)에 잠시 보관되었다가 적당한 때가 되면 디스크에 실제로 쓰는 지연쓰기 정책 사용
- 입출력 동기화를 위한 플래그를 사용하면 즉시 디스크에 데이터를 쓰며, 쓰기가 종료되면 호출이 리턴

- 생성할 파일의 접근권한을 나타내는 mode인자
  - S_IRWXU : 파일 소유자의 읽기, 쓰기, 실행권한을 지정 00700사용해도됨.
  - S_IRUSR : 파일 소유자의 읽기 권한을 지정 00400을 사용해도됨.
  - S_IWUSR : 파일 소유자의 쓰기 권한을 지정 00200을 사용해도됨

## close()

```c
#include <unistd.h>
int close(int filedes);

// 성공하면 0, 에러나면 -1리턴
```

- 오픈한 파일을 닫을 때 사용하는 시스템 호출 함수
- 사용이 끝난 함수는 close()사용하여 바로 닫는게 좋다.
  - 사용중인 파일 디스크립터가 너무 많으면 실수로 의도하지 않은 파일에 데이터를 덮어쓰는 경우도 발생할 수 있고 시스템 자원도 낭비됨
- 리눅스에서 오픈할 수 있는 파일의 개수는 제한이 있음
  - cat /proc/sys/fs/file-nr 명령어를 사용하여 확인 가능
  - 리눅스에서 오픈한 파일은 shell에서 lsof라는 명령어로 확인 가능
- 파일이 닫히면 해당 디스크립터에 대한 모든 입출력 시도는 실패함
  - 이후 open()이나 creat()에 의해 새로 생성되는 다른 파일이 이전의 파일 디스크립터 사용 가능
  - 프로세스 종료하기 전이라면 굳이 close()써서 모든 오픈 한 파일 닫을 필요 없음

### open()예제

```c
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
```

- 결과값

Filename:open_text.txt
Descriptor:3

- 오픈 성공시 이미 오픈되어있는 파일 디스크립터(0:포준입력,1:표준출력,2:표준에러) 리턴
- 다음 순서의 파일 디스크립터 3을 리턴

## creat()

```c
#include <sys/types.h>
#include <sys/stat.>
#include <fcntl.h>

int creat(const char* pathname,mode_t mode);
```

- 파일을 생성할 때 사용하는 시스템 호출 함수
- 파일에 쓴 데이터 다시 읽으려면 open(pathname,O_WRONLY | O_CREAT | O_TRUNC,mode); 를 사용하는 것이 좋다.
- 파일 생성을 성공하기 위해서는 파일을 생성하고자 하는 패스에 해당하는 디렉토리에 대해 쓰기 권한을 가지고 있어야 한다.
- creat()도 성공하면 파일 디스크립터 리턴
- 단점은 접근모드가 쓰기 전용이라는 것

```c
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(void){
    char *fname = "creat_text.txt";
    int fd;

    if((fd=creat(fname,0666))<0){
        fprintf(stderr,"creat error for %s\n",fname);
    }else{
        printf("Success!\n Filename:%s\n Descriptor:%d\n",fname,fd);
        close(fd);
    }
    exit(0);

}
```

- 위 예제에서 파일 오픈까지 하고 싶으면

```c
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(void){
    char *fname = "creat_text.txt";
    int fd;

    if((fd=creat(fname,0666))<0){
        fprintf(stderr,"creat error for %s\n",fname);
    }else{
				close(fd);
				fd = open(fname,O_RDWR);
        printf("Success!\n Filename:%s\n Descriptor:%d\n",fname,fd);

    }
    exit(0);

}
```
