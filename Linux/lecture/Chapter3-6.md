## umask()

```c
#include <sys/types.h>
#include <sys/stat.h>
mode_t umask(mode_t cmask);
// 리턴값 : 이전의 파일 모드 생성 마스크
```

- 프로세스의 파일 생성 마스크를 생성하고 이전 값을 리턴하는 시스템호출 함수.
- 시스템 보안 장치 중 하나로 사용자가 파일 접근 권한을 느슨하게 풀어뒀을 때 발생하는 보안상의 문제를 사전에 방지해준다.
- 파일이나 디렉토리 생성하는 시스템호출 시 프로세스의 umask값을 참조하여 특정한 접근권한을 막게됨.
- 프로세스 내에서 umask값을 일시적으로 변경시키는데도 사용
- 프로세스 내에서 변경된 umask값은 프로세스가 종료할때까지만 유효 , 종료되면 원래값으로 변경된다.
- 여러개 플래그들을 논리합 연산으로 결합한 값을 가질 수도 있다.
  - umask값이 011 파일 생성시 파일 모드 값을 0777을 설정하면 비트연산을 통해 0766으로 모드값 변환후 파일 생성함.

```c
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
```

- umask()를 통해 마스크 값을 지정하고, 생성된 파일의 접근권한을 확인한다.
- umask()를 호출하여 파일 생성시 파일의 마스크값을 0으로 만들고 생성한 파일의 접근권한을 RW_MODE로 하여 파일 소유자, 그룹 사용자, 다른 사용자 모두 읽기,쓰기가 가능하게 한다.
- file2의 경우는 파일의 소유자만 읽기, 쓰기가 가능하게 한다.
- umask를 통해 임의로 변경한 파일생성 마스크 값은 부모 프로세스 쉘에는 영향을 주지 않는다!

## chmod(), fchmod()

```c

#include <sys/stat.h>
int chmod(const char* pathname,mode_t mode);
int fchmod(int filedes,mode_t mode);
```

- 기존 파일의 접근권한을 변경하기 위해 사용되는 시스템 호출함수
- chmod()를 통해 변경하려면 프로세스의 유효 사용자 ID가 파일 사용자 ID와 같거나 루트 권한으로 실행되고 있어야한다.
- chmod()의 경우 첫번째 인자로 파일의 경로 이름을, fchmod()는 이미 개방되어있는 파일 디스크립터를 사용해야한다.
- mode 인자의 경우 umask()에서 사용하는 cmask인자와 동일

```c
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
```

- chmod()를 통해 1번파일은 SUID를 설정 , 2번 파일은 인자로 지정한 모드로 접근권한을 변경한다
- **`(statbuf.st_mode & ~S_IXGRP)`**는 비트 연산을 사용하여 파일의 모드에서 그룹 실행 권한 (**`S_IXGRP`**)을 제거하는 연산
- 실행결과
  ❯ ls -l umask_text1.txt umask_text2.txt
  -rwSrw-rw- 1 jungmin staff 0 2 5 21:31 umask_text1.txt
  -rw-r--r-x 1 jungmin staff 0 2 5 21:31 umask_text2.txt

## chown(), fchown(), lchown()

```c
#include <unistd.h>
int chown(const char* pathname,uid_t owner,gid_t group);
int fchown(int filedes,uid_t owner,gid_t group);
int ;chown(int filedes,uid_t owner,gid_t group);
//성공시 0 리턴 에러시 -1리턴
```

- 파일 소유자 ID와 그룹사용자 ID를 변경하기 위해 사용되는 시스템호출 함수
- 적절한 권한이 있는 사용자만 이 함수들을 호출 가능
  - 일부 시스템은 파일 소유자 ID와 그룹 사용자 ID변경하는 것을 루트권한이 있어야 가능하게 한다.
- chown(),fchown() : 지정한 파일이 심볼릭 링크이면 chown()과 fchown()은 심볼릭 링크가 가리키는 파일 소유자를 변경한다.
- lchown() : 심볼릭 링크 파일 자체의 소유자 변경
- owner인자나 group인자가 -1이면 해당 ID는 변경되지 않는다.

```c
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
```

- 파일 소유자 ID와 그룹 사용자 ID를 chown()를 통해 변경한다.
- 루트 권한으로 실행한 결과이기 때문에 소유자 ID, 그룹 사용자 ID가 모두 0이었던 파일이 chown()인자로 사용된 소유자 ID와 그룹 사용자 ID로 변경된것을 확인할 수 있다.
