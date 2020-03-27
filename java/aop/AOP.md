[TOC]

# 概念
## AOP实现方式

### 预编译

- AspectJ

### 运行期动态代理
- SpringAOP、JbossAOP

## AOP相关概念
| 名称                    | 说明                                                                                                         |
| :---------------------- | :----------------------------------------------------------------------------------------------------------- |
| 切面(Aspect)            | 一个关注点的模块化，这个关注点可能会横切多个对象                                                             |
| 连接点(Joinpoint)       | 程序执行过程中的某个特定的点                                                                                 |
| 通知(Advice)            | 在切面的某个特定的连接点上执行的动作                                                                         |
| 切入点(Pointcut)        | 匹配连接点的断言，在AOP中通知和一个切入点表达式关联                                                          |
| 引入(Introduction)      | 在不修改类的前提下，为类添加新的方法和属性                                                                   |
| 目标对象(Target Object) | 被一个或者多个切面所通知的对象                                                                               |
| AOP代理(AOP Proxy)      | AOP框架创建的对象，用来执行切面锲约（aspect contract）(包括通知方法执行等功能)                               |
| 织入(Weaving)           | 把切面连接到其他的应用程序类型或者对象上，并创建一个被通知的对象，分为：编译时织入，类加载时织入，执行时织入 |

## Advice类型
| 名称                                  | 说明                                                                   |
| :------------------------------------ | :--------------------------------------------------------------------- |
| 前置通知(Before advice)               | 在某连接点之前执行的通知，但不能阻止连接点前的执行（除非抛出一个异常） |
| 返回后通知(After returning advice)    | 在某连接点正常完成后执行的通知                                         |
| 抛出异常后通知(After throwing advice) | 在方法抛出异常退出时执行的通知                                         |
| 后通知(After(finally) advice)         | 当某连接点退出时执行的通知（无论是正常返回还是异常退出）               |
| 环绕通知(Around Advice)               | 包围一个连接点的通知                                                   |

## 有接口和无接口的Spring AOP实现区别
- spring AOP 默认使用标准的JavaSE 动态代理作为AOP 代理这使得任何接口（ 或者接口集） 都可以被代理
- Spring AOP 中也可以使用GLIB代理（如果一个业务对象并没有实现一个接口）
- CGLIB是字节码层面的类继承。jdk动态代理是接口代理，无接口无法使用


# SpringAOP

## 1.普通Advice
### 说明
**对目标类的所有方法进行拦截。（不够灵活）**
### 案例1
StudenDao接口
```java
public interface StudentDao {
    public void find();
}
```
StudenDao实现类StudenDaoImpl
```java
public class StudentDaoImpl implements StudentDao{
    @Override
    public void find() {
        System.out.println("查找学生");
    }
}
```
前置通知类
```java
import org.springframework.aop.MethodBeforeAdvice;
import java.lang.reflect.Method;

public class MyBeforeAdvice implements MethodBeforeAdvice {
    @Override
    public void before(Method method, Object[] objects, Object o) throws Throwable {
        System.out.println("前置通知....");
    }
}
```
applicationContext.xml
```xml
    <!--spring配置目标类-->
    <bean id="studentDao" class="demo3.StudentDaoImpl" />

    <!--前置通知类型-->
    <bean id="myBeforeAdvice" class="demo3.MyBeforeAdvice" />

    <!--Spring的AOP产生代理对象-->
    <bean id="studentDaoProxy" class="org.springframework.aop.framework.ProxyFactoryBean">
        <!--配置目标类-->
        <property name="target" ref="studentDao" />
        <!--传入需要增强的接口 -->
        <property name="proxyInterfaces" value="demo3.StudentDao"/>
        <!-- 需要织入目标类的通知类 -->
        <property name="interceptorNames" value="myBeforeAdvice"/>

        <!-- 返回代理是否是单例 ， 默认为单例-->
        <!-- <property name="singleton" value="true"/>-->

        <!-- 是否对类代理而不是接口，true为启用CGLib代理， 默认为false -->
        <!-- <property name="proxyTargetClass" value="false"/>-->

        <!-- true为强制开启CGLib代理-->
        <!-- <property name="optimize" value="true"/>-->
    </bean>
```
测试类
```java
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.test.context.ContextConfiguration;v
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;

import javax.annotation.Resource;

@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration("classpath:applicationContext.xml")
public class SpringDemo3 {

    @Resource(name = "studentDaoProxy")
    private StudentDao studentDao;

    @Test
    public void demo3(){
        studentDao.find();
    }
}
```
控制台输出
```
前置通知....
查找学生
```

## 2.PointcutAdvice
### 说明
### 案例1