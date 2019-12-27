
## SpringBoot接口Http协议
### SpringBoot2.xHTTP请求配置
#### SpringBoot2.xHTTP请求注解讲解和简化注解配置技巧

* @RestController and @RequestMapping是springMVC的注解，不是springboot特有的
* @RestController = @Controller+@ResponseBody
* @SpringBootApplication =@Configuration+@EnableAutoConfiguration+@ComponentScan​localhost:8080


### 请求
#### get请求

@GetMapping = @RequestMapping(method = RequestMethod.GET) 
#### post请求
@PostMapping = @RequestMapping(method = RequestMethod.POST)
#### put请求
@PutMapping = @RequestMapping(method = RequestMethod.PUT)
#### delete请求
@DeleteMapping = @RequestMapping(method = RequestMethod.DELETE)

### SpringBoot2.x目录文件结构讲解
#### SpringBoot目录文件结构和官方推荐的目录规范
* 目录:
    1. src/main/java：存放代码
    2. src/main/resources:
        - static: 存放静态文件，比如 css、js、image, （访问方式 http://localhost:8080/js/main.js）
        - templates:存放静态页面jsp,html,tpl
        - config:存放配置文件,如：application.properties
* 引入依赖 Thymeleaf:
    ```xml
    <dependency>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-thymeleaf</artifactId>
    </dependency>
    ​
    //注意：如果不引人这个依赖包，html文件应该放在默认加载文件夹里面，
    //比如resources、static、public这个几个文件夹，才可以访问
    ```
* 同个文件的加载顺序,静态资源文件 Spring Boot 默认会挨个从
    1. META/resources >
    2. resources >
    3. static >
    4. public >
<br>里面找是否存在相应的资源，如果有则直接返回。

* application.properties默认配置:
    ```properties
    spring.resources.static-locations = classpath:/META-INF/resources/,classpath:/resources/,classpath:/static/,classpath:/public/
    ```