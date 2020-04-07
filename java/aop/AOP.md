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
### 案例1(有接口)
StudenDao接口 <span id = "StudenDao"></span>
```java
public interface StudentDao {
    public void find();
}
```
StudenDao实现类StudenDaoImpl <span id = "StudenDaoImpl"></span>
```java
public class StudentDaoImpl implements StudentDao{
    @Override
    public void find() {
        System.out.println("查找学生");
    }
}
```
MyBeforeAdvice.java前置通知类  <span id = "MyBeforeAdvice"></span>
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
普通advice不够灵活。通常用带切入点的切面。**对符合条件的方法进行拦截** <br>
实现类:
- DefaultPointcutAdvisorz 最常用的切面类型，通过任意Pointcut和Advice组合定义切面。
- `jdkRegexpMethdPointcut` 构造正则表达式切点。

### 案例1(无接口)
CustomerDao接口 <span id = "CustomerDao"></span>
```java
package demo4;

public class CustomerDao {
    public void find(){
        System.out.println("查询客户....");
    }
}
```
MyAroundAdvice环绕通知类<span id = "MyAroundAdvice"></span>
```java
package demo4;

import org.aopalliance.intercept.MethodInterceptor;
import org.aopalliance.intercept.MethodInvocation;

public class MyAroundAdvice implements MethodInterceptor {
    @Override
    public Object invoke(MethodInvocation methodInvocation) throws Throwable {
        System.out.println("环绕前");
        Object obj = methodInvocation.proceed();
        System.out.println("环绕后");
        return obj;
    }
}
```
applicationContext.xml
```xml

    <!--配置目标类-->
    <bean id="customerDao" class="demo4.CustomerDao" />

    <!--配置通知-->
    <bean id="myAroundAdvice" class="demo4.MyAroundAdvice" />

    <!--一般的切面是使用通知作为切面的，因为要对目标类的某个方法进行增强就需要配置一个带有切入点的一个切面-->
    <bean id="myAdvice" class="org.springframework.aop.support.RegexpMethodPointcutAdvisor" >
        <!--  正则表达式匹配需要织入的方法  .任意字符 *任意次数   -->
        <property name="pattern" value=".*find.*"/>
        <!--  匹配多个正则表达式   -->
        <!-- <property name="patterns" value=".*find.*,.*save.*" />  -->
        <property name="advice" ref="myAroundAdvice" />
    </bean>


    <!--配置产生代理-->
    <bean id="customerDaoProxy" class="org.springframework.aop.framework.ProxyFactoryBean">
        <property name="target" ref="customerDao" />
        <property name="proxyTargetClass" value="true" />
        <property name="interceptorNames" value="myAdvice" />
    </bean>
```
控制台输出
```
环绕前
查询客户....
环绕后
```
## 3.自动创建代理
### 说明
- 上述两个案例中，每个代理都是通过ProxyFactoryBean织入切面代理，在实际开发中，非常多的bean都配置ProxyFactoryBean工作量太大
- 解决方案: **自动代理**:
    1. BeanNameAutoProxyCreator `根据bean名称创建代理`
    1. DefaultAdvisorAutoProxyCreator `根据Advisor本身包含信息创建代理`
    1. AnnotationAwareAspectJAutoProxyCreator 基于bean中的AspcetJ注解进行代理
### 案例1(BeanNameAutoProxyCreator)
[CustomerDao.java](#CustomerDao)    <br>
[MyAroundAdvice.java](#MyAroundAdvice)<br>
[StudentDao.java](#StudenDao)<br>
[StudentDaoImpl.java](#StudenDaoImpl)<br>
[MyBeforeAdvice.java](#MyBeforeAdvice)<br>
applicationContext.xml
```xml
    <!--配置目标类-->
    <bean id="customerDao" class="demo5.CustomerDao" />
    <bean id="studentDao" class="demo5.StudentDaoImpl" />

    <!--配置通知-->
    <bean id="myAroundAdvice" class="demo5.MyAroundAdvice" />
    <bean id="myBeforeAdvice" class="demo5.MyBeforeAdvice" />

    <!--配置产生代理-->
    <bean id="customerDaoProxy" class="org.springframework.aop.framework.autoproxy.BeanNameAutoProxyCreator">
        <!-- 对所有Dao结尾的目标类进行代理  -->
        <property name="beanNames" value="*Dao" />
        <property name="interceptorNames" >
            <list>
                <value>myBeforeAdvice</value>
                <value>myAroundAdvice</value>
            </list>
        </property>
    </bean>
```
测试类
```java
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;

import javax.annotation.Resource;

@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration("classpath:applicationContext3.xml")
public class SpringDemo5 {

    @Resource(name = "customerDao")
    private CustomerDao customerDao;

    @Resource(name = "studentDao")
    private StudentDao studentDao;

    @Test
    public void demo(){
        customerDao.find();
        studentDao.find();
    }

}
```
控制台输出
```
前置通知....
环绕前
查询客户....
环绕后
前置通知....
环绕前
查找学生
环绕后
```
### 案例2 (DefaultAdvisorAutoProxyCreator)
# AspcetJ