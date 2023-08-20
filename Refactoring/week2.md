# 2주차 스터디

<aside>
🙀 냄새 3 : 긴 함수

</aside>

### 짧은함수 vs 긴함수

- 코드 자체가 의미만을 나타낸다면 → 긴함수라 볼 수 없다
- 작은 함수에 좋은 이름을 사용했다면 → 해당 함수의 코드를 보지 않고도 이해할 수 있다
- 어떤 코드에 주석을 남기고 싶다면 → 주석 대신 함수를 만들고 함수의 이름으로 의도를 표현해볼 것

### 사용할 수 있는 리팩토링 기술

- **함수 추출하기**
  - 주석으로 할 수 있음
- 긴 함수를 처리하는 방법
  - **매개변수가 많다면?**
    - 임시변수를 질의 함수로
    - 매개변수 객체 바꾸기
    - 객체 통째로 넘기기
  - **조건문이 많아진다면?**
    - 조건문 분해하기
    - switch 여러개? → 조건문 다형성으로 바꾸기
  - **반복문에서 여러 작업을 한다면?**
    - 반복문 쪼개기

### 임시 변수를 질의 함수로 바꾸기

- 변수를 사용하면 동일한 식을 피할 수 있고 이름을 사용해 의미를 표현할 수 있다
- 매개변수가 3개 이상 넘어가면 많다고 생각함
- 실습율
  - 참석율을 계산하는 함수
    ```tsx
    participants.forEach(p -> {
                    long count = p.homework().values().stream()
                            .filter(v -> v == true)
                            .count();
                    double rate = count * 100 / totalNumberOfEvents;

                    String markdownForHomework = String.format("| %s %s | %.2f%% |\n", p.username(), checkMark(p, totalNumberOfEvents), rate);
                    writer.print(markdownForHomework);
    });
    ```
    `String markdownForHomework = String.format("| %s %s | %.2f%% |\n", p.username(), checkMark(p, totalNumberOfEvents), rate);`
    - 이 부분을 더 줄여볼 수 있지 않을까?
      - 계산하거나 format하는 것들을 다 함수로 빼준다!
    ```tsx
    participants.forEach(p -> {
                    String markdownForHomework = getMarkdownForParticipant(totalNumberOfEvents, p);
                    writer.print(markdownForHomework);
     });
    ```
    ```tsx
    private String getMarkdownForParticipant(int totalNumberOfEvents, Participant p) {
            return String.format("| %s %s | %.2f%% |\n", p.username(), checkMark(p, totalNumberOfEvents), getRate(totalNumberOfEvents, p));
    }
    ```
    ```tsx
    private double getRate(int totalNumberOfEvents, Participant p) {
            long count = p.homework().values().stream()
                    .filter(v -> v == true)
                    .count();
            double rate = count * 100 / totalNumberOfEvents;
            return rate;
        }
    ```
    - `String markdownForHomework = getMarkdownForParticipant(totalNumberOfEvents, p);`
      - 여기서 rate도 매개변수에 있었는데 임시변수로 빼내면서 파라미터를 줄였다!

### 매개변수 객체 만들기

- 같은 매개변수들이 여러 메소드에 걸쳐 나타난다면 그 매개변수들을 묶은 자료구조를 만들 수 있다.
  - 자료구조는
    - 해당 데이터간의 관계를 보다 명시적으로 나타낼 수 있다
    - 함수에 전달할 매개변수 개수를 줄일 수 있다.
    - 도메인을 이해하는데 중요한 역할을 하는 클래스로 발전할 수도 있다.
- 실습
  ```tsx
  int totalNumberOfEvents, Participant p
  ```
  - 두가지 매개변수가 계속 함수에 들어가는 것을 알 수 있다.
  - Record로 리펙토링 해보자!
    - introduceParameterObject → Record로 만들어줌
      - 근데 undo함?!
    - totalNumberOfEvents를 필드로 올려버린다!
      ```tsx
      double getRate(Map<Integer, Boolean> homework) {
              long count = homework.values().stream()
                      .filter(v -> v == true)
                      .count();
              return (double) (count * 100 / this.totalNumberOfEvents);
          }
      ```
      - this를 씀으로써 필드라는 것을 명시적으로 표시

### 객체 통째로 넘기기

- 어떤 한 레코드에서 구할 수 있는 여러 값들을 함수에 전달하는 경우, 해당 매개변수를 레코드 하나로 교체 가능
- 매개변수 목록을 줄일 수 있다(향후 추가할지도 모를 매개변수 까지도!)
- 의존성을 고려해야함!
- 해당 메소드의 위치가 적절하지 않을 수도 있다
- 실습
  ```tsx
  participants.forEach(p -> {
                  String markdownForHomework = getMarkdownForParticipant(x);
                  writer.print(markdownForHomework);
              });
  ```
  - `totalNumberOfEvents, p` → 이 부분을 아예 Participant로 전달할 수도 있다
  - 파라미터를 줄일 수 있다는 장점
  - 질문 : 근데 Participant 하나로 줄이면 다른 도메인에서 쓸 수 있을까?
    - **💭 이게 도메인 하나에서만 쓰인다면 의존성이 생기는건가?**
    - **💭 매개변수에 객체가 들어간다고 그게 의존성이 생긴다고 볼 수 있는건가??**

### 함수를 명령으로 바꾸기

- 함수를 독립적인 객체인 Command로 만들어 사용 가능
- 커맨드 패턴을 사용하면?
  - 부가적인 기능으로 Undor기능을 만들 수 있다.
  - 더 복잡한 기능을 구현하는데 필요한 여러 메소드 추가 가능
  - 상속이나 템플릿 활용 가능
  - 복잡한 메소드를 여러 메소드나 필드를 활용해 쪼개기 가능
- 대부분의 경우에 커멘드 보다는 함수 사용
- 실습
  명령부분을 새로운 클래스에 execute라는 함수로 뺀다
  ```tsx
  public void execute() throws IOException {
          try (FileWriter fileWriter = new FileWriter("participants.md");
               PrintWriter writer = new PrintWriter(fileWriter)) {
              this.participants.sort(Comparator.comparing(Participant::username));

              writer.print(header(this.participants.size()));

              this.participants.forEach(p -> {
                  String markdownForHomework = getMarkdownForParticipant(p);
                  writer.print(markdownForHomework);
              });
          }
      }
  ```
  - **💭 명령 관련 부분을 아예 클래스로 빼라는건가? StudyPrinter라는 클래스? 아니면 StudyPrinter에 execute라는 메서드를**

```tsx
new StudyPrinter(this.totalNumberOfEvents, participants).execute();
```

- 이렇게 객체를 불러옴으로써 코드를 정리

### 조건문 분해하기

- 여러 조건에 따라 달라지는 코드를 작성하다보면 종종 긴 함수가 만들어지는 것을 목격할 수 있다.
- **조건**과 **액션** 모두 의도를 표현해야한다!
- 함수 추출하기와 동일한 리팩토링!
- 실습
  ```tsx
  private Participant findParticipant(String username, List<Participant> participants) {
          return isNewParticipant(username, participants) ?
                  createNewParticipant(username, participants) :
                  findExistingParticipant(username, participants);
      }
  ```
  - 삼항 연산자를 이용해서 축약하기
  - if 문 안에도 함수를 넣어서 명시적으로 표시
  - **💭 삼항연산자를 쓰면 코드 가독성에 좋을까?**

### 반복문 쪼개기

- 하나의 반복문에서 여러 다른 작업을 코드를 쉽게 찾아볼 수 있다.
- 해당 반복문 수정할 때 여러 작업을 모두 고려하며 코딩해야한다.
- 반복문을 여러개로 쪼개면 보다 쉽게 이해하고 수정 가능
- 어차피 O(N)
  - 성능 문제를 야기할 수 있지만 리팩토링과 성능 최적화는 다른 작업
- 실습
  ```tsx
  try {
                          GHIssue issue = ghRepository.getIssue(eventId);
                          List<GHIssueComment> comments = issue.getComments();
                          Date firstCreatedAt = null;
                          Participant first = null;

                          for (GHIssueComment comment : comments) {
                              Participant participant = findParticipant(comment.getUserName(), participants);
                              participant.setHomeworkDone(eventId);

                              if (firstCreatedAt == null || comment.getCreatedAt().before(firstCreatedAt)) {
                                  firstCreatedAt = comment.getCreatedAt();
                                  first = participant;
                              }
                          }

                          firstParticipantsForEachEvent[eventId - 1] = first;
                          latch.countDown();
                      } catch (IOException e) {
                          throw new IllegalArgumentException(e);
                      }
  ```
  - 여기 for문을 함수로 빼기! → checkHomework()
  ```tsx
  try {
                      GHIssue issue = repository.getIssue(eventId);
                      checkHomework(issue.getComments(), participants, eventId);
                      latch.countDown();
                  } catch (IOException e) {
                      throw new IllegalArgumentException(e);
                  }
  ```
  ```tsx

      private void checkHomework(List<GHIssueComment> comments, List<Participant> participants, int eventId) {
          for (GHIssueComment comment : comments) {
              Participant participant = findParticipant(comment.getUserName(), participants);
              participant.setHomeworkDone(eventId);
          }
      }
  ```
- **💭 ExecutorService?**
  - 간단히 쓰레드풀 생성하여 병렬처리 할 수 있다!
    [Java - ExecutorService를 사용하는 방법](https://codechacha.com/ko/java-executors/)
- **💭 CopyOnWriteArrayList()**
  ```tsx
  List<Participant> participants = new CopyOnWriteArrayList<>();
  ```
  - 여러 스레드에 동시에 변경이 일어나면 ArrayList의 경우 → 예외 발생
  - 이 친구는 Concurrency에 안전함
  - 객체지향 복습!
    - ArrayList는 동기화가 안돼서 → 동시에 여러 스레드
    - Vector는 동기화 되기에 → 한번에 하나의 스레드
    - **💭 근데 왜 ArrayList는 여러 스레드에서 동시에 변경이 일어나면 예외가 발생하는거지?**
- 구현 → 의미로 코드를 바꾸는 과정이 중요한것 같다!

### 조건문을 다형성으로 바꾸기

- 여러 타입에 따라 각기 다른 로직으로 처리해야하는 경우 다형성을 적용해 조건문을 보다 명확하게 분리 가능
- 반복되는 switch문을 각기 다른 클래스를 만들어 제거 가능
  - 팩토리 메서드를 사용할 수도 있다
- 공통으로 사용되는 로직은 상위 클래스에 두고 달라지는 부분만 하위 클래스에 둠으로써 달라지는 부분만 강조 가능
- 모든 조건문을 다형성으로 바꿔야하는 것은 아니다!

**💭 추상클래스를 쓸때와 인터페이스를 쓸 때의 차이점은 뭘까?**
