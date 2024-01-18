# 2장 실행컨텍스트

## 실행컨텍스트란?

실행할 코드에 제공할 환경정보들을 모아놓은 객체

- 동일한 환경, 하나의 실행 컨텍스트 구성할 수 있는 방법
  - 전역 공간
  - eval()
  - 함수
- 자바 스크립트의 실행컨텍스트는 스택과 비슷
  - 전역 컨텍스트 → 처음 호출한 함수 ….→ 마지막 호출한 함수 (얘가 맨위)

## Variable Environment

- 최초 실행시 스냅샷 유지
- 실행컨텍스트 생성 → Variable Environment에 정보 먼저 담음 → 이걸 복사해서 렉시컬 환경 만들기 → 이후에는 렉시컬 환경 주로 활용
- Variable Environment과 Lexical Environment에는 environmentRecord와 outer-EnvironmentReference로 구성

## Lexical Environment

- 현재 컨텍스트 내부에는 a,b,c같은 식별자들이 있고 그 외부 정보는 D를 참조하도록 구성돼있음
- 컨텍스트를 구성하는 환경정보들을 사전에서 접하는 느낌

## environmentRecord와 호이스팅

- environmentRecord에는 현재 컨텍스트와 관련된 코드의 식별자 정보들이 저장
- 컨텍스트를 구성하는 함수에 지정된 매개변수 식별자, 선언한 함수가 있을 경우 그 함수 자체, var로 선언된 변수의 식별자들이 식별자에 해당
- 컨텍스트 내부 전체를 처음부터 끝까지 쭉 훑으면서 순서대로 수집
- 아무튼 매개변수 이름, 함수선언, 변수명등이 레코드에 담긴다

- 호이스팅을 할 때는 어떤 식별자들이 있을지만 관심있고, 어떤 값이 할당되는지는 관심이 없다!
- 변수명만 끌어올리고 할당과정은 그대로

```java
function a(){

	var x;
	var x;
	var x;

	x=1;
	console.log(x); //1
	console.log(x); //1
	x=2;
	console.log(x); //2
}

a(1);
```

- a함수 실행 시 → a함수의 실행컨텍스트 생성 → 변수명과 함수선언을 위로 올림

```java
function a(){
	console.log(b);
	var b = 'bbb';
	console.log(b);
	function b(){}
	console.log(b);
}

a();
```

```java
function a(){
	var b;
	var b = function b(){}; // 함수 표현식

	console.log(b);
	b='bbb';
	console.log(b);
	console.log(b);
}

a();
```

## let과 const의 호이스팅

`let/const`선언 변수는 호이스팅되지 않는 것이 아니다. 스코프에 진입할 때 변수가 만들어지고 TDZ(Temporal Dead Zone)가 생성되지만, 코드 실행이 변수가 실제 있는 위치에 도달할 때까지 액세스할 수 없는 것이다. `let/const`변수가 선언된 시점에서 제어흐름은 `TDZ`를 떠난 상태가 되며, 변수를 사용할 수 있게 된다.

## 함수 선언문과 함수 표현식

- 함수 선언문은 function 정의부만 존재하고 별도의 할당 명령이 없는거
- 함수 표현식은 function 을 별도의 변수에 할당하는 것!
- 오버라이딩을 방지하기 위해 웬만하면 표현식 쓸것!

## 호이스팅 리캡

- 함수 선언문과 변수 선언부 끌어올린다
- 변수 할당부는 원래 자리에!

## 스코프,스코프 체인, outerEnvironmentReference

- 스코프란 식별자에 대한 유효범위
- ES5까지는 전역공간 제외하면 함수에서만 스코프가 생김
- 식별자의 유효범위를 안에서부터 바깥으로 차례로 검색하는게 스코프체인
  - 이걸 가능하게 하는게 렉시컬 환경의 outerEnvironmentReference
  - 연결리스트 형식으로 구성
  - 동일한 식별자를 지정한 경우 무조건 스코프 체인 상에서 가장 먼저 발견된 식별자에만 접근 가능하게 됨.
- 스코프체인을 타고 갈수록 함수의 규모는 작아지지만 접근할 수 있는 변수의 범위는 커짐!

## 변수 은닉화

```java
var a = 1;
var outer = function(){
	var inner = function(){
			console.log(a);
			var a =3;
	}
}
outer();
```

- inner의 a는 전역의 a에 접근하지 못하는거!
