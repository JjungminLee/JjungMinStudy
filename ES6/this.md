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
