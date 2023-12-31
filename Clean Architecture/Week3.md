# 3주차

## 🛠아키텍쳐

### 아키텍처란?

- 소프트웨어 아키텍트는 프로그래머
- 아키텍트는 코드와 동떨어져서는 안된다.
- 아키텍처의 주 목적은 시스템의 생명주기를 지원하는 것
- 좋은 아키텍처는 시스템을 쉽게 이해, 개발, 유지보수, 배포함.
- 궁극적인 목표는 시스템의 수명과 관련된 비용은 최소화하고, 프로그래머의 생산성은 최대화
- 아키텍처의 필요성이 느껴지는건 팀의 인원이 많을 수록!

### 운영?

- 시스템을 운영할 때 아키텍처가 맡은 역할
  - 시스템 운영방식을 드러냄
    - 유스케이스,기능,시스템의 필수행위를 일급 엔티티로 격상
    - 이들 요소가 개발자에게 주요 목표로 인식되게
    - 시스템을 이해하기 쉬워지며 개발과 유지보수에 큰 도움이 됨

### 유지보수

- 비용이 가장 많이 들어감
  - 탐사+위험부담이 가장 비용이 많이 드는 부분
    - 탐사 → 기존 소프트웨어에 새로운 기능 추가하거나 결함을 수정할 때 어떤 전략을 쓰는게 최적일지 결정할 때 드는 비용
- 주의를 기울여 신중하게 아키텍처를 만들면 이 비용을 크게 줄일 수 있음
- 시스템을 컴포넌트로 분리하고, 안정된 인터페이스를 두어 서로 격리

### 선택사항 열어두기

- 소프트웨어를 부드럽게 유지하는 방법은 선택사항을 가능한 많이 그리고 가능한 오랫동안 열어두는 것
- 열어두어야 하는 선택사항 → 중요하지 않은 세부사항

### 장치 독립성

- 어떤 장치를 사용할지 전혀 모른채, 고려하지 않고도 프로그램 작성 가능
- 고수준 정책은 디스크의 물리적 구조로부터 독립되어야 함.

### Recap

- 좋은 아키텍트란 세부사항을 정책으로부터 신중하게 가려내고
- 정책이 세부사항과 결합되지 않도록 엄격하게 분리한다.
- 좋은 아키텍트는 세부사항에 대한 결정을 가능한 오랫동안 미룰 수 있는 방향으로 정책을 설계

## 🛠 독립성

### 유스케이스

- 시스템의 업무 요구사항
- 시스템의 의도를 지원해야함
- 시스템의 수평적인 계층을 가로지르도록 자른, 수직으로 좁다란 조각
- 각 유스케이스는 UI의 일부, 애플리케이션 특화 업무 규칙의 일부, 애플리케이션 독립적 업무 규칙의 일부, 데이터베이스 기능의 일부를 사용
- pattern
  - 시스템에서 서로 다른 이유로 변경되는 요소들의 결합을 분리하면 기존 요소에 지장을 주지않고도 새로운 유스케이스 계속해서 추가할 수 있게 됨.
  - 유스케이스 뒷받침하는 UI와 데이터 베이스를 서로 묶어서 각 유스케이스가 UI와 데이터베이스의 서로 다른 관점을 사용하게 된다면, 새로운 유스케이스를 추가하더라도 기존 유스케이스에 영향을 주는 일은 거의 없을 것

### 결합 분리모드

- 계층과 유스케이스의 결합을 분리해야함
- 분리 방법
  - 소스 수준 분리 : 소스코드 모듈 사이의 의존성 제어
  - 배포 수준 분리 : jar,DLL,공유 라이브러리와 같은 배포 가능한 단위 들 사이의 의존성 제어
  - 서비스 수준 분리 : 의존하는 수준을 데이터 구조 단위까지 낮추기

우선은 서비스 수준에서의 분리를 기본 수준으로 잡자!

## 🛠경계 : 선긋기

- 소프트웨어 아키텍쳐는 선을 긋는 기술 → 이런 선을 경계라고 부름
- 인적 자원의 효율을 떨어뜨리는 요인 → 결합

### 어떻게 선을 그을까?

- 관련이 있는 것과 없는 것 사이에 선을 긋는다
  - GUI와 업무규칙은 관련이 없기 때문에 선을 긋는다
  - DataBase→Business Rules 이지만 Business Rules는 DataBase를 알지 못함
- 입력과 출력 (GUI)는 중요하지 않다

### 플러그인 아키텍처

- 소프트웨어 개발 기술의 역사는 플러그인을 손쉽게 생성하여 확장가능하며 유지보수가 쉬운 시스템 아키텍쳐를 확립할 수 있게 만드는 방법에 대한 이야기
- 선택적이거나 또는 수많은 다양한 형태로 구현될 수 있는 나머지 컴포넌트로부터 핵심적인 업무규칙은 분리되어 있고, 또한 독립적

## 🛠 경계 해부학

### 경계 횡단

- 그 비결은 소스코드 의존성 관리
- 단순한 형태의 경계횡단은 저수준 클라이언트에서 고수준 서비스로 향하는 함수 호출
- 경계 횡단시 의존성은 고수준 컴포넌트를 향한다
- 이렇게 하면 고수준 컴포넌트는 저수준 세부사항으로부터 독립적으로 유지

### 로컬 프로세스

- 명령행이나 그와 유사한 시스템 호출을 통해 생성
- 정적으로 링크된 단일체
  - 여러 모노리틱 프로세스가 같은 컴포넌트들을 가질 수 있음
- 동적으로 링크된 여러개의 컴포넌트
  - 동적으로 링크된 배포형 컴포넌트들을 서로 공유 가능
- 로컬 프로세스간 분리 전략 또한 항상 고수준의 컴포넌트를 향할 것

## 🛠 정책과 수준

- 저수준의 컴포넌트가 고수준의 컴포넌트에 의존하도록 설계되어야함
- 저수준 컴포넌트는 고수준 컴포넌트에 플러그인 되어야 한다

### 수준

- 입력과 출력까지의 거리
- 멀리 위치할 수록 정책의 수준의 높아짐

## 🛠 업무 규칙

- 사업적으로 수익을 얻거나 비용을 줄일 수 있는 규칙 또는 절차
- 핵심업무규칙은 보통 데이터를 요구 → 이런 데이터를 핵심 업무 데이터

### 엔티티

- 컴퓨터 시스템 내부의 객체로서 핵심 업무 데이터 기반으로 동작하는 일련의 조그만 핵심 업무 규칙 구체화

### 유스케이스

- 모든 업무규칙이 엔티티처럼 순수하지 않음
- 애플리케이션에 특화된 업무 규칙을 설명
- 엔티티 내부의 핵심 업무규칙을 어떻게, 언제 호출할지 명시하는 규칙을 담음
- 엔티티가 어떻게 춤을 출지 유스케이스가 제어
- 즉 엔티티가 고수준이고, 유스케이스가 저수준
  - 유스케이스가 단일 애플리케이션에 특화
  - 엔티티는 수많은 다양한 애플리케이션에서 사용될 수 있도록 일반화
  - 유스케이스는 엔티티에 의존, 엔티티는 유스 케이스에 의존하지 않음

### Recap

- 업무규칙은 소프트웨어 시스템이 존재하는 이유이다.
- 사용자 인터페이스나 데이터 베이스와 같은 저수준의 관심사로 인해 오염되서는 안되며 원래 그대로의 모습으로 남아있어야 한다.
- 업무규칙은 시스템에서 가장 독립적이며 가장 많이 재사용할 수 있는 코드여야 한다.

## 🛠 소리치는 아키텍처

- 소프트웨어의 아키텍쳐도 유스케이스에 대해 소리쳐야한다!
  - 예를 들면 이건 헬스 케어 시스템이야! 이건 재고 관리 시스템이야! , 레일스야! 스프링이야!

### 프레임워크는 도구일뿐, 삶의 방식은 아니다

- 프레임워크가 모든 것을 하게하자라는 태도는 위험함
- 어떻게 하면 아키텍쳐를 유스케이스에 중점을 둔채 그대로 보존할 수 있을지 생각해야함

### Recap

- 아키텍쳐는 시스템을 이야기해야하며 시스템에 적용한 프레임워크에 대해 이야기해서는 안된다
- 만약 헬스케어 시스템 구축하고 있을 때 새로 들어온 프로그래머가 소스코드를 보고 “오, 헬스케어 시스템이군”이어야만 함.
