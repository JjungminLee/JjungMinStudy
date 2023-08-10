## JUnit5 Intro

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
