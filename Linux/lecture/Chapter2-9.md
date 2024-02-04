## write()

```c
#include <unistd.h>
ssize_t write(int filedes,const void* buf,size_t nbytes);
// 리턴 : 성공시 기록된 바이트 수, 에러시 -1리턴
```

- 오픈한 파일에 데이터를 쓸 때 사용하는 시스템 호출 함수
- 지정된 메모리로 부터 쓰기 전용 또는 읽기,쓰기 혼용 접근 권한으로 오픈된 파일로 정해진 바이만큼의 데이터 기록
- 파일을 오픈할 때 O_APPEND 옵션을 사용하면 매번 write()가 수행될때마다 데이터를 쓰기 전에 파일의 오프셋 위치를 파일 끝으로 이동시킨다.
- 리턴값은 파일에 실제로 쓴 데이터의 바이트수가 된다.
- read()와 달리 쓰기를 요청한 바이트보다 실제로 쓴 바이트가 더 적은 값이 될수는 없다.
  - 두 값이 일치하지 않으면 어디선가 에러가 발생한것
- 일관성을 유지하기 위해 write()가 수행되는 동안 해당파일의 i-node에 락을 걸고 해당 파일에 접근 하려는 다른 모든 연산들을 블록 시킴

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define BUFFER_SIZE 1024

int main(void){

    char buf[BUFFER_SIZE];
    int length;
    length = read(0,buf,BUFFER_SIZE);
    write(1,buf,length);
    exit(0);
}
```

- 위의 예제는 표준입력에서 read()를 호출하여 데이터를 읽는다
- 읽은 데이터의 양을 read()의 리턴값으로 알 수 있으며, 이 리턴값을 사용하여 읽은 만큼의 데이터를 write()를 통해 표준 출력에 쓴다.

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#define S_MODE 0644
#define BUFFER_SIZE 1024

int main(int argc,char* argv[]){

    char buf[BUFFER_SIZE];
    int fd1,fd2;
    int length;

    if(argc!=3){
        fprintf(stderr,"Usage : %s filein fileout \n",argv[0]);
        exit(1);
    }

    if((fd1=open(argv[1],O_RDONLY))<0){
        fprintf(stderr,"open error for %s \n",argv[1]);
        exit(1);
    }
    if((fd2=open(argv[2],O_WRONLY | O_CREAT | O_TRUNC, S_MODE))<0){
        fprintf(stderr,"open error for %s \n",argv[2]);
        exit(1);
    }
    while((length = read(fd1,buf,BUFFER_SIZE))>0){
        write(fd2,buf,length);
    }
    exit(0);
}
```

- 개념 내용 정리
  - argc란 메인함수에 전달 되는 데이터의 개수, argv란 메인함수에 실제적으로 전달되는 데이터로 char형 포인터형 배열로 이루어져 있음
  - `fcntl.h` : 리눅스 시스템에서는 열려진 파일의 속성을 가져오거나 설정할 때 사용
- 위 예제는 두개의 파일을 인자로 입력받아 첫번째 파일의 내용을 두번째 파일에 복사한다.
- open()을 통해 두개의 파일을 오픈하고 read()를 통해 첫번째 파일에서 문자열을 읽어 write()를 통해 출력할 파일에 쓴다.

## pread(), pwrite()

```c
#include <unistd.h>
ssize_t pread(int filedes,const void* buf,size_t nbytes, off_t offset);
// 리턴 : 성공시 기록된 바이트 수, 에러시 -1리턴

ssize_t pwrite(int filedes,const void* buf,size_t nbytes, off_t offset);
```

- 오픈된 파일에 파일 탐색과 입출력 연산을 원자적으로 수행할 때 사용하는 시스템 호출 함수
- pread()는 filedes의 offset인자 위치에 있는 데이터를 nbytes 인자 만큼 읽어 buf인자에 저장.
- pwrite()는 filedes의 buf 인자에 들어있는 데이터를 filedes의 파일 위치인 offset인자에 nbytes 바이트만큼 쓴다.
- pread()는 lseek()와 read()기능을 동시에 수행하는 함수이며, pwrite()는 lseek()와 write()의 기능을 동시에 수행하는 함수
- read(),write()와 다른점
  - pread()와 pwrite()를 호출해도 파일 오프셋의 위치가 변경되지 않는다!
  - 원자적 연산을 지원하는 시스템호출 함수라고 한다!
  - 멀티 쓰레드 환경에서 동일한 파일을 여러개의 쓰레드가 동시에 읽고 쓰더라도 `오프셋 위치에서 읽기 때문에` 다른 쓰레드의 읽기,쓰기 연산 결과에 영향을 받지 않아 매우 유용하게 사용됨

## dup()

```c
#include <unistd.h>
int dup(int filedes);
int dup(int filedes,int filedes2);
// 리턴값: 성공시 새로운 파일디스크립터
```

- dup()와 dup2()는 기존 `파일 디스크립터를 복사할때` 사용하는 시스템호출 함수이다.
- dup()와 dup2() 호출이 성공되면 파일 디스크립터 테이블 상에서 현재 사용되지 않는 가장 적은 값의 새로운 파일 디스크립터 리턴. → 리턴값이 파일디스크립터 테이블 상의 가장 작은 미사용 항목이라는 것을 보장해줌
- dup()을 통해 리턴받은 새로운 파일디스크립터와 인자로 저장한 파일 디스크립터는 파일 테이블 상에서 동일한 하나의 항을 공유하게 된다.
- dup()에 의한 파일 디스크립터 복사는 같은 파일을 서로 다른 파일 디스크립터에 의해 접근하여 `파일을 공유`할 수 있게 하는 역할
- 원본 파일 디스크립터를 사용하여 파일 오프셋을 변경시키면 복사된 파일 디스크립터를 이용한 읽기,쓰기 시에도 변경된 파일 오프셋이 적용된다.
- dup()에 의해 복사된 파일 디스크립터가 원본 디스크립터와 정확하게 일치하는 것은 아님!
- 파일 디스크립터 테이블의 각 항은 파일 테이블에 대한 포인터 외에 파일 디스크립터 플래그를 가지고 있음.
  - dup() 호출 시 그대로 복사되지 않고 초기화 됨
  - filedes 인자는 반드시 현재 사용중인 파일디스크립터가 지정되어야 하며 그렇지 않은 경우 에러 발생시킴.
- 리턴될 파일 디스크립터의 값을 미리 지정하고자 할 때는 dup2()를 호출
  - 인자로 지정한 filedes2가 이미 사용중인 파일 디스크립터라면 그것은 우선 닫고 다시 오픈하며 filedes 인자가 가리키는 파일 테이블의 항을 가리키게 됨.
  - filedes와 filedes2가 같은 값이면 이때는 아무런 복사가 일어나지 않고 단순히 파일디스크립터를 리턴

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
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
    fd2 = dup(fd1);// dup 함수는 파일 디스크립터를 복제하는 함수입니다. fd1이 가리키는 파일을 나타내는 새로운 파일 디스크립터 fd2가 생성됩니다.
    count = read(fd1,buf,12); //read 함수를 사용하여 파일 디스크립터 fd1이 가리키는 파일에서 최대 12바이트의 데이터를 읽어와서 buf 배열에 저장합니다.
    buf[count] = 0; //읽어온 데이터의 마지막에 null 문자('\0')를 추가하여 문자열을 만듭니다. 이렇게 함으로써 나중에 printf 함수로 문자열을 출력할 때, 올바른 문자열을 출력할 수 있습니다.
    printf("fd1's printf : %s\n",buf);
    lseek(fd1,1,SEEK_CUR); //lseek 함수를 사용하여 fd1이 가리키는 파일에서 현재 위치를 1바이트만큼 이동시킵니다.
    count = read(fd2,buf,12); //이제 fd2를 이용하여 파일에서 데이터를 읽어옵니다. fd2와 fd1은 같은 파일을 가리키기 때문에, fd1에서의 lseek로 인해 fd2에서도 현재 위치가 이동되어 있습니다. 다시 read 함수를 사용하여 최대 12바이트의 데이터를 읽어옵니다.
    buf[count] = 0; //읽어온 데이터의 마지막에 null 문자를 추가하여 문자열을 만듭니다.
    printf("fd2's printf : %s\n",buf);
    exit(0);
}
```

- 결과 예시
  fd1's printf : Hello World!
  fd2's printf : Hello Kathy!
- dup()은 dup_exercise.txt 파일을 open() 호출을 통해 리턴 받은 파일 디스크립터를 dup()를 호출하여 복사한다.
- dup()를 호출하여 복사한 파일 디스크립터는 원본이 되는 파일 디스크립터와 오프셋을 공유하기 때문에 fd2에서 읽은 데이터는 fd1에서 읽은 데이터의 뒷부분이 된다.
- dup 한후 단순히 write하면 파일에 쓰는거고 이걸 콘솔에 불러오고 싶다면?
  - while문 돌리면서 read하고 + fprintf또는 putchar해준다!

## fsync(), fdatasync()

```c
#include <unistd.h>
int fsync(int filedes);
int fdatasync(int filedes);

void sync(void);
```

- sync(), fsync(), fdatasync()는 디스크 상의 파일 시스템과 버퍼 캐쉬의 내용간의 일관성을 보장하기 위해 사용하는 시스템 호출 함수.
- sync()와 fsync()를 통해 버퍼 캐쉬의 내용을 디스크에 쓴다
  - sync(): 변경된 모든내용을 디스크에 씀 , 디스크 입출력이 완료될 때 까지 대기하지 않고 곧바로 리턴
  - fsync() : 인자로 지정한 파일의 변경된 내용만 디스크에 씀, 입출력이 완료된 후에 리턴
- 리눅스, 유닉스에서는 디스크 입출력 시 주기억장치 내에 버퍼 캐쉬라고 불리는 임시 버퍼를 사용함으로써 시스템 효율을 높이는 기법을 쓴다.
- write호출시 바로 디스크에 쓰는 것이 아닌 일정한 양을 블록 크기로 모아 나중에 한번에 디스크로 쓴다.
- read()도 호출할 때 마다 디스크로부터 읽는 것이 아닌 최초에 read() 호출할 때 원하는 데이터 포함한 데이터 블록을 주기억장치 내 버퍼 캐쉬에서 읽고 다음번 read() 호출에 대해서도 버퍼 캐쉬로 부터 먼저 읽는다.
- 전원이 꺼질 때를 대비하여 버퍼 캐쉬 내용과 디스크 내용을 일치 시켜야 하는데 리눅스 커널 프로세스 중 update라는 프로세스는 버퍼 캐쉬의 데이터를 디스크 블록에 써 디스크 블록 내용을 갱신하는 역할을 수행
- fsync() 사용하면 버퍼 캐쉬 전체가 아닌 개별 파일을 대상으로 디스크 입출력을 수행할 수 있기에 시스템 효율을 저하시키지 않으면서 원하는 목적 얻기 가능!

## fsync(), fdatasync()

```c
#include <unistd.h>
int fsync(int filedes);
int fdatasync(int filedes);

void sync(void);
```

- sync(), fsync(), fdatasync()는 디스크 상의 파일 시스템과 버퍼 캐쉬의 내용간의 일관성을 보장하기 위해 사용하는 시스템 호출 함수.
- sync()와 fsync()를 통해 버퍼 캐쉬의 내용을 디스크에 쓴다
  - sync(): 변경된 모든내용을 디스크에 씀 , 디스크 입출력이 완료될 때 까지 대기하지 않고 곧바로 리턴
  - fsync() : 인자로 지정한 파일의 변경된 내용만 디스크에 씀, 입출력이 완료된 후에 리턴
- 리눅스, 유닉스에서는 디스크 입출력 시 주기억장치 내에 버퍼 캐쉬라고 불리는 임시 버퍼를 사용함으로써 시스템 효율을 높이는 기법을 쓴다.
- write호출시 바로 디스크에 쓰는 것이 아닌 일정한 양을 블록 크기로 모아 나중에 한번에 디스크로 쓴다.
- read()도 호출할 때 마다 디스크로부터 읽는 것이 아닌 최초에 read() 호출할 때 원하는 데이터 포함한 데이터 블록을 주기억장치 내 버퍼 캐쉬에서 읽고 다음번 read() 호출에 대해서도 버퍼 캐쉬로 부터 먼저 읽는다.
- 전원이 꺼질 때를 대비하여 버퍼 캐쉬 내용과 디스크 내용을 일치 시켜야 하는데 리눅스 커널 프로세스 중 update라는 프로세스는 버퍼 캐쉬의 데이터를 디스크 블록에 써 디스크 블록 내용을 갱신하는 역할을 수행
- fsync() 사용하면 버퍼 캐쉬 전체가 아닌 개별 파일을 대상으로 디스크 입출력을 수행할 수 있기에 시스템 효율을 저하시키지 않으면서 원하는 목적 얻기 가능!
