
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
  - [struct input_dev输入设备](#struct-input_dev输入设备)
  - [input_allocate_device](#input_allocate_device)
  - [注册注销 input_dev](#注册注销-input_dev)
    - [注册](#注册)
    - [注销](#注销)
  - [上报数据](#上报数据)
  - [struct input_event上报数据包](#struct-input_event上报数据包)
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
## struct input_dev输入设备
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
## input_allocate_device
分配input_dev对象空间
```c
struct input_dev *input_allocate_device(void)
```
```c
ret = input_register_device(inputdev);
if(ret != 0){
    printk(KERN_ERR "input_register_device error \n");
    goto err_0;
}
```
## 注册注销 input_dev
### 注册
```c
int input_register_device(struct input_dev *dev)
```
```c
ret = input_register_device(inputdev);
if(ret != 0){
    printk(KERN_ERR "input_register_device error \n");
    goto err_0;
}
```
### 注销
```c
void input_unregister_device(struct input_dev *dev)
```
## 上报数据
一般写在中断里边
```c
  //功能：上报数据
  void input_event(struct input_dev *dev, unsigned int type, unsigned int code, int value)
  //参数1 ---- input_dev对象 
  //参数2 ---- 设备类型
  //参数3 ---- 上报的数据类型
  //参数4 ---- 数据的值
  
  //功能：上报按键数据
  static inline void input_report_key(struct input_dev *dev, unsigned int code, int value)
  //参数1 ---- input_dev对象 
  //参数2 ---- 上报的数据类型，KEY_DOWN KEY_UP等
  //参数3 ---- 数据的值
```
```c
static irqreturn_t key_irq_handler(int irq, void *dummy)
{
    int value;
	printk("------------^_^ %s---------------------\n",__FUNCTION__);
	value = gpio_get_value(S5PV210_GPH0(1));
    // 2,上报数据
	if(value){
		//松开
		input_event(inputdev,EV_KEY,KEY_DOWN,0);
		input_sync(inputdev);
	}else{
		//按下
		input_report_key(inputdev, KEY_DOWN, 1);
		input_sync(inputdev);
	}
    return IRQ_HANDLED;
}
```
## struct input_event上报数据包
```c
//上报的数据包
struct input_event {
  struct timeval time;
  __u16 type;		//设备类型
  __u16 code;		//数据类型
  __s32 value;	//数据的值
};
```

# 实例