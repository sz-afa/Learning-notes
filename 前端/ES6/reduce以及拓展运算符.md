### 计算平均数
```javascript
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
	<title>Document</title>
</head>
<body>
	<script type="text/javascript">
        //...args拓展运算符，将函数参数转换成数组对象
		function rest(...args){
            //将args的值从左到右依次作为参数进入function累加。0为初始参数
		    return args.reduce(function(a,b){
		        return a+b;
		    },0)/args.length
		}
		console.log(rest(1,2,3,34));
	</script>
</body>
</html>
```
结果:输出10