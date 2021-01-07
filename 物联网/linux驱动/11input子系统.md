
<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [简介](#简介)
  - [输入设备](#输入设备)
    - [种类](#种类)
    - [共同点](#共同点)
  - [输入子系统](#输入子系统)
    - [目的](#目的)
    - [特点](#特点)
    - [三层框架](#三层框架)
      - [input_handler层](#input_handler层)
        - [作用](#作用)
      - [input_core层](#input_core层)
        - [作用](#作用-1)
      - [input_dev层](#input_dev层)
        - [作用](#作用-2)
- [相关函数结构体](#相关函数结构体)
  - [input_dev结构体](#input_dev结构体)
- [实例](#实例)

<!-- /code_chunk_output -->

# 简介
input子系统也叫输入子系统
## 输入设备
### 种类
- key按键
- mouse
- touchscreen触摸屏
- joystick游戏摇杆
### 共同点
|差异化部分|通用部分|
|:----|:----|
|获取到数据(硬件信息)|上报给用户(xxx_read, copy_to_user, 阻塞)|
多个输入设备之间，有`差异化`的部分也有`通用`的部分。
linux内核就把`通用`的代码编写好，把`差异化`的代码留给驱动工程师。
于是就设计出了输入子系统
## 输入子系统
### 目的
为了让编程人员和驱动编程人员编程的时候简单统一
### 特点
- 兼容所有的输入设备
- 提供统一的编程驱动方法(实现差异化的硬件操作)
- 提供统一的应用操作接口: /dev/input/event0、event1
open("/dev/input/event0"), read(fd, strcut input_event):strcut input_event是一个`统一的数据包`
### 三层框架
#### input_handler层
数据处理者`(内核自带)`,内核代码实现:/drivers/input/evdev.c 
##### 作用
- 与应用层交互，实现设备操作接口
- 不知道如何从硬件中获取数据，但知道如何把数据传给应用层
#### input_core层
管理层`(内核自带)`,内核代码实现:/drivers/input/input.c
##### 作用
- 维护整个input子系统框架
- 为上下两层提供接口函数
- 申请设备号-------major=13，minor = 64开始
- 创建类
#### input_dev层
设备层`(自己编写)`,参考：Documentation/input/input-programming.txt
##### 作用
- 硬件初始化
- 知道如何获取硬件中的数据，但不知道如何把数据传给应用层
# 相关函数结构体
## input_dev结构体
```c
struct input_dev {
	//辅助信息
	const char *name;
	const char *phys;
	const char *uniq;
	struct input_id id;

	unsigned long propbit[BITS_TO_LONGS(INPUT_PROP_CNT)];

	unsigned long evbit[BITS_TO_LONGS(EV_CNT)];	//evbit: 设置当前设备类型: EV_KEY EV_REL EV_ABS
	unsigned long keybit[BITS_TO_LONGS(KEY_CNT)];	//keybit: 设置能产生的数据的按键,如：KEY_DOWN
	unsigned long relbit[BITS_TO_LONGS(REL_CNT)];	//relbit: 设置能产生的数据的相对坐标,如：x坐标
	unsigned long absbit[BITS_TO_LONGS(ABS_CNT)];	//relbit: 设置能产生的数据的绝对坐标,如：x坐标
	unsigned long mscbit[BITS_TO_LONGS(MSC_CNT)];
	unsigned long ledbit[BITS_TO_LONGS(LED_CNT)];

	struct device dev;
}
```
# 实例