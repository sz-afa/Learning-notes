### 1.let 与 const
#### var
var变量默认为全局变量
```javascript
var human '我是人';
console.log(human);
var human;
console.log(human);
```
`输出两句 我是人`

var存在变量提升例如：
```javascript
console.log(dad);
var dad = '我是爸爸';
```
浏览器实际运行为
```javascript
var dad
console.log(dad);
dad = '我是爸爸';
```
#### let
let变量只在当前块级作用域{}内有效
let变量不能被重复声明
let变量不存在变量提升即在声明变量前调用会报错
### const
const 在当前块级作用域内不允许被修改value
不存在变量提升
```javascript
const PI = 3.14
if(true){
    const PI=2.14
    console.log(PI);//输出2.14
}
console.log(PI)//输出3.14
```
### 暂存死区
```javascript
let dog = '狗1';
{
 console.log(dog);
 let dog = '狗2';
}
```
` 会报undefine错误 dog向上取不到，向下取不到。let替换const也是报错。`