# 📌 Refactoring

<aside>
💡 냄새 1 : 이해하기 힘든 이름

</aside>

- 깔끔한 코드에서 가장 중요한 것 중 하나가 “좋은 이름”
- 함수, 변수, 클래스 , 모듈의 이름 중 모두 어떤 역할을 하는지 어떻게 쓰이는지 직관적이여야함.
- 리팩토링 기술
  - 함수 선언 변경하기
  - 변수 이름 바꾸기
  - 필드 이름 바꾸기

### 함수 선언 변경하기

- 함수 이름 변경, 메소드 이름 변경, 매개변수 추가, 매개변수 제거, 시그니처 변경하기
- 좋은 이름을 가진 함수 → 어떻게 구현되어있는지 코드를 보지 않아도 이름만 보고 이해할 수 있다.
- 주석을 작성한 다음 → 주석을 함수 이름으로 만들기
- 함수의 매개변수
  → 내부의 문맥 결정
  - 전화번호 포멧팅 함수
    - 전화번호 string만 ⇒ 또 다른 전화번호를 포멧팅할 때 재사용 가능
    - Person 객체 ⇒ 국가 타입별로 전화번호를 포멧팅하는것 가능
    - 정답이 있는게 아니다!
  - 의존성 결정
    - Payment 만기일 계산 함수
      - Payment만 넘길지, 만기일만 넘길지 → 정답이 있는게 아님!
      ### 💭 질문 : Payment를 모두 넘긴다면 의존성이 높은건가?
      서비스에서 매개변수를 다 입력을 받아야하는 경우 → 의존성이 높다고 할 수 없다.
- 실습
  - 깃허브 스터디 리뷰 이슈에 작성되어 있는 리뷰어 목록과 리뷰를 읽어오는 함수
    ```tsx
    private void studyReviews(GHIssue ghIssue){}
    ```
  - studyReviews라는 함수명이 적합할까?
    - 주석으로 함수의 기능을 적어본다!
    - intellij → refactor → rename (메서드 이름 변경)
    - loadReview로 개선
    - 리뷰가 있는 이슈는 오직 한개 → 파라미터가 필요할까?
    - refactor→ change signature (매개변수 변경)

### 변수 이름 바꾸기

- 더 많이 사용되는 변수일수록 그 이름이 더 중요하다
- 다이나믹 타입을 지원하는 언어에서는 타입을 이름에 넣기도 한다!
- 여러함수에 걸쳐 쓰이는 필드 이름에는 더 고민을 해야한다!
- 실습
  1. 람다식
  ```tsx
  studyDashboard.getUsernames().forEach(name -> System.out.println(name));
  ```
  name 대신 n으로 쓰기도 한다
  ### **💭 보통 람다에서 어떻게 쓰는지?**
  - 정민 : 저렇게 name을 많이 쓰고 js할때는 Item을 쓰는 편!
  ```tsx
  studyDashboard.getUsernames().forEach(System.out::println);
  ```
  이런식으로 쓰기도 함!
  1. 리뷰에 달린 코멘트
  ```tsx
  List<GHIssueComment> comments = issue.getComments();
          for (GHIssueComment comment : comments) {
              usernames.add(comment.getUserName());
              this.reviews.add(comment.getBody());
          }
  }
  ```
  - 리뷰에 달린 코멘트인데 comments가 적당할까? → reviews가 더 적당함
  ### 💭 00list 이런식으로 쓰기도 하는데 이렇게 변수명을 적용하는게 맞을까?

### 필드 이름 바꾸기

- Record 자료구조의 이름은 프로그램 전반에 참조될 수 있기에 매우 중요!
  - 파이썬의 딕셔너리
  - 자바의 record 키워드 → 14버전 부터 있다!
  - 타입스크립트도 Record가 있다! Record<key타입, 속성 타입>
- 자바의 record

  - Record는 enum과 같은 특별한 형태의 class. 주로 간단하게 데이터를 저장하고 옮기는 역할로 쓰임.
  - dto쓸 때 쓰면 좋음!
  - immutable하다!

  ```tsx
  private Set<StudyReview> studyReviews = new HashSet<>();

  // main 메서드
  studyDashboard.getStudyReviews().forEach(System.out::println);

  // record
  public record StudyReview(String reviewer,String review){}
  ```

  - 객체지향에서 최지웅 교수님이 객체 하면 생각해야하는 메서드
    - toString, equals, hashcode(객체지향 세상에서 객체가 제구실을 하기 위해 필요한 메서드) ⇒ 자동으로 재공
  - getter도 자동으로?
    - getter는 field이름과 유사한 이름으로 생성됩니다 ex) id(), email()..
      ⇒ wow!

<aside>
💡 냄새 2 : 중복 코드

</aside>

- 중복코드의 단점
  - 비슷한지, 완전히 동일한 코드인지 주의 깊게 봐야함
  - 코드를 변경할 때 → 동일한 모든 곳의 코드를 변경해야함
- 사용할 수 있는 리팩토링 기술
  - 동일한 코드를 여러 메서드에서 사용하는 경우 → 함수 추출하기
  - 코드가 비슷하게 생겼지만 완전히 같지는 않은 경우 → 코드 분리하기
  - 여러 하위 클래스에 동일한 코드가 있다면 → 메서드 올리기

### 함수 추출하기

- 의도와 구현 분리하기
- 무슨일을 하는 코드인지 알아내려고 노력해야하는 코드라면 → **구현을 드러내고 있다!**
- 코드가 어떤 일을 하는지 잘 알 수 있다면 → **의도를 드러내고 있다!**
- 각각의 메서드 안에 있는 주석을 메서드 이름으로 추출하면 → 좀더 좋은 코드가 될 것
- 인텔리제이에서 **command + opt + m 또는 refactor → extract method**

```tsx
GitHub gitHub = GitHub.connect();
GHRepository repository = gitHub.getRepository("whiteship/live-study");
GHIssue issue = repository.getIssue(eventId);
```

- 이 부분이 중복되고 있음 → getGhIssue(Long issue) 로 함수를 추출해줌
- 최대한 함수를 빼서 중복과, 주석을 없앤다!

### 코드 정리하기

[Slide Statements](https://refactoring.com/catalog/slideStatements.html)

- 관련있는 코드끼리 묶어있어야 코드를 더 쉽게 이해할 수 있다
- 함수에서 사용할 변수를 상단에 미리 정의하기 보다는 해당 변수를 사용하는 코드 바로 위에 선언하자
  - 한뭉탱이로 볼 수 있다!
  - 이렇게 하지 않으면 중복코드를 찾아내는게 어려울 수도 있다!
- 관련있는 코드끼리 묶은 다음 함수 추출하기를 사용해서 더 깔끔하게 분리 가능
- 이거 단축키가 뭐였지?

### 메서드 올리기

- 중복코드는 당장은 잘 동작해도 미래에 버그를 만들어내는 빌미 제공
- 여러 하위 클래스에 동일한 코드가 있다면 손쉽게 적용 가능
- 비슷하지만 일부 값만 다른 경우 → **함수 매개변수화 하기** 리팩토링 적용 후 이 방법 사용 가능
- 하위 클래스에 있는 코드가 상위 클래스가 아닌 하위 클래스 기능에 의존하고 있다면 **필드 올리기**를 적용한 이후 이 방법 적용 가능

  - 하위 클래스에 똑같이 사용되는 중복코드를 상위 클래스로 올려버리기 (pull up method / pull members up)
  - reviewers와 participants에 중복으로 사용되는 아래 코드를 printUserNames로 바꿔 부모 클래스인 Dashboard로 올려준다!

    ```tsx
    public void printUserNames(int eventId) throws IOException {
            // Get github issue to check homework
            GitHub gitHub = GitHub.connect();
            GHRepository repository = gitHub.getRepository("whiteship/live-study");
            GHIssue issue = repository.getIssue(eventId);

            Set<String> userNames = new HashSet<>();
            issue.getComments().forEach(c -> userNames.add(c.getUserName()));


    	      userNames.forEach(System.out::println);
        }
    ```

- 두 메서드가 비슷한 절차 따르고 있다면 **템플릿 메서드 패턴** 적용 고려해볼 수 있음 (디자인패턴)

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
