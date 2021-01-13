
<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [简介](#简介)
- [函数](#函数)
  - [用户空间](#用户空间)
  - [内核空间](#内核空间)
- [定义数据](#定义数据)
  - [方法一](#方法一)
  - [方法二](#方法二)

<!-- /code_chunk_output -->

# 简介
ioctl 是设备驱动程序中设备控制接口函数，一个字符设备驱动通常会实现设备打开、关闭、读、写等功能，在一些需要细分的情境下，如果需要扩展新的功能，通常以增设 ioctl() 命令的方式实现。
# 函数
## 用户空间
```c
#include <sys/ioctl.h> 

int ioctl(int fd, int cmd, ...) ;
```

|参数|描述|
|:----|:----|
|fd|文件描述符|
|cmd|交互协议，设备驱动将根据 cmd 执行对应操作|
|可变参数|依赖 cmd 指定长度以及类型|
|返回值|成功0 失败<0|
## 内核空间
```c
long (*unlocked_ioctl) (struct file *, unsigned int, unsigned long);
long (*compat_ioctl) (struct file *, unsigned int, unsigned long);
```
- unlocked_ioctl，顾名思义，应该在无大内核锁（BKL）的情况下调用
- compat_ioctl，compat 全称 compatible（兼容的），主要目的是为 64 位系统提供 32 位 ioctl 的兼容方法，也是在无大内核锁的情况下调用。
- 在字符设备驱动开发中，一般情况下只要实现 unlocked_ioctl 函数即可，因为在 vfs 层的代码是直接调用 unlocked_ioctl 函数

用法:
```c
int xxx_ioctl(struct file *filp, unsigned int cmd, unsigned long flags){
    
    switch(cmd){
        case 1:
            .......
            break;
        case 2:
            ........
            break;
        .......
        default:
            .......
    }
}
const struct file_operations fops = {
	.unlocked_ioctl =	xxx_ioctl,
};
```
# 定义数据
## 方法一

可以直接定义一些整数来表示不同的命令，如：
```c
#define LED_ALL_ON  0x1
#define LED_ALL_OFF 0x2
#define LED_NUM_ON  0x3
#define LED_NUM_OFF 0x4
```
## 方法二
可以借助内核提供的算法获得命令的值，如： 
```c
/* used to create numbers */
#define _IO(type,nr)		_IOC(_IOC_NONE,(type),(nr),0)
```
|参数|描述|
|:----|:----|
|参数1：type|一个字符，可以用来表示不同的设备的命令|
|参数2：nr|一个整数，可以表示一个编号 |
```c
#define _IOR(type,nr,size)	_IOC(_IOC_READ,(type),(nr),(_IOC_TYPECHECK(size)))
#define _IOW(type,nr,size)	_IOC(_IOC_WRITE,(type),(nr),(_IOC_TYPECHECK(size)))
```
|参数|描述|
|:----|:----|
|参数1：type|一个字符，可以用来表示不同的设备的命令|
|参数2：nr|一个整数，可以表示一个编号 |
|参数3：size|表示ioctl中的第三个参数的实际类型|
例如： 
```c
//用户ioctl操作  不需要第三个参数
#define LED_ALL_ON  _IO('L', 0x1234)
#define LED_ALL_OFF _IO('L', 0x5678)

//用户ioctl操作  需要第三个参数时指定第三个参数类型
#define LED_NUM_ON  _IOW('L',0x1122, int)
#define LED_NUM_OFF _IOW('L', 0x3344, int)
```