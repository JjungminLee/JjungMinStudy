## truncate(), ftruncate()

```c
#include <unistd.h>
#include <sys/types.h>

int truncate(const char* pathname,off_t length);
int ftruncate(int filedes,off_t length);
```

- 파일을 지정된 길이로 자르는 시스템 호출 함수.
- 인자로 지정된 파일의 길이를 인자로 지정한 length바이트가 되도록 자른다.
- 원래 크기가 length보다 작았을 경우 시스템 마다 다를 수 있으나 대부분의 시스템은 파일의 크기가 늘어남.
- 원래의 EOF에서 늘어난 크기까지 0으로 채운다.

## link(), unlink()

```c
#include <unistd.h>

int link(const char* existingPath, const char* newPath);
int unlink(const char* pathname);
```

- link()는 기존 파일에 대한 링크를 생성하는 시스템 호출 함수
  - 이미 존재하는 파일에 대해서 새로운 링크 생성하고자 할 때
  - 첫번째 인자는 이미 존재하는 파일의 이름, 두번째 인자는 link()로 새로 생성할 파일의 이름
  - 두번째 인자 newPath가 이미 존재한다면 에러 리턴
  - 하나의 파일은 하나의 inode와 매핑
    - inode는 구조체로 구성 , 파일의 모든 정보 포함
    - inode의 정보들 중 링크 카운트가 있음 → 해당 파일이 가리키는 inode와의 링크 수 의미
    - 리눅스에서는 디스크 상의 동일한 파일에 대해서 서로 다른 이름으로 접근 가능
      - 동일한 inode번호 갖는 다수의 파일이 있음! (각각의 파일들은 동일한 링크 카운트값 가짐)
  - link()에 의해서 생성된 링크 파일은 이름만 다를 뿐 link()호출 시 첫번째 인자로 지정한 파일과 동일한 inode번호를 가지고 되고, 어느 한 파일 이름을 사용해서 파일의 내용을 수정하면 동일한 inode번호 가진 모든 링크 파일들도 동일하게 파일 내용이 수정됨
- unlink()는 기존 디렉토리 항목의 링크 제거하는 시스템 호출 함수

## 링크 파일을 생성한다 ≠ 파일복사한다.

- 파일을 복사하면 디스크 상에 두개의 서로 다른 파일이 생기고 다른 inode를 갖게 된다.
- 복사한 파일 중 어느 하나의 파일 내용을 변경하더라도 다른쪽 파일에는 영향을 주지 않는다.
- 링크는 파일을 중복해서 저장하지 않아도 되기 때문에 디스크 공간을 절약할 수 있고 여러 사용자가 같은 파일을 쉽게 사용 가능
- link()는 일반 파일에만 사용하며, 디렉토리에 대한 link()호출은 루트권한이 있어야 가능
- 리눅스에서는 다른 종류의 여러개 파일 시스템을 사용할 수 있는데 하나의 디스크를 몇개의 파티션으로 나눈 후 각각의 파티션에 별도의 파일 시스템을 사용할 수 있고, 다른 종류의 파일 시스템을 사용하는 디스크를 동시에 여러개 사용할 수도 있다.
  - 여러개 파일 시스템 사용하려면 → 시스템 부팅시 mount작업
- 사용중인 여러 파일 시스템 중에서 특정 파일 시스템이 언마운트 되어 사용할 수 없는 상황에서 언마운트 된 파일 시스템 내에 포함된 파일에 링크된 파일 이름을 사용하는 것은 불가능
- **리눅스에서는 서로 다른 파일 시스템간의 링크는 허용하지 않으며 주어진 두개의 인자는 반드시 하나의 파일 시스템 내에서 링크될 파일들이어야 한다.**
- unlink()는 link()와 반대로 파일의 링크 카운트 하나 감소시킨다.
  - unlink()에서 인자로 지정한 파일 이름으로는 해당 파일에 접근할 수 없다
  - 이전에 링크 카운트가 2 이상이였다면, 링크 카운트가 최소한 1이상이기 때문에 동일한 inode번호 갖는 다른 파일 이름으로 디스크 상에 접근 가능
  - 링크 카운트가 0이라면 해당 파일이 사용하고 있던 디스크 공간은 해제되어 자유영역으로 리턴되고 모든 데이터는 접근 불가능 상태
  - 파일 inode번호도 자유 inode리스트에 넣어지게 되어서 새로운 파일 생성시에 사용 가능

## 레퍼런스 카운트와 링크 카운트의 차이점

- 레퍼런스 카운트 : 메모리의 파일시스템 복사본 내에 존재하면서 현재 해당 파일을 사용하고 있는 프로세스가 몇개인지 나타냄
- 링크 카운트 : 파일 시스템내에서 동일한 inode번호 갖는 파일이름(디렉토리 엔트리)이 몇개나 있는지 나타냄.
- 리눅스는 다중사용자 환경을 지원하는 시스템이기 때문에 디스크에서 특정 파일을 위해 사용되고 있던 공간이 해제되는 것은 링크카운트가 0인시점이 아닌 레퍼런스 카운트가 0인 시점

- link_exercise.c

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(int argc,char* argv[]){
    if(argc<3){
        fprintf(stderr,"usage:%s <file1> <file2>\n",argv[0]);
    }
    if(link(argv[1],argv[2])==-1){
        fprintf(stderr,"link error for %s\n",argv[1]);
        exit(1);
    }
    exit(0);

}
```

- link_file.c

```c
#include <stdio.h>
#include <stdlib.h>

int main(void){
    printf("This is link file\n");
    exit(0);
}
```

- gcc -o link_file link_file.c
- gcc link_exercis.c → ./a.out이 나옴
- ./a.out link_file eoslab
- ls -l link_file eoslab
- 결과값
  ❯ ls -l link_file eoslab
  -rwxr-xr-x 2 jungmin staff 33464 2 6 11:37 eoslab
  -rwxr-xr-x 2 jungmin staff 33464 2 6 11:37 link_file
  ❯ ./eoslab
  This is link file
- link를 호출해 첫번째 인자로 지정한 파일과 같은 파일을 두번째 인자로 지정한 이름으로 새로운 파일을 생성한다.
- cp명령어를 사용하여 복사한 파일과 다르게 동일한 디스크 공간을 가리킨다.

## remove(), rename()

```c
#include <stdio.h>

int remove(const char* pathname);
int rename(const char* oldname,const char* newname);
```

- remove()
  - 파일이나 디렉토리를 unlink시키는 시스템호출 함수
  - unlink()의 경우 루트권한이 있어야만 디렉토리를 인자로 받을 수 있으나 remove()는 파일과 디렉토리 모두 인자로 받기 가능!
  - 파일의 경우 → unlink()와 비슷
  - 디렉토리의 경우 rmdir()과 비슷
- rename()
  - 이름 변경할 때 호출하는 시스템호출 함수
  - 변경대상이 파일인 경우 이미 존재하는 파일이름을 두번째 인자로 넘길경우는 불가능 삭제하고 해야한다.
  - 디렉토리도 이미 존재하는 이름인 경우 빈 디렉토리면 이름이 변경되고 그렇지 않으면 호출 실패
  - 대상 파일이 심볼릭 링크인 경우 심볼릭 링크 파일 자체의 이름으로 변경되고 두 인자가 같을 경우 아무것도 변경하지 않고 성공적으로 리턴

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(int argc,char* argv[]){
    if(argc!=3){
        fprintf(stderr,"usage:%s <oldname> <newname>\n",argv[0]);
    }
    if(link(argv[1],argv[2])<0){
        fprintf(stderr,"link error \n");
        exit(1);
    }else{
        printf("step1 passed.\n");
    }

    if(remove(argv[1])<0){
        fprintf(stderr,"remove error\n");
        remove(argv[2]);
        exit(1);
    }else{
        printf("step2 passed.\n");
    }
    printf("Success! \n");
    exit(0);

}
```

- 실행결과
  ❯ ./a.out remove_text.txt ssu_text.txt
  step1 passed.
  step2 passed.
  Success!
  - remove_text.txt가 ssu_text.txt로 변경된다.
- link()와 remove()를 통해 파일의 이름을 변경한다. 인자로 지정한 파일을 link()를 통해 새로운 파일을 생성하고 remove()는 unlink()와 동일하게 동작하기 때문에 파일은 삭제되지만 그 파일의 디스크 공간은 해제되지 않고 link()로 생성한 파일이 그 디스크 공간을 가리킨다.

## symlink(), readlink()

```c
#include <unistd.h>

int symlink(const char* actualpath, const char* sympath);
ssize_t readlink(const char* pathname, const* char buf, size_t bufsize);
// 성공시 읽은 바이트 수 리턴
```

- symlink()
  - 심볼릭 링크 생성하는 시스템호출 함수
- readlink()
  - 심볼릭 링크 읽는 시스템호출 함수
- 심볼릭링크란?
  - link()를 통해 만들어진 하드 링크의 한계를 해결하기 위해 만들어졌다.
  - 디스크상의 inode를 가리키는 파일이 아닌 파일 자체를 가리키는 파일
  - 바로가기 기능과 동일
- 하드링크는 링크와 해당 파일이 동일한 파일 시스템에 존재해야함
- 심볼릭 링크는 서로 다른 파일 시스템의 파일과 링크가 가능
- 첫번째 인자로 지정한 `파일은 존재하지 않아도 되는데` 심볼릭 링크가 대상 파일의 경로 이름만을 자신의 데이터로 가질 뿐 대상 파일의 데이터와는 무관하기 때문
- 새로 만들어지는 심볼릭링크 이름이 두번째 인자여야 한다!
- readlink()는 `링크로 연결된 파일 이름을 읽을 수` 있다. buf인자에 저장된 문자열의 끝이 NULL로 끝나지 않는다.

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(int argc,char* argv[]){
    if(argc!=3){
        fprintf(stderr,"usage:%s <actualname> <symname>\n",argv[0]);
    }
    if(symlink(argv[1],argv[2])<0){
        fprintf(stderr,"symlink error \n");
        exit(1);
    }else{
        printf("symlink: %s -> %s\n",argv[2],argv[1]);
    }
    exit(0);

}
```

- 실행결과
  ❯ gcc symlink_exercise.c
  ❯ ./a.out ./link_file ./symlink_file
  symlink: ./symlink_file -> ./link_file
- symlink()호출을 통해 인자로 지정한 파일의 심볼릭 링크 파일을 생성한다!
- 추가 a.out말고 이름 지정해주고 싶으면
  gcc 파일 -o 파일명
