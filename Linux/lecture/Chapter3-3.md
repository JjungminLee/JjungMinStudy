## stat()

```c
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int stat(const char* restrict pathname, struct stat* restrict buf);
int fstat(int filedes, struct stat *buf);
int lstat(const char* restrict pathname, struct stat* restrcit buf);
```

- restrict ?
  - 포인터 최적화
  - restrict를 쓰면 그 포인터가 가리키는 객체는 다른 포인터가 가리키지 않는다.
  - \*은 자료형과 restrict 사이에 위치해야한다.
- stat(), fstat(), lstat()은 인자로 지정한 pathname 및 filesdes에 해당하는 파일에 대한 정보를 포함하고 있는 stat구조체를 리턴하는 시스템호출 함수.
- stat구조체의 멤버변수는 운영체제 마다 다르게 구현
- stat()
  - pathname인자로 지정한 `경로에 해당하는 파일에 대한 정보를 넣은 구조체`를 두번째 인자를 통해서 리턴
- fstat()
  - 파일 디스크립터 filedes인자에 대해 `이미 열려있는 파일에 대한 정보` 확인
- lstat()
  - stat()와 다르게 지정된 파일이 심볼릭 링크면 그 링크가 가리키는 파일이 아닌 그 `파일 자체에 대한 정보 리턴`
  - 심볼릭 링크 : 링크를 연결하여 원본 파일을 직접 사용하는 것과 같은 효과를 내는 링크이다. 윈도우의 바로가기와 비슷한 개념
  - stat관련 함수들을 사용하는 대표적인 것이 쉘에서 사용하는 ls 명령어
  - 쉘에서 chmod 명령어를 사용하면 파일의 읽기/쓰기/실행 권한을 변경할 수 있다.

```c
struct stat{
	dev_t st_dev; 디바이스 번호
	mode_t st_mode; // 모드
	off_t st_size; // 파일 크기(바이트)
	time_t st_atime; // 최종접근시간
	time_t st_mtime; // 최종수정시간
}
```

```c
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc,char* argv[]){
    struct stat statbuf;

    if(argc!=2){
        fprintf(stderr,"usage: %s <file> \n",argv[0]);
        exit(1);
    }

    if((stat(argv[1],&statbuf))<0){
        fprintf(stderr,"stat error \n");
        exit(1);
    }
    printf("%s is %lld bytes \n", argv[1], (long long)statbuf.st_size);
    exit(0);

}
```

- 인자로 지정한 파일의 정보를 확인
  - 파일 정보는 stat구조체를 통해 리턴
  - 결과
    ❯ ./a.out stat_text.txt
    stat_text.txt is 5 bytes
  - 명령어
    - gcc stat_exercise.c
    - ./a.out이 생기면 ./a.out stat_text.txt

## 파일의 종류

- 파일은 크게 일반 파일, 디렉토리 파일, 특수 파일이 있다.
  - 특수파일은
    - 블록 특수파일 (디스크 드라이브 같은 장치에 대한 고정 크기 단위의 버퍼링 입출력 접근 제공)
    - 문자 특수파일 (장치에 대한 가변 크기 단위의 비버퍼링 출력 접근을 제공하는 파일)
    - FIFO (프로세스 간 통신에 쓰이는 파일)
    - 소켓
    - 심볼릭 링크
- 파일의 종류를 확인하려면 stat()을 호출하여 S_IFMT(파일타입 비트 필드 위한 비트 마스크)와 논리곱(AND)연산을 하면 된다.
  ```c
  stat(pathname,&statbuf);
  if((statbuf.st_mode & S_IFMT) == S_IFREG){
  	//일반파일임
  }
  ```
- 파일 매크로를 사용하는 방법도 있다.
  - 사실상 #define S_ISREG(statbuf.st_mode)
  ```c
  stat(pathname,&statbuf);
  if(S_ISREG(statbuf.st_mode)){
  	//일반파일임
  }
  ```

## access()

```c

#include <unistd.h>

int access(const char* pathname,int mode);
```

- 인자로 지정한 파일에 대해 사용자 또는 프로세스가 접근가능한지 판단하는 시스템호출 함수
- 프로세스가 파일에 접근하기 위해 먼저 파일 접근권한을 검사하는데
  - 검사대상이 되는건 프로세스 유효 사용자 ID와 유효그룹 ID
- mode인자로 사용되는 상수
  - R_OK : 읽기권한 판정
  - W_OK : 쓰기권한 판정
  - X_OK : 실행권한 판정
  - F_OK : 파일 존재여부 판정

```c
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc,char* argv[]){
    int i;
    if(argc<2){
        fprintf(stderr,"usage:%s <file1> <file2> .. <fileN>\n ",argv[0]);
        exit(1);
    }
    for(i=1;i<argc;i++){
        if(access(argv[i],F_OK)<0){
            fprintf(stderr,"%s doesn't exist.\n",argv[i]);
            continue;
        }
        if(access(argv[i],R_OK)==0){
            printf("User can read %s\n",argv[i]);
        }

    }
    exit(0);

}
```

- 실행결과
  ❯ ./a.out stat_text.txt
  User can read stat_text.txt
- 인자로 지정한 파일의 존재여부, 읽기, 쓰기 실행가능 여부를 판단 가능
