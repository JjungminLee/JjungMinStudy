# 1장 데이터타입

## 데이터 타입의 종류

- 기본형
  - 숫자
  - 문자열
  - 불리언
  - null
  - undefined
- 참조형
  - 객체
  - 배열
  - 함수
  - 날짜
  - 정규표현식
  - Map,WeakMap
  - Set,WeakSet

## +) WeakMap,WeakSet?

- map과 weakmap의 첫번째 차이는 **weakmap의 key가 반드시 객체**여야 한다는 점!

원시값은 위크맵의 key가 될 수 없다

```java
let weakMap = new WeakMap();

let obj = {};

weakMap.set(obj, 'ok'); // 정상적으로 동작한다 (객체 key)
weakMap.set('test', 'notOk'); // Error: Invalid value used as weak map key
```

- map과 weakmap의 두 번째 차이는 weakmap은 반복 작업과 `keys()`, `values()`, `entries()`메서드를 지원하지 않는다는 점 👉 **weakmap에서는 key나 값 전체를 얻는 것이 불가능하다**

• set과의 weakSet의 차이점 - 객체만 저장할 수 있다, 원시값은 저장할 수 없다

• weakmap과는 달리 복잡한 데이터를 저장하지 않고 yes/no 같은 간단한 답변 얻는 용도

[Map, Set / WeakMap, WeakSet](https://velog.io/@100pearlcent/Map-Set-WeakMap-WeakSet#weakmap--weakset)

## 자바스크립트가 타입에서 자유로운 이유?

- 메모리 공간이 월등하게 커졌다!
- 정수형이 4바이트였는데 자스는 8바이트나!

## 데이터 할당

- 자바 스크립트는 데이터 영역과 변수영역을 따로 관리!
  - 예를 들어 내가 문자열을 저장한다고 하면
  - 메모리에서 비어있는 공간 확보하고 그 공간의 이름 설정
    - 1003번지의 이름은 a, 값은 @5004
  - 데이터를 저장하기 위한 별도의 메모리 공간을 또 확보에서 문자열을 저장
    - 5004번지의 데이터 ‘abc’저장
- 데이터 변환을 자유롭게 할 수 있게 함과 동시에 메모리 효율적으로 사용하게 하기 위함

## 불변값

- 불변값과 상수는 같은 개념이 아니다!
  - 변수와 상수의 구분은!
    - 변수영역의 메모리
    - 한번 데이터 할당이 이루어진 변수 공간에 다른 데이터를 재할당할수 있는지 여부가 관건
  - 불변성 여부의 구분?
    - 데이터 영역의 메모리
    - **abc를 담는 변수가 있고 뒤에 def를 붙이려면 abc가 abcdef가 되는게 아닌 abcdef라는 문자열을 새로 만들어서 변수에 저장!**
- 기본형 데이터는 모두 불변값!

## 가변값

- 참조형 객체는 전부 가변값은 아님!
- **그치만 새로운 객체가 만들어지는게 아닌 기존 객체 내부의 값만 바뀜**
- 중첩객체

```java
const obj ={
	x : 3,
	arr : [3,4,5],
}
```

## 변수 복사 비교

- 불변값이던 가변값이던 복사 과정은 동일함

## 정말 기묘한 변수 찾는 방식

```java
let a = 10;
let b = a;
let obj1 = {c:10, d:'ddd'};
let obj2 = obj1;

b = 15;
obj2.c = 20;
```

- 보통 사람이라면 위에서 아래로 찾을것 같은데
- 우선 b=15부터 가서 → 데이터영역에 15가 없으므로 새로운 공간에 저장(5004번지) → 5004번지에서 주소를 든 채로 변수영역에서 식별자가 b인 주소를 찾음
- 데이터영역 → 변수영역으로 접근
- 아래 obj2.c도 마찬가지 20부터 가서 새로운 공간에 저장(5005번지) → 5005번지에서 식별자 c인 주소를 찾는다.
- 큰일남!
  - 책에서는 b는 바뀌고 obj2는 값이 바뀌지 않는다 했는데
  ![Untitled](https://prod-files-secure.s3.us-west-2.amazonaws.com/ae71164f-c067-4ec6-914d-e1875499ca16/fcf6a910-5b4d-4f15-aa6a-4a944a3d1a2e/Untitled.png)
  - 왜 이럴까요.. 또르륵…
- 아무튼 자바스크립트는 기본값이던 참조값이던 주소값을 참조한다함!

## 불변객체

- 값으로 전달받은 객체에 변경을 가하더라도 원본 객체는 변하지 않아야 하는 경우
- user와 user2가 다르다고 인식되는 코드는 어떤걸까요?
  ```java
  var user = {
  	name : "어쩌구",
  	gender = "male",
  }

  var changeName = function(user,userName){

  	var newUser = user;
  	newUser.name = userName;
  	return newUser ;
  };

  var user2 = changeName(user,'Jung');
  ```
  ```java
  var user = {
  	name : "어쩌구",
  	gender = "male",
  }

  var changeName = function(user,userName){
  	return {
  		name : userName,
  		gender : user.gender,
  	};
  };

  var user2 = changeName(user,'Jung');
  ```

## 얕은복사와 깊은 복사

- 얕은 복사
  - 바로 아래 단계의 값만 복사하는 방법
  - 주소값만 복사
  - 이러면 원본과 사본이 모두 동일한 참조형 데이터의 주소를 가리킴
  - 사본을 바꾸면 원본이 바뀌고 원본을 바꾸면 사본이 바뀜 띠용

```java
var user2 = copyObject(user);
```

- 깊은 복사
  - 내부의 모든 값들을 하나하나 찾아서 전부 복사하는 방법

```java
var copyObjectDeep = function(target){

	var result = {};
	if(typeof target === 'object' && target !== null){
		for (var prop in target){
			result[prop] = copyObjectDeep(target[prop]);
		}
	} else {
		result = target;

	}
	return result;
}
```

- 또는 JSON 문법으로 표현된 문자열로 전환했다가 다시 JSON객체로 바꾸기!

## undefined와 null

- 없음을 나타내는 두 친구
- undefined
  - 값을 대입하지 않은 변수 (즉 데이터 영역의 메모리 주소를 지정하지 않은 식별자에 접근시)
  - 객체 내부에 존재하지 않는 프로퍼티에 접근 시
  - return문이 없거나 호출되지 않은 함수의 실행결과
  - undefined는 직접 할당하지 말 것
  ```java
  let arr1 = [undefined,1];
  ```
  - reference error vs undefined
  ```java
  var a;
  console.log(a) ; // undefined
  var obj = {a:1};
  console.log(obj.b); // reference error
  ```
- null
  - 비어있음을 나타내고 싶을 때
  - typeof null은 object이다!

## 그만써 &&

[It’s 2023, Please Stop Using “&&” for Conditional Rendering in React](https://medium.com/@Evelyn.Taylor/its-2023-please-stop-using-for-conditional-rendering-in-react-be6cbcd72cba)

- as-is

```java
function  MyComponent ( { isLoggedIn } ) {
  return (
    <div>
      {isLoggedIn && <p>다시 오신 것을 환영합니다, 사용자님!</p>}
    </div>
  );
}
```

- to-be
  - 삼항연산자를 쓰자

```java
function  MyComponent ( { isLoggedIn } ) {
  return (
    <div>
      {isLoggedIn ? <p>다시 오신 것을 환영합니다, 사용자님!</p> : null}
    </div>
  );
}
```

- 논리 or을 쓰자
