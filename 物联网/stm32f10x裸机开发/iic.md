
<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [概述](#概述)
  - [基本操作](#基本操作)
    - [1 初始化](#1-初始化)
    - [2 起始结束信号](#2-起始结束信号)
    - [3 应答和非应答](#3-应答和非应答)
    - [4 从总线收发1字节数据](#4-从总线收发1字节数据)
      - [发](#发)
      - [收](#收)
  - [AT24c0x](#at24c0x)
    - [控制字节](#控制字节)
    - [写数据](#写数据)
    - [读数据](#读数据)

<!-- /code_chunk_output -->
# 概述
飞利浦公司开发，两根总线半双工。两根全都上拉。可多个主从设备。<br>SCL为高时,SDA数据有效（数据为1 或 0 传输数据时SDA状态不可改变，否则视为起始或结束）。<br>SCL为低时，SDA数据无效此时可改变数据（1 0之间变化）
## 基本操作
### 1 初始化
```c
#define  SDA_HIGH             GPIO_SetBits(GPIOB, GPIO_Pin_7)
#define  SDA_LOW              GPIO_ResetBits(GPIOB, GPIO_Pin_7)
#define  SDA_READ             GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)

#define  SCL_HIGH             GPIO_SetBits(GPIOB, GPIO_Pin_6)
#define  SCL_LOW              GPIO_ResetBits(GPIOB, GPIO_Pin_6)

#define  noACK            1
#define  ACK              0

void IIC_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

	GPIO_InitStruct.GPIO_Mode   =GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Pin    =GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Speed  =GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void  IIC_Sdaout(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode   =GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Pin    =GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Speed  =GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void IIC_Sdain(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode   =GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin    =GPIO_Pin_7;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
}
```
### 2 起始结束信号
![](./img/start&stop.png)<br>
SCL为1时SDA的状态改变就是起始和结束信号
`开始信号`:SCL 1、SDA 1 -> 0
`结束信号`:SCL 1、SDA 0 -> 1
```c
void IIC_start(void)
{
	SCL_LOW;
	IIC_Sdaout();

	SDA_HIGH;
	SCL_HIGH;
	SysTicks_DelayNus(5);
	SDA_LOW;	
	SysTicks_DelayNus(5);

	SCL_LOW;
}

void IIC_stop(void)
{
	SCL_LOW;
	IIC_Sdaout();	

	SDA_LOW;
	SCL_HIGH;
	SysTicks_DelayNus(5);
	SDA_HIGH;
	SysTicks_DelayNus(5);
}
```
### 3 应答和非应答
![](./img/ack&noack.png)<br>
两种状态。具体意义跟设备有关。
```c
void IIC_sendack(void)
{
	SCL_LOW;
	IIC_Sdaout();	

	SDA_LOW;
	SCL_HIGH;
	SysTicks_DelayNus(5);	
	
	SCL_LOW;
}

void IIC_sendNoack(void)
{
	SCL_LOW;
	IIC_Sdaout();

	SDA_HIGH;
	SCL_HIGH;
	SysTicks_DelayNus(5);	
	
	SCL_LOW;	
}

uint8_t  IIC_waitACK(void)
{
	uint8_t count=0;
	SCL_LOW;
	IIC_Sdain();

	SCL_HIGH;
	SysTicks_DelayNus(5);
	while(SDA_READ)
	{
		count++;
		if(count>250)
		{
			IIC_stop();
			return noACK;
		}
	}
	SCL_LOW;
	return  ACK;
}
```
### 4 从总线收发1字节数据
![](./img/data.png)<br>
#### 发
```c
void  IIC_SendByteData(uint8_t data)
{
	uint8_t i;
	SCL_LOW;
	IIC_Sdaout();

	for(i=0;i<8;i++)
	{
		if(data&0X80)
		{
			SDA_HIGH;
		}
		else
		{
			SDA_LOW;
		}
		SCL_HIGH;
		SysTicks_DelayNus(5);	
		data=data<<1;
		SCL_LOW;
		SysTicks_DelayNus(5);		
	}
	SCL_LOW;
}
```
#### 收
```c
uint8_t  IIC_RxByteData(uint8_t ackflag)
{
	uint8_t i,data;
	SCL_LOW;
	IIC_Sdain();

	for(i=0;i<8;i++)
	{
		data=data<<1;
		SCL_HIGH;
		if(SDA_READ)
		{
			data=data|0x01;
		}
		SysTicks_DelayNus(5);
		SCL_LOW;
		SysTicks_DelayNus(5);
	}
	SCL_LOW;	
	if(ackflag==ACK)
	{
		IIC_sendack();
	}
	else
	{
		IIC_sendNoack();
	}
	return  data;
}
```
## AT24c0x
![](./img/at24c-hw.PNG)<br>
A2 = NC管脚
### 控制字节
![](./img/ctrlByte.PNG)<br>
根据原理图A2 A1 A0均为0
0xA0为读操作 10100000
0xA1为写操作 10100001

### 写数据
```c
void At24c02_WriteByteData(uint8_t addr,uint8_t data)
{
	IIC_start();    //起始信号

	IIC_SendByteData(0XA0);  //发送设备地址
	if(ACK !=IIC_waitACK())  //等待接收方回应
	{
		IIC_stop();         //反应非应答＃，停止IIC
		return;             //跳出循环
	}

	IIC_SendByteData(addr);  //发送要在设备中写入的内存单元地址
	if(ACK !=IIC_waitACK())   //等待接收方回应
	{
		IIC_stop();
		return;
	}
	
	IIC_SendByteData(data);  //发送要写入内存单元的数据
	if(ACK !=IIC_waitACK())   //等待接收方回应
	{
		IIC_stop();
		return;
	}
	IIC_stop();               //停止IIC
}
```
### 读数据
```c
uint8_t At24c02_ReadByteData(uint8_t addr)
{
	uint8_t data;

	IIC_start();         //起始信号

	IIC_SendByteData(0XA0); //发送设备地址
	if(ACK !=IIC_waitACK()) //等待接收方回应
	{
		IIC_stop();
		return 0xff;
	}

	IIC_SendByteData(addr);//发送要在设备中读出的内存单元地址
	if(ACK !=IIC_waitACK())//等待接收方回应
	{
		IIC_stop();
		return 0xff;
	}

	IIC_start();//起始信号
	IIC_SendByteData(0XA1);  //发送写命令
	if(ACK !=IIC_waitACK())  //等待接收方回应
	{
		IIC_stop();
 		return 0xff;
	}

	data=IIC_RxByteData(noACK); //将接收方返回的数据存起来
	IIC_stop();                  //停止IIC
	return data;                 //将数据返回
}
```