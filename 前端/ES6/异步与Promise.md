
<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [异步](#异步)
- [Promise](#promise)
  - [介绍](#介绍)
    - [1回调地狱及改造](#1回调地狱及改造)
    - [2catch捕获异常](#2catch捕获异常)
    - [3Promise.all](#3promiseall)
    - [4Promise.race](#4promiserace)

<!-- /code_chunk_output -->

# 异步
就是非阻塞执行代码
# Promise
## 介绍
为了解决"回调地狱"的问题，提出了Promise对象
### 1回调地狱及改造
```javascript
{
    function ajax(cb){
        setTimeout(()=>{
            cb && cb(()=>{
                console.log('任务2')
            })
        }, 1000)
    }

    ajax((cb2)=>{
        console.log('任务1')
        setTimeout(()=>{
            cb2 && cb2()
        }, 1000)
    })
}
{
    {
    function ajax(){
        return new Promise((resolve, reject) => {
            setTimeout(()=>{
                resolve()
            },1000)
        }) 
    }
    ajax()
        .then(()=>{
            console.log('任务1')
            return ajax()
        }).then(()=>{
            console.log('任务2')
        })
}
}
```
### 2catch捕获异常
```javascript
    //使用catch捕获错误
    function chechNumber(num){
        return new Promise((resolve, reject) => {
            if(typeof (num) === 'number'){
                resolve(num)
            }else{
                const err = new Error('请输入数字')
                reject(err)
            }
        })
    }
    chechNumber(2)
        .then(num => {
            console.log(num,'是一个数字')
        })
        .catch(err=>{
            console.log(err)
        })
```
### 3Promise.all
```javascript
    const imgURL1 = 'httpxxx'
    const imgURL2 = 'httpxxx'
    const imgURL3 = 'httpxxx'

    function getImage(url){
        return new Promise((resolve, reject)=>{
            const img = document.createElement('img')
            img.src = url
            img.onload = () => resolve(img)
            img.onerror = () => reject(img)
        })
    }

    function showImage(imgs){
        imgs.forEach(element => {
            document.body.appendChild(element)
        })
    }

    //所有promise完成后 才执行下一步
    Promise.all([getImage(imgURL1), getImage(imgURL2), getImage(imgURL3)]).then(showImage)
```
### 4Promise.race
```javascript
{
    const imgURL1 = 'https://file.xdclass.net/xdclass/xdclass_logo.png'
    const imgURL2 = 'https://file.xdclass.net/video/2020-12%20%E5%AE%98%E7%BD%91%E8%B6%85100k%E4%B8%BB%E5%9B%BE%E6%9B%B4%E6%96%B0/%E4%B8%BB%E5%9B%BE/36-es.jpg'
    const imgURL3 = 'https://xdvideo-file.oss-cn-shenzhen.aliyuncs.com/video/%E5%AE%98%E7%BD%91%E8%BD%AE%E6%92%AD%E5%9B%BE/2020%E5%B9%B412%E6%9C%88/lbt-yzx.jpeg'

    function getImage(url){
        return new Promise((resolve, reject)=>{
            const img = document.createElement('img')
            img.src = url
            img.onload = () => resolve(img)
            img.onerror = () => reject(img)
        })
    }

    function showFristImage(img){
        document.body.appendChild(img)
    }

    //有一个promise响应后就立即处理
    Promise.race([getImage(imgURL1), getImage(imgURL2), getImage(imgURL3)]).then(getFristImage)
}
```