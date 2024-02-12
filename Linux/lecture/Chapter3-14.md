## utime()

```c
#include <sys/types.h>
#include <utime.h>
int utime(const char* pathname,const struct utimbuf* times);
```

- 파일의 최종접근시간과 최종 변경시간을 변경하는 시스템호출 함수.
- utimebuf구조체

  ```c
  struct utimebuf{

  	time_t actime; // 접근시간
  	time_t modtime; // 수정시간
  }
  ```

  - utime() 호출하기 위해서는 프로세스의 유효 사용자 ID가 파일의 사용자 ID와 같거나 프로세스가 파일에 대해서 쓰기권한을 가지고 있어야
  - utimebuf구조체 대한 포인터가 NULL이 아니면 최종접근 시간은 actime으로, 최종변경시간은 modtime으로 각각 변경된다.
  - 최종접근시간과 최종 변경시간을 변경하려면 프로세스의 유효 사용자 ID가 파일의 사용자 ID와 같거나 루트 권한으로 실행해야 한다.

## mkdir(), rmdir()

```c
#include <sys/types.h>
#include <sys/stat.h>
int mkdir(const char* pathname, mode_t mode);

#include <unistd.h>
int rmdir(const char* pathname);
```

- mkdir()은 디렉토리를 생성할 때 사용하는 시스템 호출 함수이며 rmdir()은 디렉토리를 삭제할 때 사용하는 시스템 호출 함수이다.
- mkdir()이 성공적으로 수행되면 . 디렉토리와 .. 디렉토리 두 항목을 자동으로 포함한 새로운 디렉토리가 생성된다.
- 빈디렉토리는 rmdir()로 제거할 수 있다. 빈 디렉토리란 `. 디렉토리`와 `.. 디렉토리` 두개의 디렉토리 항목만을 갖는 디렉토리이며 만약 이 함수 호출로 디렉토리의 링크 카운트가 0이 되고 다른 프로세스가 이 디렉토리를 오픈하지 않고 있다면 이 디렉토리가 차지하고 있던 디스크 공간은 새로운 파일이나 디렉토리를 위해 자유공간으로 리턴됨.
- 하나 이상의 프로세스가 이 디렉토리를 오픈하고 있다면 이 프로세스들이 모두 디렉토리를 닫은 후에 해당 디렉토리는 제거되며 해당 디렉토리가 제거된 후에 rmdir()이 리턴됨.

## opendir(),reddir(), rewinddir(), closedir(), telldir(), seekdir()

```c
#include <sys/types.h>
#include <dirent.h>

DIR* opendir(const char* name); // 성공시 포인터 반환

#include <dirent.h>
struct dirent* readdir(DIR* dp); // 성공시 포인터, 디렉토리의 끝일 경우 NULL 반환

int closedir(DIR* dp);

long telldir(DIR* dp);

void seekdir(DIR* dp, long loc);
```

- 디렉토리 내용을 보기 위한 시스템호출 함수이다.
- 디렉토리는 접근권한만 있으면 자유롭게 읽을 수 있다. 그러나 디렉토리에 대한 쓰기 작업은 커널만 할 수 있는데 사용자가 임의로 디렉토리 구조를 수정하는 것을 막기 위함이다.
- DIR구조체는 읽고 싶은 디렉토리 정보를 처리하는데 필요한 시스템 내부 구조체로서 FILE구조체와 비슷한 방법으로 처리된다.
- DIR구조체는 `디렉토리의 포인터를 저장하기 위한 구조체`로서 해당 디렉토리의 값을 가리킨다.
- dirent구조체는 `디렉토리 정보를 포함하고 있는 구조체`로 포인터가 가리키는 디렉토리의 정보를 포함하고 있다.
- 디렉토리 내 파일들은 dirent 구조체로 관리된다.

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>

#define DIRECTORY_SIZE MAXNAMLEN

int main(int argc,char* argv[]){
    struct dirent* dentry;
    struct stat statbuf;
    char filename[DIRECTORY_SIZE+1];
    DIR* dirp;

    if(argc<2){
        fprintf(stderr,"usage:%s <directory>\n",argv[0]);
        exit(1);
    }

    if((dirp=opendir(argv[1]))==NULL || chdir(argv[1])==-1){
        fprintf(stderr,"opendir, chdir error for %s \n",argv[1]);
        exit(1);
    }

    while((dentry=readdir(dirp))!=NULL){
        if(dentry->d_ino==0)
            continue;
        memcpy(filename,dentry->d_name,DIRECTORY_SIZE);
        if(stat(filename,&statbuf)==-1){
            fprintf(stderr,"stat error for %s \n",filename);
            break;
        }

        if((statbuf.st_mode &S_IFMT)==S_IFREG){
            printf("%-14s %ld\n",filename,statbuf.st_size);
        }else{
            printf("%-14s \n",filename);
        }
        exit(0);
    }
}
```

- 인자로 지정한 디렉토리를 읽어 이름과 파일 크기를 출력한다.
  - opendir()호출을 통해 DIR구조체를 얻고, 이 구조체를 readaddir() 인자로 넣어 파일이름을 얻는다.
  - 파일이름이 있다면 stat()호출을 통해 파일 정보를 리턴받을 수 있다.
  - 파일 이름은 경로를 포함하지 않은 파일 이름이기 때문에 opendir()을 호출할때 chdir()도 호출하여 파일이름만 가지고도 접근할 수 있게 작업 디렉토리를 변경한다.
  - 리턴 받은 stat구조체의 파일정보를 가지고 일반 파일인지 확인 후 이름과 크기 출력

## chdir(), fchdir()

```c
#include <unistd.h>

int chdir(const char* pathname);
int fchdir(int filedes);
```

- chdir()과 fchdir()은 현재 작업 디렉토리를 변경하는 시스템호출 함수.
- 모든 프로세스는 현재 디렉토리에 대한 정보를 가진다.
- 이 디렉토리는 루트로 시작하지 않는 모든 상대적 디렉토리 경로의 시작 경로가 된다!
- chdir() : `변경하고자 하는 디렉토리의 경로를 나타내는 문자열`을 인자로 받음
- fchdir() : 디렉토리에 대한 파일 디스크립터를 인자로 받는다.
- chdir()은 지정한 디렉토리에서 많은 파일들을 처리할 필요가 있을때 유용하게 사용
  - 절대경로보다 상대경로가 더 효율

## getcwd(),get_current_dir_name()

- 현재 작업 디렉토리에 대한 `전체 경로 이름`을 얻을 수 있는 시스템호출 함수
- 시스템은 프로세스의 한 속성으로서 현재 작업 디렉토리에 대한 포인터를 가진다.
  - 이 값은 현재 작업 디렉토리의 경로 이름을 나타내는 문자열이 아닌 파일 디스크립터 값.
  - 현재 디렉토리를 알려면 → 파일 디스크립터 값으로부터 부모 디렉토리를 역으로 추적하면서 최종적으로 루트 디렉토리에 도달할 때까지 반복해야함!
  - 이런 수고를 없애기 위해 getcwd()가 사용됨

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define PATH_MAX 1024

int main(void){
    char* pathname;
    if(chdir("/src/lecture2")<0){
        fprintf(stderr,"chdir error \n");
        exit(1);
    }
    pathname = malloc(PATH_MAX);
    if(getcwd(pathname,PATH_MAX)==NULL){
        fprintf(stderr,"getcwd error \n");
        exit(1);
    }
    printf("current directory = %s \n",pathname);
    exit(0);
}
```

- getcwd()를 호출하여 chdir()로 변경한 현재 작업디렉토리를 확인한다.
