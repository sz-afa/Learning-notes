# 生成公钥密钥
## ssh-keygen

```shell
ssh-keygen -t rsa -b 1024 -f output_keyfiles -C "备注"
```
|参数|说明|
|:----|:----|
|-b 1024|长度1024bit,最长4096|
|-t rsa|采用rsa加密方式,t=type|
|-f	|生成文件名,f=output_keyfiles|
|-C|备注，C=comment|
```shell
ssh-keygen -t rsa -b 4096  -C "email"
```
```shell
Generating public/private rsa key pair.
Enter file in which to save the key (/home/schacon/.ssh/id_rsa):
Created directory '/home/schacon/.ssh'.
Enter passphrase (empty for no passphrase):
Enter same passphrase again:
Your identification has been saved in /home/schacon/.ssh/id_rsa.
Your public key has been saved in /home/schacon/.ssh/id_rsa.pub.
The key fingerprint is:
d0:82:24:8e:d7:f1:bb:9b:33:53:96:93:49:da:9b:e3 schacon@mylaptop.local
```
首先 ssh-keygen 会确认密钥的存储位置（默认是 .ssh/id_rsa），然后它会要求你输入两次密钥口令。 如果你不想在使用密钥时输入口令，将其留空即可。 然而，如果你使用了密码，那么请确保添加了 -o 选项，它会以比默认格式更能抗暴力破解的格式保存私钥。
# 公钥上传到服务器
```shell
cat id_rsa.pub >> ~/.ssh/authorized_keys

chmod 600 ~/.ssh/authorized_keys
chmod 700 ~/.ssh

sudo vim /etc/ssh/sshd_config
```
```
RSAAuthentication yes
PubkeyAuthentication yes

PermitRootLogin no //禁止root登录
PasswordAuthentication yes //允许密码登录，根据你的情况设置
```
```shell
service sshd restart
```

# 连接服务器
## 方法1
```shell
ssh -i ~/.ssh/id_rsa -p 22 user@yourservername
```
## 方法2
修改~/.ssh/config
```
Host server_alias(你的服务器别名)
HostName test.com/192.168.1.1(域名或IP)
Port 22
User user
Identifier id_rsa
```
```shell
ssh server_alias
```