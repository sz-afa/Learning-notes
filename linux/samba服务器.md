# samba安装
```shell
sudo apt-get install samba
```
# 配置
创建一个需要共享的目录，并修改权限
```shell
mkdir share
sudo chmod 777 share/ -R 
```
## 无密码
```shell
sudo vim /etc/samba/smb.conf
```
```
		[share]                  (共享文件夹的名字)
		path = /home/afa/share   (想共享的文件夹绝对路径)
		available = yes
		browseable = yes
		public = yes
		writable = yes
```
## 有密码
设置密码
```
sudo smbpasswd -a afa   (afa是用户名)
```
```shell
sudo vim /etc/samba/smb.conf
```
```
		[share]
		path = /home/afa/share
		available = yes
		browseable = yes
		writable = yes
		valid users = afa
```
# 重启服务
sudo /etc/init.d/smbd restart