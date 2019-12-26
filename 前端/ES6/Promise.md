### 1
```javascript
function f(){
    return new Promise(resolve => {
        setTimeout(function() {
            resolve();
        }, 1000);
    })
}

f()
    .then(function(){
        console.log(1);
        return f();
    })
    .then(function(){
        console.log(2);
        return f();
    })
```
