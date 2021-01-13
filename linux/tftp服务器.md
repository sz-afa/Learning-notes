# 创建tftp的工作目录，并修改权限
```shell
sudo mkdir /tftpboot
sudo chmod 777 /tftpboot
```
# 安装
sudo apt-get install tftp tftpd openbsd-inetd
# 打开配置文件添加内容
sudo vim /etc/inetd.conf 
```
tftp dgram udp wait nobody /usr/sbin/tcpd /usr/sbin/in.tftpd  /tftpboot（路径，可变）
```
# 打开(创建)文件/etc/default/tftpd-hpa
sudo vim /etc/default/tftpd-hpa
```
RUN_DAEMON="yes"
OPTIONS = "-l -s /tftpboot（路径，可变）"
```
# 重启服务
sudo /etc/init.d/openbsd-inetd restart
# 测试
tftp 192.168.7.4            登陆tftp服务器
tftp> get hello.c           下载文件（文件必须存在）
    Received 95 bytes in 0.1 seconds
tftp> quit      退出tftp服务器