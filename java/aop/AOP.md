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
对目标类的 **所有方法** 进行拦截。（不够灵活）
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
普通advice不够灵活。通常用带切入点的切面。**对符合匹配条件的类或方法进行拦截** <br>
实现类:
- DefaultPointcutAdvisorz 最常用的切面类型，通过任意Pointcut和Advice组合定义切面。
- `jdkRegexpMethdPointcut` 构造正则表达式切点。

### 案例1(无接口)
CustomerDao <span id = "CustomerDao"></span>
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
[CustomerDao.java](#CustomerDao)    <br>
[MyAroundAdvice.java](#MyAroundAdvice)<br>
[StudentDao.java](#StudenDao)<br>
[StudentDaoImpl.java](#StudenDaoImpl)<br>
[MyBeforeAdvice.java](#MyBeforeAdvice)<br>
applicationContext.xml
```xml
    <!--配置目标类-->
    <bean id="customerDao" class="demo6.CustomerDao" />
    <bean id="studentDao" class="demo6.StudentDaoImpl" />

    <!--配置通知-->
    <bean id="myAroundAdvice" class="demo6.MyAroundAdvice" />
    <bean id="myBeforeAdvice" class="demo6.MyBeforeAdvice" />

    <!--配置切面-->
    <bean id="myAdvisor" class="org.springframework.aop.support.RegexpMethodPointcutAdvisor">
        <!-- 为demo6.CustomerDao类的find方法织入myAroundAdvice环绕通知  -->
        <property name="pattern" value="demo6\.CustomerDao\.find" />
        <property name="advice" ref="myAroundAdvice" />
    </bean>

    <!--配置切面-->
    <bean id="myAdvisor2" class="org.springframework.aop.support.RegexpMethodPointcutAdvisor">
        <!-- 为demo6.StudentDao的find方法织入myBeforeAdvice前置通知  -->
        <property name="pattern" value="demo6\.StudentDao\.find" />
        <property name="advice" ref="myBeforeAdvice" />
    </bean>

    <!--自动根据切面信息产生代理-->
    <bean class="org.springframework.aop.framework.autoproxy.DefaultAdvisorAutoProxyCreator" />
```
测试类
```java
package demo6;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;

import javax.annotation.Resource;

@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration("classpath:applicationContext.xml")
public class SpringDemo6 {
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
环绕前
查询客户....
环绕后
前置通知
查找学生....
```
# AspcetJ
## 注解方式
### 说明
除了基本的spring核心包以外，还需要引入spring-aop、aopalliance、aspectjweaver、spring-aspects
applicationContext.xml开启aspectJ自动代理。
```xml
<aop:aspectj-autoproxy/>
```
#### 通知类型:
- @Before 前置通知，相当于BeforeAdvice
- @AfterRetruning 后置通知，相当于AfterReturningAdvice
- @Around环绕通知，相当于MethodInterceptor
- @AfterThrowing 异常抛出通知，相当于ThrowAdvice
- @After 最终final通知，不管是否异常，该通知都会执行
- `@DeclareParents 引介通知，相当于IntroductionInterceptor(不要求掌握)`
#### execution:
- 语法: execution(<访问修饰符>?<返回类型><方法名>(<参数>)<异常>) `<访问修饰符>是可有可无`
- 案例1: 匹配所有类public方法 execution( public * *(..) )
- 案例2: 匹配指定包下所有类方法 execution( * com.company.dao.*(..) ) `不包含子包`
- 案例3: 匹配指定包下所有类方法 execution( * com.company.dao..*(..) ) `包含子包`
- 案例4: 匹配指定类的所有方法 execution( * com.company.dao.UserDaoImpl.*(..) )
- 案例5: 匹配实现特定接口所有类方法 execution( * com.company.dao.UserDao+.*(..) )
- 案例6: 匹配所有save开头的方法 execution( * save*(..) )
### @Before前置通知
目标类ProductDao.java <span id = "aspectjProductDao"></span>
```java
package demo7;

public class ProductDao {
    public void  save(){
        System.out.println("保存商品..");
    }
    public void find(){
        System.out.println("查找商品..");
    }
}
```
通知类MyAspectJAnno.java
```java
package demo7;

import org.aspectj.lang.JoinPoint;
import org.aspectj.lang.annotation.Aspect;
import org.aspectj.lang.annotation.Before;

/**
 * 切面类
 */
@Aspect
public class MyAspectJAnno {

    @Before( value = " execution( * demo7.ProductDao.*(..) ) " )
    public void before(JoinPoint joinPoint){
        System.out.println("joinPoint :"+joinPoint);
        System.out.println("====前置通知====");
    }
}
```
测试类
```java
package demo7;

import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;


@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration("classpath:applicationContextAop.xml")
public class aspectjDemo1 {

    @Autowired
    private ProductDao productDao;

    @Test
    public void demo1(){
        productDao.save();
        productDao.find();
    }
}
```
applicationContextAop.xml
```xml
    <!--  开启AspectJ注解  -->
    <aop:aspectj-autoproxy/>

    <!--  目标类  -->
    <bean id="productDao" class="demo7.ProductDao" />

    <!--  定义切面  -->
    <bean class="demo7.MyAspectJAnno" />
```
控制台输出
```
joinPoint :execution(void demo7.ProductDao.save())
====前置通知====
保存商品..
joinPoint :execution(void demo7.ProductDao.find())
====前置通知====
查找商品..
```
### @AfterReturning后置通知，可以获取方法的返回值
MyAspectJAnno.java切面类
```java
package demo7;

import org.aspectj.lang.JoinPoint;
import org.aspectj.lang.annotation.AfterReturning;
import org.aspectj.lang.annotation.Aspect;
import org.aspectj.lang.annotation.Before;

/**
 * 切面类
 */
@Aspect
public class MyAspectJAnno {
    //匹配ProductDao的find方法,通过returning属性定义返回值，作为参数。
    @AfterReturning( value = " execution( * demo7.ProductDao.find(..) ) " ,returning = "result")
    public void afterReturning(JoinPoint joinPoint,Object result){
        System.out.println("====后置通知====");
        System.out.println("joinPoint:"+joinPoint);
        System.out.println("目标类返回值:"+result);
    }
}
```
[ProductDao.java目标类](#aspectjProductDao)<br>

测试类
```java
@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration("classpath:applicationContextAop.xml")
public class aspectjDemo1 {

    @Autowired
    private ProductDao productDao;

    @Test
    public void demo1(){
        productDao.find();
    }
}
```
控制台输出
```
查找商品..
====后置通知====
joinPoint:execution(String demo7.ProductDao.find())
目标类返回值:product
```
### @Around环绕通知，可以打断目标方法的执行。
MyAspectJAnno.java切面类
```java
    @Around( value = " execution( * demo7.ProductDao.find(..) )")
    public Object around(ProceedingJoinPoint joinPoint) throws Throwable {
        System.out.println("====环绕前====");
        Object obj = joinPoint.proceed();//执行目标方法，若该方法抛出异常环绕后通知可能不会执行（当前直接throws错误必定不会执行环绕后通知）。
        System.out.println("====环绕后====");
        System.out.println("obj:"+obj);
        return obj;
    }
```
[ProductDao.java目标类](#aspectjProductDao)<br>

测试类
```java
@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration("classpath:applicationContextAop.xml")
public class aspectjDemo1 {

    @Autowired
    private ProductDao productDao;

    @Test
    public void demo1(){
        productDao.find();
    }
}
```
控制台输出
```
====环绕前====
查找商品..
====环绕后====
obj:product
```
### @AfterThrowing 异常抛出通知（没有异常就不会执行）
ProductDao.java目标类
```java
package demo7;

public class ProductDao {
    public String find(){
        System.out.println("查找商品..");
        int i = 1/0;
        return "product";
    }
}
```
MyAspectJAnno.java切面类
```java
package demo7;

import org.aspectj.lang.ProceedingJoinPoint;
import org.aspectj.lang.annotation.*;

/**
 * 切面类
 */
@Aspect
public class MyAspectJAnno {
    @Around( value = " execution( * demo7.ProductDao.find(..) )" )
    public Object around(ProceedingJoinPoint joinPoint) throws Throwable {
        System.out.println("====环绕前====");
        Object obj = joinPoint.proceed();//执行目标方法，若该方法抛出异常，如果不是trycatch则环绕后通知不会执行。
        System.out.println("====环绕后====");
        System.out.println("obj:"+obj);
        return obj;
    }

    @AfterThrowing( value = " execution( * demo7.ProductDao.find(..) )" ,throwing = "error")
    public void afterThrowing(Throwable error){
        System.out.println("====异常抛出通知====");
        System.out.println(error.getMessage());
    }
}
```
测试类**同上**<br>
控制台输出
```
====环绕前====
查找商品..
====异常抛出通知====
/ by zero

java.lang.ArithmeticException: / by zero
```
### @After最终通知（无论目标方法是否有异常都会执行。类似finally）
ProductDao.java目标类**同上**<br>
MyAspectJAnno.java切面类
```java
package demo7;

import org.aspectj.lang.ProceedingJoinPoint;
import org.aspectj.lang.annotation.*;

/**
 * 切面类
 */
@Aspect
public class MyAspectJAnno {
    @Around( value = " execution( * demo7.ProductDao.find(..) )" )
    public Object around(ProceedingJoinPoint joinPoint) throws Throwable {
        System.out.println("====环绕前====");
        Object obj = joinPoint.proceed();//执行目标方法，若该方法抛出异常，如果不是trycatch则环绕后通知不会执行。
        System.out.println("====环绕后====");
        System.out.println("obj:"+obj);
        return obj;
    }

    @AfterThrowing( value = " execution( * demo7.ProductDao.find(..) )" ,throwing = "error")
    public void afterThrowing(Throwable error){
        System.out.println("====异常抛出通知====");
        System.out.println(error.getMessage());
    }

    @After( value = " execution( * demo7.ProductDao.find(..) )" )
    public void after(){
        System.out.println("====最终通知====");
    }
}
```
测试类**同上**<br>
控制台输出
```
====环绕前====
查找商品..
====最终通知====
====异常抛出通知====
/ by zero

java.lang.ArithmeticException: / by zero
```
### @Pointcut
#### 说明
- 在每个通知内定义切点，会造成工作量大不容易维护，对于重复的切点，可以使用@Pointcut进行定义
- 切点方法: private void 无参数方法，方法名为切点名。
- 当通知多个切点时，可以使用 || 进行连接
MyAspectJAnno.java
```java
    @Before( value = "myPointcut1() || myPointcut2()" )
    public void before(JoinPoint joinPoint){
        System.out.println("====前置通知===="+joinPoint);
    }

    @AfterReturning( value = "myPointcut1()" ,returning = "result")
    public void afterReturning(JoinPoint joinPoint,Object result){
        System.out.println("====后置通知===="+joinPoint);
        System.out.println("目标类返回值:"+result);
    }

    @Pointcut( value = " execution( * demo7.ProductDao.find(..) )" )
    private void myPointcut1(){}

    @Pointcut( value = " execution( * demo7.ProductDao.save(..) )" )
    private void myPointcut2(){}
```
测试类
```java
@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration("classpath:applicationContextAop.xml")
public class aspectjDemo1 {

    @Autowired
    private ProductDao productDao;

    @Test
    public void demo1(){
        productDao.save();
        productDao.find();
    }
}
```
控制台输出
```
====前置通知====execution(void demo7.ProductDao.save())
保存商品..
====前置通知====execution(String demo7.ProductDao.find())
查找商品..
====后置通知====execution(String demo7.ProductDao.find())
目标类返回值:product
```
## xml方式

### 说明
### 案例1