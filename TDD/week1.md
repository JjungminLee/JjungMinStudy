# 📌 TDD

### JUnit5 Intro

- java reflection을 사용하면 앞에 굳이 Public을 쓸 필요가 없다.

### 💭 JAVA Reflection이 뭐지?

리플렉션은 힙 영역에 로드된 Class 타입의 객체를 통해, 원하는 클래스의 인스턴스를 생성할 수 있도록 지원하고, 인스턴스의 필드와 메소드를 **접근 제어자와 상관 없이** 사용할 수 있도록 지원하는 API이다.

구체적인 Class Type을 알지 못하더라도 해당 Class의 method, type, variable들에 접근할 수 있도록 해주는 자바 API이며, 컴파일된 바이트 코드를 통해 **Runtime에 동적으로 특정 Class의 정보를 추출할 수 있는 프로그래밍 기법**이다.

**Reflection을 사용하는 곳**

- 동적으로 Class 사용하는 경우
- Test code 작성
- 자동 Mapping 기능 구현**Jackson, GSON 등의 JSON Serialization Library**

[[Java] 리플렉션 (reflection) 개념 이해하기](https://ebabby.tistory.com/4)

- 정민 생각 : 리플렉션이라는게 내가 모르는 클래스의 필드를 조회하거나 생성자를 만들어야할 때 getDeclaredFields()나 getConstructor()같은 일관된 메서드를 통해 조회하거나 만들 수 있게 해주는 기능인건가??!
- 승우 생각 : 라이브러리를 만드는 사람 입장에서 사용자가 어떻게 쓰는지 모르니까 만들어 놓은 클래스 느낌

### @BeforeAll

- 테스트 실행하기 전에 딱 한번 호출
- 반드시 static
- private은 안됨, Default가능
- return 없음

### @AfterAll

- 모든 테스트 실행한 이후 1번

### @BeforeEach

- 테스트 메서드 실행 이전에 수행

### @AfterEach

- 테스트 메서드 실행 이후에 수행

### @Disable

- 테스트를 실행하고 싶지 않을 때
- 깨지는 테스트인경우

## JUnit5 테스트 이름 표시하기

### @DisplayNameGeneration

- 테스트 이름을 설정하는 어노테이션

```tsx
@DisplayNameGeneration(DisplayNameGenerator.ReplaceUnderscores.class)
class Test{

	@Test
	void create_new_test(){

	}

}
```

- create_new_test → create new test로 바꿔줌!

### @DisplayName

- 테스트 이름을 직접 지정
- 권장
- 한글,이모지 가능

## JUnit5 Assertion

### assertEquals

```
@Test
@DisplayName("스터디 만들기")
void create_new_study(){
    Study study=new Study();
assertNotNull(study);
assertEquals("스터디를 처음 만들면 상태값이 Draft여야함",StudyStatus.DRAFT,study.getStatus());
}
```

- message, 기대하는 값, 테스트하려는 값
- 실제 값이 기대한 값과 같은 지 확인
- message는 실행에 실패했을때 나오는 것
- ()→ “ “ 람다로 표현할 수 있다
  - 성능을 신경써야할 때
  - “스터디를 처음 만들 땐”+StudyStatus.Draft+”상태다” 보다 () →”스터디를 처음 만들 땐”+StudyStatus.Draft+”상태다”
  - 이걸 쓰면 테스트가 실패했을 때만 문자열 연산이 실행됨

### assertTrue

- 다음 조건이 참인지 확인

### assertAll

- 테스트를 전부 묶어주는 ?
- n개의 assert문을 동시에 실행
- 몇개의 assert문이 깨졌는지 한번에 알 수 있다

### assertThrows

- 예외처리한 부분을 테스트 하는 방법

```tsx
public class CustomClass {

    private int customInt;

    public CustomClass(int customInt) throws CustomException {
        if(customInt < 0){
            throw new CustomException("CustomException!!!");
        }
        this.customInt = customInt;
    }

}
```

CustomClass에서 예외가 나는지 테스트 할 예정

```tsx
@Test
void test2() {
	CustomException exception = assertThrows(CustomException.class, ()-> new CustomClass(-1));
    String message = exception.getMessage();
    assertEquals("CustomException!!!", message);
}
```

assertThrows메소드의 첫번째 인자로 발생할 예외 클래스의 타입을 받고, 두번째 인자는 어떤 코드를 실행할 때 예외가 발생하는지 테스트 코드를 짜면 된다!

### assertTimeout

- 특정시간안에 코드가 끝나야할 때 테스트 하는 방법

```tsx
assertTimeout(Duration.ofSeconds(10),()->new Study(10));
```

```tsx
assertTimeOut(Duaration.ofMillis(100,()->{
		new Study(10);
		Thread.sleep(300);
	});
}
```

- 이미 테스트가 실패해도 300ms까지 지난 후 테스트 종료하게됨!
- 100ms가 넘으면 테스트를 실패하게 하고 싶은 경우
  - assertTimeOutPreemptively사용!

```tsx
assertTimeOutPreemptively(Duaration.ofMillis(100,()->{
		new Study(10);
		Thread.sleep(300);
	});
}
```

- 하지만 롤백이 안돼서 디비에 반영될 수도 있는 문제를 가지고 있음!

### 💭 Thread Local?

- A 스레드에서 변수 x를 쓰고 있는데 변수 x가 도중에 B스레드로 넘어가서 B스레드의 결과값이 필요할 때 가 있음 → 그럴때 thread local를 쓰는게 아닌지?
- 다른 스레드에서 접근을 하지 못하게 저장소를 두는 것
- 동시성 문제를 피하기 위함.

## 📖 테스트 주도 개발

### 의존성과 중복

- 의존성이 문제 자체라면 중복은 문제의 징후
- 프로그램에서는 중복만 제거해도 의존성은 제거됨

### TDD 주기

- 테스트 작성
- 실행가능하게 만들기
- 올바르게 만들기

### 작동가능한 깔끔한 코드 만들기

1. 작동 가능하게 만들고
2. 깔끔한 코드를 만들기

- 아키텍쳐 주도 개발 : 깔끔한 코드 만들기 → 작동가능하게 만들기

### TDD를 빠르게 통과하는 방법

1. 가짜로 구현하기
   - 상수를 반환하게 만들고 진짜 코드를 얻을 때 까지 단계적으로 상수를 변수로 바꾸어가기
2. 명백한 구현 사용하기
   - 실제구현 입력하기

### Recap

- 두 테스트가 동시에 실패하면 망한다
- 위험요소가 있음에도 계속 진행
- 테스트와 코드 사이의 결합을 낮추기 위해 테스트 하는 객체의 새 기능 사용
  - as-is
  ```tsx
  Dollar five=new Dollar(5);
  Dollar product=five.times(2);
  assertEquals(10,product.amount);
  ```
  - to-be
  ```tsx
  Dollar five=new Dollar(5);
  assertEquals(new Dollar(10),five.times(2));
  ```

### 상향식 테스트 방법, 하향식 테스트 방법

- 상향식 테스트 방법 ⇒ tdd
  - 스텁 (가상의 클라이언트)
- 하향식 테스트 방법 ⇒ postman
  - 드라이버 (우리가 클라이언트 마냥 입력값을 줌 → 가상의 서버를 드라이버라고 함)
    s
