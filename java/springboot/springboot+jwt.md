# 案例
## 依赖包
```
        <dependency>
            <groupId>io.jsonwebtoken</groupId>
            <artifactId>jjwt</artifactId>
            <version>0.7.0</version>
        </dependency>
```
## jwt工具类
```java
import io.jsonwebtoken.Claims;
import io.jsonwebtoken.Jwts;
import io.jsonwebtoken.SignatureAlgorithm;
import java.util.Date;

/**
 * jwt工具类
 *
 */
public class JwtUtils {

    /**
     * 过期时间一周
     */
    private static final long EXPIRE = 60000 * 60 * 24 * 7;

    /**
     * 加密密钥
     */
    private static final String SECRET = "1234567";

    /**
     * 令牌前缀
     */
    private static final String TOKEN_PREFIX = "cloud-xxx";

    /**
     * subject
     */
    private static final String SUBJECT = "gk.plf";

    /**
     * 根据用户信息生成令牌
     * @param wxUser
     * @return
     */
    public static String geneJsonWebToken(WxUser wxUser){
        String token = Jwts.builder().setSubject(SUBJECT)
                .claim("openid",wxUser.getOpenid())//往里边塞数据
                .claim("id",wxUser.getId())
                .setIssuedAt(new Date())//颁布时间
                .setExpiration(new Date(System.currentTimeMillis()+EXPIRE))//设置过期时间
                .signWith(SignatureAlgorithm.HS256,SECRET).compact();//设置签名方式
        token = TOKEN_PREFIX+token;                                         //token加上前缀。用于区分应用
        return token;
    }

    /**
     * 校验token方法
     * @param token
     * @return
     */
    public static Claims checkJWT(String token){
        try {
            final Claims claims = Jwts.parser()
                    .setSigningKey(SECRET)
                    .parseClaimsJws(token.replace(TOKEN_PREFIX,""))
                    .getBody();
            return claims;
        }catch (Exception e){
            return null;        //认证失败。过期或者有错误
        }
    }
}

```