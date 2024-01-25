# 4장 콜백함수

- 다른코드의 인자로 넘겨주는 함수
- 제어권과 관련이 깊다
- 인자로 넘겨주면서 제어권도 넘긴다.

## 제어권

```jsx
const timer = setInterval(function () {
  console.log(count);
}, 300);
```

- timer변수에는 setInterval의 id가 담긴다.
- setInterval이라는 다른 코드에 첫번째 인자로 익명함수를 넘기면 setInterval에 제어권이 넘어가며 스스로의 판단에 따라 익명함수를 실행함
- 제어권을 넘겨받은 코드는 콜백함수 호출 시점에 대한 제어권 가짐

## this

- map을 직접 구현해보자

```jsx
Array.prototype.map = function (callback, thisArg) {
  let mappedArr = [];
  for (var i = 0; i < this.length; i++) {
    let mappedValue = callback.call(thisArg || window, this[i], i, this);
    mappedArr[i] = mappedValue;
  }

  return mapedArr;
};
```

- thisArg에 값이 있을 경우 그 값을, 없는 경우 전역객체를 지정한다.
- **제어권을 넘겨받는 코드에서 call/apply 메서드의 첫번째 인자에 콜백함수 내부에서 this가 될 대상을 명시적으로 바인딩하기 때문!**

## 콜백함수는 함수다

- 어떤 객체의 메서드를 전달하더라도 메서드가 아닌 함수로 호출됨!

## 콜백 함수 내부의 this에 다른 값 바인딩하기

- 콜백함수 내부에서 this가 객체를 바라보게 하고 싶다면 어떻게 해야할까?
- 이전에는 변수 여러번 거쳐서 넣어놓는 느낌이 강했는데 매우 불편하기 그지없었음
- ES5의 bind메서드를 활용할것

```jsx
const obj1 = {
  name: "obj1",
  func: function () {
    console.log(this.name);
  },
};

setTimeout(obj1.func.bind(obj1), 1000);
```

## 자바스크립트의 비동기 처리

- Promise
  ```jsx
  addCoffee('에스프레소)()
  	.then(addCoffe('아메리카노'))
  	.then(addCoffe('아메리카노'))
  ```
  - 콜백함수는 호출될때 바로 실행되지만 내부에 resolve나 reject함수 호출하는 구문이 있을 경우 둘 중 하나가 실행되기 전까지는 then(다음) 또는 오류(catch) 구문으로 넘어가지 않음
- Async/Await
  ```jsx
  const coffeeMaker = async function () {
    const _addCoffee = async function () {};
    await _addCoffee("에스프레소");
  };
  ```

---

궁금한거

- **제어권을 넘겨받는 코드에서 call/apply 메서드의 첫번째 인자에 콜백함수 내부에서 this가 될 대상을 명시적으로 바인딩하기 때문! → 이 말이 이해가 안감**
- 자바스크립트 내장 API나 라이브러리 보면 _ga, \_addCoffee 같이 함수명 앞에 _ 많이 쓰던데 이거 뭐에여
