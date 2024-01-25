# 3장 this

## 왜 자바스크립트에서 this는 혼란스러울까?

- 다른 객체지향언어에서 this는 클래스로 생성한 인스턴스 객체를 의미
- 자바스크립트의 this는 어디서나 사용이 가능

## 상황에 따라 달라지는 this

- 실행컨텍스트가 생성될때 함께 결정됨
- this는 함수를 호출할때 결정된다.

## 전역공간에서의 this

- 전역공간에서는 전역객체를 가리킨다.
  - 전역객체는 window를의미
- 전역변수를 선언하면 전격객체의 프로퍼티로 할당.
- 자바스크립트의 모든 변수는 실은 특정 객체의 프로퍼티로서 동작
  - 특정객체란 실행 컨텍스트의 렉시컬 환경을 의미

```jsx
const a = 1;
console.log(window.a);
console.log(this.a);
```

- 둘 다 같다!
- delete의 경우 살짝 다름

  ```jsx
  const a = 1;
  delete window.a;

  window.b = 2;
  delete window.b;
  ```

  - 처음으로 전역객체의 프로퍼티로 할당한 경우(window.b) 삭제가 되는 반면 전역변수(const a)로 선언한 경우 삭제가 되지 않는다.

## 메서드로서 호출할 때 그 메서드 내부에서의 this

- 함수를 실행하는 방법
  - 함수로서 호출
  - 메서드로서 호출
- 둘의 차이는 → 독립성
  - 함수는 그자체로 `독립적인 기능`
  - 메서드는 `자신을 호출한 대상 객체에 관한 동작을 수행`
- 객체의 메서드로서 호출할 경우에만 메서드로 동작하고 그 외는 함수로 동작.

```jsx
const func = (x) => {
  console.log(this, x);
};
func(1); //함수로 동작 window{}

const obj = {
  method: func,
};

obj.method(2); // {method:f) 메서드로 동작
```

- 메서드 내부에서의 this
  - this에는 호출한 주체에 대한 정보가 담긴다.
  - 메서드로서 호출한 경우 호출 주체는 객체
  - 위 예시에서는 obj가 됨

## 함수로서 호출할때 그 함수 내부에서의 this

- 함수 내부에서의 this
  - 어떤 함수를 함수로서 호출하는 경우 this가 지정되지 않는다.
  - this에는 호출한 주체가 담기는데 함수로서 호출하면 → 전역 객체를 바라본다.
- 메서드 내부 함수에서의 this

  ```jsx
  const obj1 = {
  	outer : function(){
  			console.log(this);
  			const innerFunc = function(){
  					console.log(this);
  			}
  			innerFunc(); // window
  			const obj2 = {
  					innerMethod:innerFunc
  			};
  			obj2.innerMethod() // obj2
  	}

  obj1.outer(); // obj1
  ```

- 메서드 내부 함수에서의 this를 우회하는 방법

  - 호출 주체가 없을때는 자동으로 전역객체를 바인딩하지 않고 호출 당시 주변의 this를 그대로 상속받아 사용하게 하려면?
  - 임시방편으로 변수를 활용하는 방법이 있음

  ```jsx
  const obj1 = {
  	outer : function(){
  			console.log(this);
  			const innerFunc = function(){
  					console.log(this);
  			}
  			innerFunc(); // window
  			const self = this;
  			const innerFunc2 = function(){
  					console.log(self); //outer
  			}
  			innerFunc2();
  	}

  obj1.outer(); // obj1
  ```

- 화살표함수를 쓰는 이유

  ```jsx
  const obj1 = {
  	outer : function(){
  			console.log(this);
  			const innerFunc = function(){
  					console.log(this);
  			}
  			innerFunc(); // window

  			const innerFunc2 = () => {
  					console.log(this); //outer
  			}
  			innerFunc2();
  	}

  obj1.outer(); // obj1
  ```

## 콜백함수 호출 시 그 함수 내부에서의 this

- 함수 A의 제어권을 다른 함수 B에게 넘겨주는 경우 함수 A를 콜백함수라 한다.
- 제어권을 받은 함수에서 콜백함수에 별도로 this가 될 대상을 지정한 경우 그 대상을 참조하게됨

```jsx
setTimeout(function () {
  console.log(this);
}, 300);
[1, 2, 3, 4, 5].forEach(function (x) {
  console.log(this, x);
});
```

- 이럴때는 this가 전역객체를 가리킴.

```jsx
document.body.querySelector("#a")
	.addEventListener('click',function(2){
			console.log(this,e);
});
```

- 이런 경우 메서드 명의 점 앞의 부분이 this가됨

## 생성자 함수 내부에서의 this

```jsx
const Cat = function (name, age) {
  this.bark = bark;
  this.name = name;
  this.age = age;
};

const choco = new Cat("초코", 7);
```

## 명시적으로 this바인딩하기

### call 메서드

- 메서드의 호출 주체인 함수를 즉시 실행하도록 하는 명령어
- call메서드의 첫번째 인자를 this로바인딩, 이후의 인자들을 호출할 함수의 매개변수로
- 원래 함수를 그냥 실행하면 this는 전역객체 참조하지만 call메서드 이용하면 임의의 객체를 this로 지정가능!

```jsx
func.call({ x: 1 }, 4, 5, 6);
```

### apply메서드

- 두번째 인자를 배열로 받아 그 배열의 요소들을 호출할 함수의 매개변수로 지정함.

```jsx
func.apply({ x: 1 }, [4, 5, 6]);
```

### 활용 - 유사배열객체

```jsx
const obj = {
  0: "a",
  1: "b",
  2: "c",
};
Array.prototype.push.call(obj, "d");
const arr = Array.prototype.slice.call(obj);
```

- 객체에 배열메서드를 직접 적용할 수 없음
- call또는 apply사용해 차용하기!
- ES6부터는 from메서드가 생겼다.

```jsx
const obj = {
  0: "a",
  1: "b",
  2: "c",
};
const arr = Array.from(obj);
```

### 활용 - 생성자 내부에서 다른 생성자 호출

- 상속 느낌인건가

```jsx
function Person(name, gender) {
  this.name = name;
  this.gender = gender;
}

function Student(name, gender, school) {
  Person.call(this, name, gender);
  this.school = school;
}
```

### 활용 - 여러인수를 묶어 하나의 배열로 전달하고 싶을 때

- 이때도 apply활용하는데 ES6에서 스프레드 문법이 이걸 대신!

## bind메서드

- call과 비슷하지만 즉시 호출하지 않고 넘겨받은 this및 인수들을 바탕으로 새로운 함수를 반환하기만 함
- 다시 새로운 함수를 호출할때 인수 넘기면 그 인수들은 기존 bind메서드 호출할때 전달했던 인수들의 뒤에 이어서 등록됨
- bind는 this미리 적용 + 부분 적용 함수 구현하는 두가지 목적 가짐

```jsx
const bindFunc = func.bind({ x: 1 });
bindFunc(5, 6, 7, 8); // {x:1}5 6 7 8
```

## 상위 컨텍스트의 this를 내부함수나 콜백함수에 전달하기

- call, apply,bind를 이용하면 더 깔끔하게 처리 가능

```jsx
const obj1 = {
	outer : function(){
			console.log(this);
			const innerFunc = function(){
					console.log(this);
			}
			innerFunc.call(this);
	}

obj1.outer(); // obj1
```

```jsx
const obj1 = {
	outer : function(){
			console.log(this);
			const innerFunc = function(){
					console.log(this);
			}.bind(this);
			innerFunc();
	}

obj1.outer(); // obj1
```

## 화살표함수 예외사항

- 얘는 함수 내부에 this가 아예 없다
- 접근하고자 하면 스코프 체인상 가장 가까운 this에 접근하게 됨

## 콜백함수에서의 this

```jsx
const report = {
  sum: 0,
  count: 0,
  add: function () {
    const args = Array.prototype.slice.call(arguments);
    args.forEach(function (entry) {
      this.sum += entry;
      ++this.count;
    }, this);
  },
};
```

- this통해 report.sum과 report.count값이 바뀌게 됨!
- 요소 순회하면서 콜백함수를 반복호출하는 경우 일부 메서드는 별도의 인자를 this로 받기도 한다!

### window

함수나 변수를 전역공간에 만들면 window에 보관한다!

만약 전역에

```tsx
function func() {}

func();
window.func();
```

이렇게 쓴다면 사실상 셋다 똑같다!

```tsx
window:{

	func(){
		console.log(this)
	}
}
```

- 사실상 이거!

### this의 정의 1: 그냥 쓰거나 일반 함수,strict mode 안에서 쓰면 window

- consoel.log(this) ⇒ window
- window란 기본 함수들 수납공간

- strict 모드

```tsx
<script>'use strict' console.log(this) // 이때 undefined뜸</script>
```

### this의 정의 2: Object내 함수 안에서 this를 쓰면 그 함수를 가지고 있는 오브젝트를 뜻함! ⇒ obj가 됨

```tsx
var obj = {
  data: "kim",
  func: function () {
    console.log(this);
  },
};
obj.func();
```

- 오브젝트 안의 함수를 메서드라고함

```tsx
var obj = {
  data: {
    func: function () {
      console.log(this);
    },
  },
};
```

- 이 this는 어떻게 될까? → obj.data가 됨

```tsx
var obj = {
  data: {
    func: () => {
      console.log(this);
    },
  },
};
```

- arrow function을 썼다면? this값을 함수 밖에 있던거 그대로 쓴다! ⇒ 즉, window가 된다!

- function 키워드 신문법!

```tsx
var obj = {
  data: {
    func() {
      console.log(this);
    },
  },
};
```

- 이런식으로 해도 된다! func : → 이렇게 안하고 냅다 함수명(){} 이렇게 써도 된다!

### this의 정의 3: 생성자 함수

```tsx
// 생성자 함수
function UserInfo() {
  this.name = "Nick";
  this.age = 20;
  this.addresss = "Busan";
}

// 객체 생성
let userInfo = new UserInfo();

console.log(userInfo);
```

### this의 정의 4 : 이벤트 리스너안에서는 e.currentTarget

```tsx
document.getElementById("button").addEventListener("click", () => {
  this;
});
```

- 이때 this는 e.currentTarget을 의미

### 연습문제 1

```tsx
document.getElementById('button').addEventListener('click',
	function(e){
			var arr=[1,2,3];
			arr.forEach(function(a){
					console.log(a);
			});

	}
}
```

- 이때는 뭐가 출력될까?
  - 이때는 window가 출력됨

### 연습문제 2

```tsx
var obj = {
  names: ["kathy", "kathy1"],
  func: function () {
    console.log(this); // 1
    obj.names.forEach(function () {
      console.log(this); // 2
    });
  },
};
```

- 1번은 obj를 출력하고
- 2번은 window를 출력한다. → 사실상 여기에 함수는 근본없는 함수로 봐도 무방하다

### 연습 문제 3 arrow function으로 바꿨을 때

```tsx
var obj = {
  names: ["kathy", "kathy1"],
  func: function () {
    console.log(this); // 1
    obj.names.forEach(() => {
      console.log(this);
    });
  },
};
```

- 화살표 함수는 내부의 this값을 변화시키지 않음
- 위의 this값 그대로 재사용 가능
