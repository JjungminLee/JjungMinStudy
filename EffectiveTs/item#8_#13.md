# 아이템 8 ~ 아이템 13

## 아이템 8 : 타입공간과 값 공간의 심벌 구분하기

- 타입스크립트의 심벌은 타입 공간이나 값 공간 중 한 곳에 존재
  - 이름이 같더라도 속하는 공간에 따라 타입으로 쓰일 수도 있고, 값으로 쓰일 수도 있다.
  - typeof 가 대표적 예시!
- 속성 접근자의 경우도 공간마다 다른 의미를 나타낸다
  - const A ⇒ 새 변수를 만들지만 as const ⇒ 이걸 상수로 쓰겠다 선언!
  - extends ⇒ class A extends B 의 경우 서브클래스를 의미, interface A extends B 서브 타입 의미/ T extends number 의 경우 제너릭 타입의 한정자 의미

---

## 아이템 9 : 타입 단언보다는 타입 선언을 이용하기

- as is

```tsx
interface Person {name:string};
const people =['alice','bob','jane'].map(name=>{name} as Person)
```

- to be

```tsx
interface Person {
  name: string;
}
const people = ["alice", "bob", "jane"].map((name) => {
  const person: Person = { name };
  return person;
});
```

- 언제 타입 단언을 쓰면 좋을까?
  - DOM 엘리먼트에 대해서!
- 모든 타입은 unknown 타입의 서브타입이다!
  - any와의 공통점 : 모든 타입을 허용
  - any와의 차별점 : 프로퍼티 또는 연산을 하는 경우 컴파일러가 체크. 그러므로 문제 되는 코드를 미리 예방

---

## 아이템 10 : 객체 레퍼타입 피하기

- 기본타입 : string,number,boolean,null,undefined,symbol,bigint (7가지)
  - 웬만하면 String같은 wrapper타입 쓰지 말것
  ```tsx
  "hello" === new String("hello"); // false라 인씩
  ```
  - string은 String에 할당 가능, String은 string에 할당 불가능

---

## 아이템 11 : 잉여 속성 체크의 한계 인지하기

- 객체 리터럴을 할당할 때 타입스크립트는 해당 타입의 속성이 있는지, 그 외의 속성은 없는지 확인
- 오타 같은 실수를 잡는데 효과적
- 잉여속성 체크에는 한계가 있다

---

## 아이템 12 : 함수 표현식에 타입 적용하기

- 함수 ‘문장’과 함수 ‘표현식’을 다르게 인식
  ```tsx
  function func(val: number): number {} // 문장
  const funcVal = (val: number): number => {}; // 표현식
  ```
- 표현식을 사용하는게 좋다!
  - 타입으로 관리하는것도 가능하다!
    ```tsx
    type BinaryFn = (a: number, b: number) => number;
    ```
- API 연동할 때 response를 받는 함수를 어떻게 작성하는게 이상적일까?
  ```tsx
  declare function fetch {
  	input : RequestInfo,init?:RequestInit
  }:Promies<Response>

  async function checkedFetch(input:RequestInfo,init?:RequestInit){
  	const response = await fetch(input,init);
  	if(!response.ok){
  		throw new Error("");
  	}
  	return response;
  }
  ```

---

## 아이템 13 : 타입과 인터페이스의 차이 알기

- 인덱스 시그니처
  ```tsx
  type TDict = { [key: string]: string };
  interface IDict {
    [key: string]: string;
  }
  ```
- 제네릭
  ```tsx
  type TPair<T> = {
    first: T;
    second: T;
  };

  interface IPair<T> {
    first: T;
    second: T;
  }
  ```
- 타입만의 차별점
  - 인터페이스는 유니온 타입 같은 복잡한 타입을 확장하지 못함
  - 복잡한 타입일 때는 타입을 써야한다
- 인터페이스 만의 차별점
  - 보강이 가능하다! ⇒ 선언병합
    ```tsx
    interface Istate{
    	name:string;
    	capital:string;
    }

    interface Istate{
    	population:number;
    }

    interface hmm:Istate{
    	name :"hmm",
    	capital:"seoul",
    	population:10,
    }

    ```
    - 보통 타입 선언 파일에서 활용
