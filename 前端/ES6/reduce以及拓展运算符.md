
<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [语法](#语法)
- [解析initialValue参数](#解析initialvalue参数)
  - [例子1](#例子1)
  - [例子2](#例子2)
- [应用](#应用)
  - [计算平均数](#计算平均数)
  - [计算数组中每个元素出现的次数](#计算数组中每个元素出现的次数)
  - [数组去重](#数组去重)
  - [将二维数组转化为一维](#将二维数组转化为一维)
  - [对象里的属性求和](#对象里的属性求和)

<!-- /code_chunk_output -->

## 语法
```javascript
arr.reduce(callback,[initialValue])
```
## 解析initialValue参数
### 例子1
```javascript
var arr = [1, 2, 3, 4];
var sum = arr.reduce(function(prev, cur, index, arr) {
	console.log(prev, cur, index);
	return prev + cur;
})
console.log(arr, sum);
```
```javascript
打印结果：
0 1 0
1 2 1
3 3 2
6 4 3
[1, 2, 3, 4] 10
```
这个例子index是从0开始的，第一次的prev的值是默认初始值0，数组长度是4，reduce函数循环4次。
### 例子2
```javascript
var  arr = [1, 2, 3, 4];
var sum = arr.reduce(function(prev, cur, index, arr) {
    console.log(prev, cur, index);
    return prev + cur;
}，0) //注意这里设置了初始值
console.log(arr, sum);
```
```javascript
打印结果：
0 1 0
1 2 1
3 3 2
6 4 3
[1, 2, 3, 4] 10
```
这个例子index是从0开始的，第一次的prev的值是我们设置的初始值0，数组长度是4，reduce函数循环4次。<br><br>

注意：如果这个数组为空，运用reduce是什么情况？
```javascript
var  arr = [];
var sum = arr.reduce(function(prev, cur, index, arr) {
	console.log(prev, cur, index);
	return prev + cur;
})
//报错，"TypeError: Reduce of empty array with no initial value"
```
我们设置了初始值就不会报错，如下：
```javascript
var  arr = [];
var sum = arr.reduce(function(prev, cur, index, arr) {
    console.log(prev, cur, index);
    return prev + cur;
}，0)
console.log(arr, sum); // [] 0
```
## 应用

### 计算平均数
```javascript
//...args拓展运算符，将函数参数转换成数组对象
function rest(...args){
	//将args的值从左到右依次作为参数进入function累加。0为初始参数
	return args.reduce((a,b)=>{
		return a+b;
	},0)/args.length
}
console.log(rest(1,2,3,34));//10
```

### 计算数组中每个元素出现的次数
```javascript
let names = ['Alice', 'Bob', 'Tiff', 'Bruce', 'Alice'];
let nameNum = names.reduce((pre,cur)=>{
  if(cur in pre){
    pre[cur]++
  }else{
    pre[cur] = 1 
  }
  return pre
},{})
console.log(nameNum); //{Alice: 2, Bob: 1, Tiff: 1, Bruce: 1}
```
### 数组去重
```javascript
let arr = [1,2,3,4,4,1]
let newArr = arr.reduce((pre,cur)=>{
    if(!pre.includes(cur)){
      return pre.concat(cur)
    }else{
      return pre
    }
},[])
console.log(newArr);// [1, 2, 3, 4]
```
### 将二维数组转化为一维
```javascript
let arr = [[0, 1], [2, 3], [4, 5]]
let newArr = arr.reduce((pre,cur)=>{
    return pre.concat(cur)
},[])
console.log(newArr); // [0, 1, 2, 3, 4, 5]
```
### 对象里的属性求和
```javascript
var result = [
    {
        subject: 'math',
        score: 10
    },
    {
        subject: 'chinese',
        score: 20
    },
    {
        subject: 'english',
        score: 30
    }
];

var sum = result.reduce(function(prev, cur) {
    return cur.score + prev;
}, 0);
console.log(sum) //60
```