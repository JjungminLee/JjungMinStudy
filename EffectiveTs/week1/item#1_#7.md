

# 아이템 1: 자바스크립트와 타입 스크립트의 관계

### 타입스크립트는 문법적으로도 자바스크립트의 상위집합

자바스크립트에 문법오류가 없다면? → 유효한 타입스크립트 프로그램

### 타입스크립트의 목표 : 런타임에 오류를 발생시킬 코드를 미리 찾아내는 것

- 타입스크립트가 정적 타입 시스템이라는 것은 바로 이런 특징
- 단점 : 타입체커가 모든 오류를 찾아내진 않는다.
  - 의도와 다르게 동작하는 코드도 있다.

### 💭 복습타임 : 컴파일 타임과 런타임의 차이점은?

- 런타임
  - Web Browser에서 작동할때, Node.js에서 작동할때, Browser와 Node.js를 런타임이라 함.
- 컴파일 타임
  - 소스코드를 작성하고 이를 컴파일 하여 기계어코드로 변환하는 일련의 과정을 의미
- 예외 vs 에러
  - 에러 : 중하다
  - 예외 : 개발자가 수습가넝하다!
  - 컴파일 에러 vs 런타임 에러
    - 컴파일 에러
      - type check error
      - 흔히 말하는 빨간줄
      - 예외 처리
        - try-catch
        - throws로 예외를 전가해버리기!
    - 런타임 에러
      - zero divison

## Recap

타입스크립트는 자바스크립트 런타임 동작을 모델링하는 타입 시스템을 가지고 있기에 런타임 오류 발생시키는 코드를 찾아내려함

- 모든 오류를 찾아낼거라 기대하면 안됨 → 타입체커를 통과하면서도 런타임 오류를 발생시키는 코드는 충분히 존재

---

# 아이템 2: 타입스크립트 설정 이해하기

```tsx
{
	 compilerOptions:{
			"noImplicity":true
	}
}
```

- noImplicity는 변수들이 미리 정의된 타입을 가져야하는 여부를 제어
- function add(a:string,b:string) → 이런식으로 타입을 정해줘야함

### compilerOptions가 뭐지?

컴파일 대상 파일들을 어떻게 변환할지 세세히 정하는 옵션

```tsx
{
	 compilerOptions:{
			"strictNullChecks":true
	}
}
```

- null과 undefined가 모든 타입에서 허용되는지 확인하는 설정

### 💭 다들 이런 에러 한번씩 보지 않았나요?

```tsx
TypeError: object is not iterable (cannot read property Symbol(Symbol.iterator))
```

- undefined일수도 있는데이터를 map 걸었을때 이런 에러가 발생하는데요
- 다들 어떤식으로 해결하시나요?

---

# 아이템 3: 코드 생성과 타입이 관계없음을 이해하기

### 타입스크립트 컴파일러의 역할

- 최신 타입스크립트/자바스크립트를 브라우저에서 동작할 수있도록 구버전의 자바스크립트로 트랜스파일한다.
- 코드의 타입 오류를 체크

### 타입 오류가 있는 코드도 컴파일이 가능할까?

타입스크립트가 자바 스크립트로 변환될 때 코드 내의 타입에는 영향을 주지 않는다!

자바스크립트의 실행 시점(런타임) 에도 타입은 영향을 미치지 않는다! → 고로 가능하다!

### 코드에 오류가 있다 === 컴파일에 문제가 있다?

오직 코드생성만이 컴파일이라 할 수 있기에

작성한 타입스크립트가 유효한 자바 스크립트라면 타입스크립트 컴파일러는 컴파일 해냄.

고로, **타입체크에 문제가 있다**가 더 정확한 표현!

### 런타임에 타입정보를 유지하는 방법은?

![Untitled](https://s3-us-west-2.amazonaws.com/secure.notion-static.com/bd720307-edfd-4427-9449-a2a4f93ab7c0/Untitled.png)

instanceof는 객체로 판별을 하는건데 값으로 읽어들이고 있다!

- 태그 기법 사용

```tsx
type Shape = Square | Rectangle;
```

```tsx
interface Square {
  kind: "square";
  width: number;
}

interface Rectangle {
  kind: "rectangle";
  height: number;
  width: number;
}

type Shape = Square | Rectangle;

function calculateArea(shape: Shape) {
  if (shape.kind == "rectangle") {
    shape;
    return shape.width * shape.height;
  }
}
```

- 클래스 사용
  - 인터페이스는 타입으로 사용이 가능하지만 클래스를 사용하면 타입과 값 모두로 사용이 가능하다
  ```tsx
  class Square {
    constructor(public width: number) {}
  }

  class Rectangle extends Square {
    constructor(public width: number, public height: number) {
      super(width);
    }
  }

  type Shape = Square | Rectangle; // 타입으로 참조

  function calculateArea(shape: Shape) {
    if (shape instanceof Rectangle) {
      // 값으로 참조
    }
  }
  ```

### 타입연산은 런타임에 영향을 주지 않는다!

무지성으로 as를 갈기지 않았을까?

- as is

```tsx
function asNumber(val: number | string): number {
  return val as number;
}
```

- to be

```tsx
function asNumber(val: number | string): number {
  return typeof val === "string" ? Number(val) : val;
}
```

- 타입체킹을 먼저 하고 형변환 시켜줄 것

### 타입스크립트 타입으로는 함수를 오버로드 할 수 없다

- 타입과 런타임 동작이 무관하기에 함수 오버로딩 불가능

---

# 아이템 4: 구조적 타이핑에 익숙해지기

### 덕타이핑

- 자바스크립트는 본질적으로 “덕타이핑”
- 매개변수 값이 모두 제대로 주어진다면 그 값이 어떻게 만들어졌는지 신경쓰지않고 사용

### 구조적 타이핑

- 테스트 코드 작성할 때 유리
- 코드 구조 관점에서 타입이 서로 호환되는지의 여부를 판단하는 것
- 어떠한 값이 다른 속성도 가질 수 있다
- 덕타이핑이라고 한다

[TypeScript 타입 시스템 뜯어보기: 타입 호환성](https://toss.tech/article/typescript-type-compatibility)

어떠한 경우에 타입호환을 허용할지 규칙이 필요함

- 명목적 서브 타이핑
  - 타입 정의시 상속관계를 명확히 명시한 경우에만 타입호환 허용
    ```tsx
    type Food = {
      protein: number;
      fat: number;
    };
    type Burger = Food & {
      brand: string;
    };
    const burger: Burger = {
      protein: 12,
      fat: 13,
      brand: "버거킹",
    };

    function calculateCalorie(food: Food) {
      return food.fat * 9 + food.protein * 3;
    }
    ```
- 구조적 서브 타이핑
  - 상속관계가 명시되어 있지 않아도 객체의 프로퍼티 기반으로 사용처에서 사용함에 문제가 없다면 사용을 허용하는 방식
  ```tsx
  type Food = {
    protein: number;
    fat: number;
  };
  type Burger = Food & {
    brand: string;
  };
  const burger = {
    protein: 12,
    fat: 13,
    brand: "버거킹",
  };

  calculateCalorie(burger);
  function calculateCalorie(food: Food) {
    return food.fat * 9 + food.protein * 3;
  }
  ```
- 캐시생각 : '구조적으로' 타입이 맞기만 한다면 이를 허용해준다는 느낌으로 이해함

---

## 아이템 5: any타입 지양하기

- 타입시스템은 점진적이고 선택적
  - 코드에 타입을 조금씩 추가할 수 있고
  - 언제든 타입체커를 해제할 수 있다
- 아무튼 any 쓰지마세요
  - 자동완성 적용 x
  - 코드 리펙토링 때 버그를 감춤
  - 타입설계를 감춤

### 💭 이벤트 같은거는 타입헨들링이 힘든데 다들 어떻게 처리하시나요?! + React.FC 다들 쓰는 편이신가요?

---

## 아이템 6: 편집기를 사용한 타입 시스템 탐색

- 타입 스크립트를 설치하면
  - 타입스크립트 컴파일러 (tsc), 단독으로 실행할 수 있는 타입스크립트 서버 (tsserver) 실행 가능
  - 코드 자동 완성, 명세 검사, 검색, 리팩터링이 포함
- 편집기를 통해 어떻게 타입 시스템이 동작하는지, 타입스크립트가 어떻게 타입을 추론하는지 개념을 잡을 수 있다!

---

## 아이템 7: 타입이 값들의 집합이라고 생각하기

- 오류를 체크하는 순간에는 ‘타입’을 가지고 있다.
  - ‘할당 가능한 값들의 집합’ ⇒ 타입
- 유닛타입
  ```tsx
  type A = "A";
  ```
  - 리터럴 타입이라고도 함
- 유니온 타입
  ```tsx
  type AB = "A" | "B";
  ```
  - 두개 혹은 세개로 묶은 것 의미
- 타입 체커의 역할 ⇒ 하나의 집합이 다른 집합의 부분집합인지 검사
- 헷갈리는 집합관계
  - keyof(A&B) = (keyof A) | (keyof B)
  - keyof(A|B)= (keyof A) & (keyof B)
  - 이런 관계에서 생각해볼까요?
  ```tsx
  type A = { foo: number; commonProp: string };
  type B = { bar: boolean; commonProp: number };

  type Intersection = A & B; // { commonProp: string | number }
  type KeysOfIntersection = keyof Intersection; // "commonProp"

  type KeysOfA = keyof A; // "foo" | "commonProp"
  type KeysOfB = keyof B; // "bar" | "commonProp"

  type UnionKeys = KeysOfA | KeysOfB; // "foo" | "bar" | "commonProp"

  console.log(UnionKeys === KeysOfIntersection); // true
  ```

### 💭 복습하기 : [객지프] Generic → WildCards and Subtyping

List <? extends A> ⇒ A를 포함해서 A의 자식들을 다 받을 수 있다.

List <? super D > ⇒ D부터 시작해서 D의 부모들

![Untitled](https://s3-us-west-2.amazonaws.com/secure.notion-static.com/5ec72351-a539-4621-ad04-8706a5a35f31/Untitled.png)

- 타입이 집합이라는 관점에서 extends는 ~에 할당 가능한 이라는 의미
- keyof 정리

  - key값만 모아서 유니온 타입으로 정의한다.

  ```tsx
  type Type = {
    name: string;
    age: number;
    married: boolean;
  };

  type Union = keyof Type;
  // type Union = name | age | married

  const a: Union = "name";
  const b: Union = "age";
  const c: Union = "married";
  ```

- Exclude 정리
  ```tsx
  type Exclude <T,U>
  ```
  T에 오는 타입들 중 U에 오는 것들은 제외하겠다는 의미
  ```tsx
  type Fruit = "cherry" | "banana" | "strawberry" | "lemon";

  type RedFruit = Exclude<Fruit, "banana" | "lemon">;
  // type RedFruit = "cherry" | "strawberry" 와 같다.
  ```
  Fruit타입 중 banana와 lemon을 제외하겠다
