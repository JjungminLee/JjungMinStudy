# arrow function

### arrow function의 장점

1. 입출력할때 보기 쉬움

   ```tsx
   const func =(a)=>{return a+10;
   ```

2. 파라미터 1개면 소괄호 생략가능

   ```tsx
   const func = (a) => {
     return a + 10;
   };
   ```

3. 코드 한줄이면 중괄호 생략가능

   ```tsx
   const func = (a) => a + 10;
   ```

### forEach에서

```tsx
[1, 2, 3, 4].forEach((a) => console.log(a));
```

### 이벤트 리스너에서

```tsx
document.getElementById("button").addEventListener("click", (e) => {
  this;
});
```

- 이때 this는 window!
- 바깥에 있던 this값 그대로 내부에서 사용
- 화살표함수는 애초에 this가 없기에 그냥 전역객체를 참조한다고 생각할것!

```tsx
button.addEventListener("click", function () {
  console.log(this); // button 엘리먼트
  this.innerHTML = "clicked";
});
```

- 하지만 이렇게 쓴다면 button 엘리먼트를 호출할 수 있다!
- 왜냐? 일반함수는 this가 동적으로 달라지기에 function 내부에서 this는 이벤트 리스너가 된다!

### Object에서

```tsx
const obj = {
  func: () => {
    return this;
  },
};
obj.func();
```

- window가 나온다! obj를 가리키지 않음! 바깥에 있던 this가 재정의
- `this`는 자신을 호출한 객체  obj이 아니라 함수 선언 시점의 상위 스코프인 전역객체를 가리키게 됩니다.

### 연습문제

1.

```
const Person = {
  name: "손흥민",
  sayHi: () => {
    console.log("안녕 나는 " + Person.name);
  },
};

Person.sayHi();
```

2.

```tsx
const Data = {
  data: [1, 2, 3, 4, 5],
  sumAll: () => {
    let sum = 0;
    Data.data.forEach((a) => (sum += a));
    console.log(sum);
  },
};
Data.sumAll();
```

### Recap : function vs arrow function

- **일반 함수:** 일반 함수 내부의 **`this`**는 해당 함수를 호출한 컨텍스트에 따라 동적으로 결정됩니다. 함수가 어떻게 호출되었는지에 따라 **`this`**가 다르게 설정됩니다.
- **화살표 함수:** 화살표 함수 내부의 **`this`**는 함수가 정의된 곳의 컨텍스트를 유지합니다. 즉, 주변 스코프의 **`this`**를 상속받아 사용하며, 동적으로 변경되지 않습니다.

```tsx
const cat = {
  name: "meow",
  foo1: function () {
    const foo2 = function () {
      console.log(this.name);
    };
    foo2();
  },
};

cat.foo1(); // undefined
```

이것은 일반함수를 썼을 때 → meow가 아닌 undefined를 뱉음!

지정되지 않아서 전역객체를 가리키는 것!

```tsx
const cat = {
  name: "meow",
  foo1: function () {
    const foo2 = () => {
      console.log(this.name);
    };
    foo2();
  },
};

cat.foo1(); // meow
```

화살표함수는 meow를 가리킬 수 있음

이게 어떻게 가능할까? → 화살표함수는 this가 없다!

### 💭 의문점

어떻게 화살표함수 안에서 this.innerHTML이 가능할까? window가 되지 않을까?

```tsx
document.getElementById("버튼").addEventListener("click", function () {
  setTimeout(() => {
    console.log(this.innerHTML);
  }, 1000);
});
```

콜백함수인 function에서 this호출 하면 버튼 엘리먼트가 호출될거고, setTimeout에서 this는 화살표 함수가 정의된 컨텍스트를 가리킴, 즉 그 외부가 버튼 엘리먼트이기 때문에 this.innerHTML이 가능해지는 것!
