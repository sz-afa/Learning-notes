# 安装nfs服务器
```shell
sudo apt-get install nfs-kernel-server
```
# 重启portmap服务
```shell
sudo /etc/init.d/portmap restart
```
# 添加目标系统的根文件系统映射目录
```shell
sudo vim /etc/exports
```
```
/opt/rootfs  *(subtree_check,rw,no_root_squash,async) 
```
```shell
sudo exportfs -a
```
# 重启服务
```shell
sudo  /etc/init.d/nfs-kernel-server	restart
```
# 开启nfs v2 uboot支持nfsv2
```shell
sudo vim/etc/default/nfs-kernel-server
```
```
RPCNFSDOPTS="--nfs-version 2,3,4"
```