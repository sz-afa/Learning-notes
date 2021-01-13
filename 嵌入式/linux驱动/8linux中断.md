# 上半部
## 相关函数结构体
### 申请中断号
request_irq
```c
static inline int request_irq(unsigned int irq, irq_handler_t handler, unsigned long flags,const char *name, void *dev)
//参数1 ---- 中断号
//参数2 ---- 中断处理函数指针：irq_handler_t handler
                typedef irqreturn_t (*irq_handler_t)(int, void *);
//参数3 ---- 触发方式:
                #define IRQF_TRIGGER_NONE	0x00000000			//内部中断触发
                #define IRQF_TRIGGER_RISING	0x00000001			//上升沿触发
                #define IRQF_TRIGGER_FALLING	0x00000002	 	//下降沿触发
                #define IRQF_TRIGGER_HIGH	0x00000004			//高电平触发
                #define IRQF_TRIGGER_LOW	0x00000008			//低电平触发
                
//参数4 ---- 字符串，描述信息，自定义
//参数5 ---- 当触发中断时，传给中断处理函数的参数
//返回值----- 成功：0，失败：错误码
```
案例
```c
irqno = IRQ_EINT(1);
ret = request_irq(irqno, key_irq_handler,IRQF_TRIGGER_FALLING|IRQF_TRIGGER_RISING, "key_eint1", NULL);
if(ret < 0){
    printk("request_irq");
    goto err_device;
}
```
### 实现中断处理函数
```c
irqreturn_t xxx_irq_handler(int irqno, void * dev_id)

```
实例
```c
irqreturn_t key_irq_handler(int irqno, void * dev_id)
{
	printk("---------^_^ %s------------\n",__FUNCTION__);
	return IRQ_HANDLED;
}
```
### 注销中断
```c
void free_irq(unsigned int irqno, void * dev);
//参数1 ----- 中断号 
//参数2 ----- 必须和request_irq()的最后一个参数保持一致
```
```c
free_irq(irqno, NULL);
```
# 下半部
## 软中断
## tasklet
## 工作队列