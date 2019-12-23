实例对象对应标签中加入 v-cloak:
```javascript
<div id="wrap" v-cloak>
```

然后在css中给定义属性选择器 
```css
　　[v-cloak]{
　　display:none
    }
```