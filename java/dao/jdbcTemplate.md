
<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [Account](#account)
- [AccountDao接口](#accountdao接口)
- [AccountDao接口实现类AccountDaoImpl](#accountdao接口实现类accountdaoimpl)
- [jdbc.property](#jdbcproperty)
- [applicationContext.xml](#applicationcontextxml)
- [测试类](#测试类)

<!-- /code_chunk_output -->

## Account
```java
@Setter
@Getter
@ToString
public class Account {
    private Integer id;
    private String username;
    private Double balance;//账户余额
}
```
## AccountDao接口
```java
public interface AccountDao {
    //添加
    public int addAccount(Account account);
    //更新
    public int updateAccount(Account account);
    //删除
    public int deleteAccount(int id);
    //指定id查找
    public Account findAccountById(int id);
    //获取所有用户
    public List<Account> findAllAccount();
}
```
## AccountDao接口实现类AccountDaoImpl
```java
import org.springframework.jdbc.core.BeanPropertyRowMapper;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.core.RowMapper;
import java.util.List;

public class AccountDaoImpl implements AccountDao {

    //声明JdbcTemplate
    private JdbcTemplate jdbcTemplate;

    @Override
    public int addAccount(Account account) {
        String sql = "insert into account(username,balance) value(?,?)";
        //定义数组存放参数
        Object[] obj = new Object[]{
                account.getUsername(),
                account.getBalance(),
        };
        int num = this.jdbcTemplate.update(sql,obj);
        return num;
    }

    @Override
    public int updateAccount(Account account) {
        //定义SQL
        String sql = "update account set username=? , balance=?  where id = ?";
        Object[] params = new Object[]{
            account.getUsername(),
            account.getBalance(),
            account.getId()
        };
        int num = this.jdbcTemplate.update(sql,params);
        return num;
    }

    @Override
    public int deleteAccount(int id) {
        String sql = "delete from account where id=?";
        int num = this.jdbcTemplate.update(sql,id);
        return num;
    }

    @Override
    public Account findAccountById(int id) {
        String sql = "select * from account where id=?";
        RowMapper<Account> rowMapper = new BeanPropertyRowMapper<Account>(Account.class);
        //将ID绑定到SQL语句中，通过该RowMapper返回一个Object类型的单行记录
        return jdbcTemplate.queryForObject(sql,rowMapper ,id);
    }

    //查询所有账户信息
    @Override
    public List<Account> findAllAccount() {
        String sql = "select * from account ";
        RowMapper<Account> rowMapper = new BeanPropertyRowMapper<Account>(Account.class);

        //执行静态的sql
        return this.jdbcTemplate.query(sql,rowMapper);

    }

    public JdbcTemplate getJdbcTemplate() {
        return jdbcTemplate;
    }
    public void setJdbcTemplate(JdbcTemplate jdbcTemplate) {
        this.jdbcTemplate = jdbcTemplate;
    }
}
```
## jdbc.property
```property
#database connection config
jdbc.driverClass=com.mysql.jdbc.Driver
jdbc.url=jdbc:mysql://127.0.0.1:3306/database_name
jdbc.username=root
jdbc.password=password
```

## applicationContext.xml
```xml
    <!--    1配置数据源   -->
    <bean id="dataSource" class="org.springframework.jdbc.datasource.DriverManagerDataSource">
        <!-- 数据库驱动  -->
        <property name="driverClassName" value="${jdbc.driverClass}"/>
        <!-- 链接数据库的URL  -->
        <property name="url" value="${jdbc.url}"/>
        <!-- 账号  -->
        <property name="username" value="${jdbc.username}"/>
        <!-- 密码  -->
        <property name="password" value="${jdbc.password}" />
    </bean>

    <!--    2配置JdbcTemplate   -->
    <bean id="jdbcTemplate" class="org.springframework.jdbc.core.JdbcTemplate">
        <!-- 默认必须使用数据源  -->
        <property name="dataSource" ref="dataSource"/>
    </bean>

    <!--   定义id为account的Bean   -->
    <bean id="accountDao" class="daoDemo1.AccountDaoImpl">
        <!-- JdbcTemplate注入 accountDao  -->
        <property name="jdbcTemplate" ref="jdbcTemplate" />
    </bean>
```

## 测试类
```java
import org.junit.Before;
import org.junit.Test;
import org.springframework.context.ApplicationContext;
import org.springframework.context.support.ClassPathXmlApplicationContext;
import org.springframework.jdbc.core.JdbcTemplate;
import java.util.List;

public class jdbcTemplateTest {
    //声明配置文件
    ApplicationContext applicationContext;
    //声明JDBCTemplate实例
    JdbcTemplate jdbcTemplate;
    //声明AccountDao实例
    AccountDao accountDao;

    @Before
    public void init(){
        //加载配置文件
        applicationContext = new ClassPathXmlApplicationContext("classpath:applicationContextTest.xml");
        //获取JDBCTemplate实例
        jdbcTemplate = (JdbcTemplate) applicationContext.getBean("jdbcTemplate");
        //获取AccountDao实例
        accountDao = (AccountDao) applicationContext.getBean("accountDao");
    }
    @Test
    public void mainTest(){
        //使用execute方法执行sql语句,创建用户账户管理表account
        jdbcTemplate.execute(" create table account("+
                "id int primary key auto_increment,"+
                "username varchar(50),"+
                "balance double) ");
        System.out.println("account创建成功");
    }

    @Test
    public void addTest(){

        Account account = new Account();
        account.setUsername("tom");
        account.setBalance(100.0);

        int num = accountDao.addAccount(account);
        if (num > 0) {
            System.out.println("插入成功");
        }else{
            System.out.println("插入失败");
        }
        
    }

    @Test
    public void updateTest(){
        Account account = new Account();
        account.setId(3);
        account.setUsername("hans");
        account.setBalance(1200.00);

        int num = accountDao.updateAccount(account);
        if (num >= 0) {
            System.out.println("成功修改数据 " + num + " 条");
        }else {
            System.out.println("修改失败");
        }
    }

    @Test
    public void finAccountByIdTest(){
        Account queryAccount = accountDao.findAccountById(1);
        System.out.println(queryAccount.toString());
    }

    @Test
    public void finAllAccountTest(){
        List<Account> account = accountDao.findAllAccount();
        for(Account temp : account){
            System.out.println(temp);
        }
        //遍历所有用户
    }
}
```
