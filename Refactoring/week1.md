# 1주차 스터디

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
