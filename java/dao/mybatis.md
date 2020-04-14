
<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [导包](#导包)
- [mybatis配置文件](#mybatis配置文件)
- [LOG4J输出SQL语句](#log4j输出sql语句)
- [SqlSessionFactory](#sqlsessionfactory)
  - [说明](#说明)
- [SqlSession](#sqlsession)
  - [说明](#说明-1)
  - [常见方法](#常见方法)
- [MybatisUtils.java](#mybatisutilsjava)
  - [说明](#说明-2)
- [例子](#例子)
  - [案例1](#案例1)
- [IDEA下Maven无法读取src的XML文件](#idea下maven无法读取src的xml文件)

<!-- /code_chunk_output -->

# 导包
```xml
    <!--  mybatis核心包  -->
    <dependency>
      <groupId>org.mybatis</groupId>
      <artifactId>mybatis</artifactId>
      <version>3.4.4</version>
    </dependency>

    <!--  mybatis/spring整合包  -->
    <dependency>
      <groupId>org.mybatis</groupId>
      <artifactId>mybatis-spring</artifactId>
      <version>1.3.1</version>
    </dependency>

    <!--  mysql驱动包  -->
    <dependency>
      <groupId>mysql</groupId>
      <artifactId>mysql-connector-java</artifactId>
      <version>5.1.41</version>
      <scope>runtime</scope>
    </dependency>
```
# mybatis配置文件
mybatis-config.xml
```xml
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE configuration PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN"
        "http://mybatis.org/dtd/mybatis-3-config.dtd">

<configuration>
    <!--引入数据库配置文件-->
    <properties resource="jdbc.properties" />
    <!--  配置默认环境id为 mysql   -->
    <environments default="mysql">
        <!--mysql环境的详细配置-->
        <environment id="mysql">
            <!--   使用JDBC的事务管理     -->
            <transactionManager type="JDBC" />
            <dataSource type="POOLED">
                <property name="driver" value="${jdbc.driverClass}"/>
                <property name="url" value="jdbc:mysql://127.0.0.1:3306/mybatis" />
                <property name="username" value="${jdbc.username}"/>
                <property name="password" value="${jdbc.password}"/>
            </dataSource>
        </environment>
    </environments>

    <!--配置Mapper的位置-->
    <mappers>
        <mapper resource="mybatisDemo1/CustomerMapper.xml" />
    </mappers>
</configuration>
```
映射文件xxxMapper.xml
```xml
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE mapper PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN"
        "http://mybatis.org/dtd/mybatis-3-mapper.dtd">

<!--namespace表示命名空间(最好唯一)-->
<mapper namespace="">

</mapper>
```
# LOG4J输出SQL语句
导包
```xml
    <dependency>
      <groupId>log4j</groupId>
      <artifactId>log4j</artifactId>
      <version>1.2.17</version>
    </dependency>
    <dependency>
      <groupId>org.slf4j</groupId>
      <artifactId>slf4j-log4j12</artifactId>
      <version>1.7.25</version>
    </dependency>
    <dependency>
      <groupId>org.bgee.log4jdbc-log4j2</groupId>
      <artifactId>log4jdbc-log4j2-jdbc4.1</artifactId>
      <version>1.16</version>
    </dependency>
```
LOG4J配置文件log4j.properties
```properties
# 全局日志配置
log4j.rootLogger=ERROR, stdout
# MyBatis 日志配置
log4j.logger.XXXX=DEBUG
# 控制台输出
log4j.appender.stdout=org.apache.log4j.ConsoleAppender
log4j.appender.stdout.layout=org.apache.log4j.PatternLayout
log4j.appender.stdout.layout.ConversionPattern=%5p [%t] - %m%n
```
# SqlSessionFactory
## 说明
![](./img/SqlSessionFactory.png)<br>
![](./img/ssf2.png)<br>
# SqlSession
## 说明
![](./img/ss1.png)<br>
## 常见方法
![](./img/cxff.png)<br>
![](./img/cxs.png)<br>
![](./img/qtff.png)<br>
# MybatisUtils.java
## 说明
为了简化开发，通常在实际项目中会使用工具类来创建SqlSession
```java
package mybatisDemo1;

import org.apache.ibatis.io.Resources;
import org.apache.ibatis.session.SqlSession;
import org.apache.ibatis.session.SqlSessionFactory;
import org.apache.ibatis.session.SqlSessionFactoryBuilder;
import java.io.IOException;
import java.io.InputStream;


public class MybatisUtils {
    private static SqlSessionFactory sqlSessionFactory = null;

    //初始化SqlSessionFactory对象
    static {
        try {
            //使用Mybatis提供的Resources类加载Mybatis的配置文件
            InputStream is = Resources.getResourceAsStream("mybatis-config.xml");
            sqlSessionFactory = new SqlSessionFactoryBuilder().build(is);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    //禁止外部new该对象
    private MybatisUtils(){}

    //获取SqlSessison对象的静态方法
    public static SqlSession getSqlSession(){
        return sqlSessionFactory.openSession();
    }
}

```
# 例子
## 案例1
Customer.java
```java
package mybatisDemo1;

import lombok.Data;

@Data
public class Customer {
    private Integer id;
    private String username;
    private String jobs;
    private String phone;
}

```
CustomerMapper.xml
```xml
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE mapper PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN"
        "http://mybatis.org/dtd/mybatis-3-mapper.dtd">

<!--namespace表示命名空间(最好唯一)-->
<mapper namespace="mybatisDemo1.Customer">

    <!--根据id查客户-->
    <select id="findCustomerById" parameterType="Integer" resultType="mybatisDemo1.Customer">
        select * from t_customer where id = #{id}
    </select>

    <!--姓名模糊查询-->
    <select id="findCustomerByName" parameterType="String" resultType="mybatisDemo1.Customer">
        select * from t_customer where username like '%${value}%'
    </select>

    <!--添加客户信息 参数类型为对应的java bean -->
    <insert id="addCustomer" parameterType="mybatisDemo1.Customer">
        insert into t_customer(username,jobs,phone) values (#{username},#{jobs},#{phone})
    </insert>

    <!--更新客户信息-->
    <update id="updateCustomer" parameterType="mybatisDemo1.Customer" >
        update  t_customer set username=#{username},jobs=#{jobs},phone=#{phone} where id=#{id}
    </update>

    <!--删除客户-->
    <delete id="delCustomer" parameterType="Integer">
        delete from t_customer where id=#{id}
    </delete>

</mapper>


```
mybatis-config.xml添加CustomerMapper.xml
```xml
    <!--配置Mapper的位置-->
    <mappers>
        <mapper resource="mybatisDemo1/CustomerMapper.xml" />
    </mappers>
```
测试类
```java
package mybatisDemo1;

import org.apache.ibatis.session.SqlSession;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import java.util.List;

public class MybatisTest {

    SqlSession sqlSession;

    @Before
    public void init() throws Exception {
        //1 读取配置文件
        //InputStream inputStream = Resources.getResourceAsStream("mybatis-config.xml");
        //2 根据配置文件构建SqlSessionFactory
        //SqlSessionFactory sqlSessionFactory = new SqlSessionFactoryBuilder().build(inputStream);
        //3 通过根据配置文件构建SqlSessionFactory创建SqlSession
        //sqlSession = sqlSessionFactory.openSession();

        //123合并 -> 利用工具类生成sqlSession;
        sqlSession = MybatisUtils.getSqlSession();
    }

    @After
    public void closeSqlSession(){
        //最后关闭sqlSession
        sqlSession.close();
    }

    @Test
    public void findCustomerById() {

        //4 sqlSession执行映射文件中的sql语句，并返回映射结果
        //第一个参数是mapper的namespace（命名空间）+id(需要执行的sql操作的id) 。第二个参数是占位符数据
        Customer customer = sqlSession.selectOne("mybatisDemo1.Customer.findCustomerById",2);

        System.out.println(customer.toString());

    }

    @Test
    public void findCustomerByName(){
        //4 sqlSession执行映射文件中的sql语句，并返回映射结果
        //第一个参数是mapper的namespace（命名空间）+id(需要执行的sql操作的id) 。第二个参数是占位符数据
        List<Customer> list = sqlSession.selectList("mybatisDemo1.Customer.findCustomerByName","o");

        for(Customer customer : list){
            System.out.println(customer.toString());
        }

    }

    @Test
    public void addCustomer(){
        //4 创建客户对象
        Customer customer = new Customer();
        customer.setJobs("driver");
        customer.setUsername("toony");
        customer.setPhone("12306");

        //5 sqlSession执行映射文件中的插入语句，返回影响条数
        int rows = sqlSession.insert("mybatisDemo1.Customer.addCustomer",customer);
        if (rows > 0) {
            System.out.println("成功插入"+rows+"条数据");
        }else {
            System.out.println("插入失败!");
        }

        //6 事务提交
        sqlSession.commit();
    }

    @Test
    public void updateCustomer(){
        //先获取对象
        Customer customer = sqlSession.selectOne("mybatisDemo1.Customer.findCustomerById",2);
        customer.setUsername("alibaba");
        int rows = sqlSession.update("mybatisDemo1.Customer.updateCustomer",customer);
        if(rows > 0){
            System.out.println("您修改了"+rows+"行数据");
        }else {
            System.out.println("更新失败");
        }
        sqlSession.commit();
    }

    @Test
    public void delCustomer(){
        int rows = sqlSession.delete("mybatisDemo1.Customer.delCustomer",4);
        if(rows > 0){
            System.out.println("删除了"+rows+"条数据");
        }else{
            System.out.println("删除失败");
        }
        sqlSession.commit();
    }

}
```
# IDEA下Maven无法读取src的XML文件
我们需要在pom.xml文件里面去配置一下，让项目启动的时候能够去读取到src/main/java下面的配置文件，如下，在pom.xml里面加入下面代码：
```xml
<build>  
    <resources>  
        <resource>  
            <directory>src/main/java</directory>  
            <includes>  
                <include>**/*.xml</include>  
            </includes>  
        </resource>  
    </resources>  
</build>
```