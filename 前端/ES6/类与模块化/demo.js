{
    //ES5是通过构造函数来实现类的功能
    function Person(name, age) {
        this.name = name
        this.age = age
    }
    Person.prototype.sayHello = function() {
        console.log(`我叫${this.name}, 今年${this.age}岁了`)
    }

    const xm = new Person('小明', 12)
    xm.sayHello()
    console.log(xm)
}