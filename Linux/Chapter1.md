## vi 편집기

- 보통은 vim편집기를 더 많이 쓴다.

```java
sudo apt-get install vim
```

- 나머지 내용은 구글링해보면서 그때그때 쓸 것

## gcc

- 처음에는 C언어만 지원하는 컴파일러였음
- 이후 여러 언어들을 지원하면서 gcc로 변경
- 많은 컴파일러와 운영체제 만드는데 사용

### 전처리

```java
gcc - E A.c -o A.i
```

- 실행문장의 매크로를 상수로 변환해준다.
- -o 옵션 : 바이너리 형식의 출력파일 이름을 지정하는데, 지정하지 않을 시 a.out이라는 기본 이름으로 실행파일 생성.

### 컴파일

```java
gcc -S A.i -o A.s
```

- 전처리된 파일 _.i를 이용해 컴파일러가 어셈블리어 파일 _.s로 변환

### 어셈블

```java
gcc -c A.s -o A.o
```

- 어셈블리어로 된 \*.s파일을 이용하여 이진코드의 오브젝트 파일을 생성
- -c : 소스 파일을 오브젝트 파일로만 컴파일하고 링크하는 과정 생략

### 링크

```java
gcc A.o -o A
```

- 오브젝트 파일은 라이브러리 함수에 해당하는 코드가 없기 때문에 실행될 수 없다.
- 여러 파일로 이루어진 프로그램의 경우에도 오브젝트 파일간 연결이 되지 않은 상태.
- 링커는 `라이브러리함수와 오브젝트 파일들을 연결해` 실행가능한 파일을 생성.

### libc.a라이브러리

- 라이브러리란?
  - /usr/lib에 존재
  - 정적라이브러리와 동적 라이브러리로 나뉨
  - \*.a 파일에 접두사 lib가 붙어야한다
- 정적 라이브러리
  - .a확장자 가짐
  - 여러개의 오브젝트 파일들을 하나로 묶어 사용하기에 Archive파일이라고도 부름
  - 컴파일 시간에 코드를 포함하여 컴파일함.
- 동적 라이브러리
  - 라이브러리만 교체해도 변경이 가능함! (정적 라이브러리는 소스코드가 변경되면 새로 컴파일해야함)
  - /usr/lib에 복사하지 않고 실행하면 라이브러리를 찾을 수 없다는 메세지 출력함.

### gdb

- GNU소프트웨어 시스템을 위한 기본 디버거
- gdb사용을 위해서는 gcc 컴파일 시 -g 옵션을 추가해 컴파일

```jsx
gcc -g -0 ssu_bug ssu_bug.c
```

### ctags

- 소스코드에서 태그의 데이터베이스 파일을 생성하는 명령어
- sudo apt-get install ctags 로 ctags설치 가능
- /usr/include 경로에서 tags파일을 불러올 수 있다.
  - sudo ctags -R

### cscope

- ctags보다 더 자세한 분석이 필요할 때 사용됨.
- ctags는 태그의 정의만 찾는 반면 cscope는 파일 및 함수가 호출된 위치 또는 해당 함수가 호출하는 함수등 상세한 분석이 가능

### make

- 프로젝트 관리 유틸리티
- 컴파일 시간을 단축하고 파일의 종속구조를 빠르게 파악할 수 있게 해줌.
- make을 사용하지 않는다면 → 각각의 소스코드를 각각 컴파일해서 마지막에 모든 오브젝트 파일을 대상으로 한번 더 링킹을 해줘야함
- Makefile의 구성
  - 매크로 정의 부분에는 자주 사용하는 문자열 정의
  - 규칙 정의 부분은 타겟, 종속항목, 명령어로 이루어지는데 타겟에 생성할 파일을 작성.
  - 종속항목에는 타겟을 만들기 위해 필요한 파일의 목록 작성

```jsx
Macro

Target : Dependency1 Dependency2
				Command1
				Command2
```

- 타겟을 만들기 위해 필요한 명령어를 작성할 때는 탭으로 시작!
- 한줄주석은 # 여러줄 주석은 ₩
- ;으로 명령행 나눌 수 있다.

```jsx
ssu_make : ssu_make.c
					gcc -Wall -W ssu_make.c -o ssu_make
clean :
			rm ssu_make
```

- 매크로를 작성했을 경우

  ```jsx
  OBJECTS = ssu_main.o ssu_os.o ssu_esp.o ssu_lsp.o
  TARGT = ssu_prog
  CC = gc
  ```

  ```jsx
  OBJECTS = ssu_main.o ssu_os.o ssu_esp.o ssu_lsp.o
  TARGET = ssu_prog
  CC = gcc
  CFLAGS = -Wall -w

  $(TARGET) : $(OBJECTS)
  		$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)
  ```
