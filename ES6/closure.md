# 5장 클로저

- 어떤 함수에서 선언한 변수를 참조하는 내부함수에서만 발생하는 현상

```jsx
var outer = function () {
  var a = 1;
  var inner = function () {
    console.log(++a);
  };
  inner();
};

var outer = function () {
  var a = 1;
  var inner = function () {
    console.log(++a);
  };
  return inner();
};
var outer2 = outer();
console.log(outer2); //2
```

- 두 예제 모두 outer함수의 실행 컨텍스트가 종료되기 이전에 inner함수의 실행컨텍스트가 종료되어있으며 이후 별도로 inner함수를 호출할 수 없음
- outer의 실행컨텍스트가 끝난 이후에도 inner함수를 호출할 수 있게 하려면?

```jsx
var outer = function () {
  var a = 1;
  var inner = function () {
    return ++a;
  };
  return inner;
};

var outer2 = outer();
conlsole.log(outer2()); //2
console.log(outer2()); //3
```

- 이전까지는 inner함수의 실행결과를 반환했다면 이제는 inner함수 자체를 반환했다!
  - `var outer2 = outer();` 에서 outer함수의 실행컨텍스트가 종료될때 outer2변수는 outer의 실행결과인 inner함수를 참조하게 된다.
  - `conlsole.log(outer2());` outer2를 호출하면 앞서 반환된 inner함수가 실행된다.
- 이상한게 inner함수 실행시점에는 outer함수는 이미 종료상태인데 어떻게 outer함수의 렉시컬 환경에 접근할 수 있는걸까?
  - **가비지 컬랙터는 어떤 값을 참조하는 변수가 하나라도 있다면 그 값은 수집 대상에 포함시키지 않는다.**
  - 외부함수인 Outer함수가 실행종료 되더라도 내부 함수인 inner함수가 outer2를 실행시키기에 호출된 가능성이 생긴것!

## 클로저는 뭘까?

- 어떤 함수에서 선언한 변수를 참조하는 내부 함수에서만 발생하는 현상이라고 앞에서 서술.
- 함수의 실행컨텍스트가 종료된 이후에도 렉시컬 환경이 가비지 컬렉터의 수집대상에서 제외되는 경우는

  - 지역변수를 참조하는 내부함수가 외부로 전달된 경우뿐 (아래와 같은 경우를 말함)

    ```jsx
    var outer = function () {
      var a = 1;
      var inner = function () {
        return ++a;
      };
      return inner;
    };

    var outer2 = outer();
    conlsole.log(outer2()); //2
    console.log(outer2()); //3
    ```

- 결국 어떤 함수에서 선언한 변수를 참조하는 내부변수에서만 발생하는 현상은 외부함수의 렉시컬 환경이 가비지 컬렉팅 되지 않는 현상을 말함.

### 클로저란 어떤함수 A에서 선언한 변수 a를 참조하는 내부함수 B를 외부로 전달할 경우 A의 실행컨텍스트가 종료된 이후에도 변수 a가 사라지지 않는 현상

## 외부함수로의 전달이 리턴만 의미하진 않는다!

```jsx
function(){
	var a = 0;
	var intervalId = null;
	var inner = function (){
		if(++a>=10){
			clearInterval(intervalId);
		}
		console.log(a);
	};
	intervalId = setInterval(inner,1000);

}();
```

- 콜백함수(setInterval) 내에서 지역변수를 참조할수 있다! (inner)

## 클로저와 메모리 소모

- 메모리 소모는 클로저의 본질적인 특성이다.
- 그러면 메모리를 어떻게 관리해야할까?
  - 클로저는 의도적으로 함수의 지역변수를 메모리를 소모함으로써 발생
  - 필요성 없어질때 메모리를 소모하지 않게 하면 된다.
  - 참조카운트를 0으로 하려면 0 → 참조형이 아닌 기본형 데이터할당하기!
- return에 의한 클로저의 메모리 해제

  ```jsx
  var outer = (function(){
  		var a = 1;
  		var inner = function(){
  			return ++a;
  		}
  		return inner;
  }();
  console.log(outer());
  outer=null; // outer식별자의 inner참조를 끊기

  ```

- setInterval에 의한 클로저의 메모리 해제

  ```jsx
  function(){
  	var a = 0;
  	var intervalId = null;
  	var inner = function (){
  		if(++a>=10){
  			clearInterval(intervalId);
  			inner = null;
  		}
  		console.log(a);
  	};
  	intervalId = setInterval(inner,1000);

  }();
  ```

## 클로저, 어디서 쓸까?

### 콜백함수 내부에서 외부 데이터를 사용하고자 할때

1. 콜백함수를 내부함수로 선언해서 외부 변수를 직접 참조하는 방식으로 클로저를 사용

```jsx
const fruits = ["apple", "banana"];

fruits.forEach(function (fruit) {
  var li = document.createElement("li");
  li.innerText = fruit;
  li.addEventListener("click", function () {
    // 여기에는 fruit이라는 외부 변수를 참조하고 있기에 클로저가 존재

    alert(fruit);
  });
});
```

- 개선된 버전

  ```jsx
  const fruits = ["apple", "banana"];

  const alertFruits = function (fruit) {
    alert(fruit);
  };

  fruits.forEach(function (fruit) {
    var li = document.createElement("li");
    li.innerText = fruit;
    li.addEventListener("click", alertFruits);
  });
  ```

1. bind를 활용하여 값을 직접 넘겨줌(클로저 발생x)

```jsx
const fruits = ["apple", "banana"];

const alertFruits = function (fruit) {
  alert(fruit);
};

fruits.forEach(function (fruit) {
  var li = document.createElement("li");
  li.innerText = fruit;
  li.addEventListener("click", alertFruits.bind(null, fruit));
});
```

1. 콜백함수를 고차함수로 바꾼다.

```jsx
const fruits = ["apple", "banana"];

const alertFruitsBuilder = function (fruit) {
  return function (fruit) {
    alert(fruit);
  };
};

fruits.forEach(function (fruit) {
  var li = document.createElement("li");
  li.innerText = fruit;
  li.addEventListener("click", alertFruitsBuilder(fruit));
});
```

+) 고차함수 : **함수를 인자로 받거나 결과로 반환하는 함수**

### 접근 권한 제어를 하고자 할 때

React에서 클로저
