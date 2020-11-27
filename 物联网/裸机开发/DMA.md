# DMA
## 作用
DMA硬件可以实现数据的转移。不需要通过cpu。
## 非中断实现
### 内存到内存demo
drv_dma.c
```c
#include "stm32f10x_dma.h"
#include "stm32f10x_rcc.h"

void DMA_Config(uint32_t *src,uint32_t *drc,uint32_t size)
{
    DMA_InitTypeDef DMA_InitStruct;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);

    DMA_InitStruct.DMA_BufferSize         = size;                           //缓冲区大小
    DMA_InitStruct.DMA_DIR                = DMA_DIR_PeripheralSRC;          //外设是源还是目标 当前设置为外设是源
    DMA_InitStruct.DMA_M2M                = DMA_M2M_Enable;                 //内存到内存
    DMA_InitStruct.DMA_MemoryBaseAddr     = (uint32_t)src;                  //存储器基地址
    DMA_InitStruct.DMA_MemoryDataSize     = DMA_MemoryDataSize_Word;        //数据宽度4字节32位
    DMA_InitStruct.DMA_MemoryInc          = DMA_MemoryInc_Enable;           //增量模式
    DMA_InitStruct.DMA_Mode               = DMA_Mode_Normal;                //模式为 不循环
    DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)drc;                  //外设地址
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;    //外设地址数据宽度4字节32位
    DMA_InitStruct.DMA_PeripheralInc      = DMA_PeripheralInc_Enable;       //增量模式
    DMA_InitStruct.DMA_Priority           = DMA_Priority_Medium;            //优先级
    DMA_Init(DMA1_Channel2, &DMA_InitStruct);
    DMA_ClearFlag(DMA1_FLAG_TC2);
    DMA_Cmd(DMA1_Channel2,ENABLE);
}

uint8_t DMA_IsTrasmitOK(uint32_t *src,uint32_t *drc,uint32_t size)
{
	uint8_t  ret=1;
	uint8_t  i;
	while(SET !=DMA_GetFlagStatus(DMA1_FLAG_TC2));
	DMA_ClearFlag(DMA1_FLAG_TC2);
	for(i=0;i<size;i++)
	{
		if(src[i] !=drc[i])
		{
		ret=0;
		}
	}
	return ret;
}
```
main.c
```c
int main(void)
{

    uint8_t arry1[]="hello";
    uint8_t arry2[]="arry2";

    RCC_DeInit();
    RCC_ConfigTO72();
    SysTicks_Config(72);
    USART_Config();
    DMA_Config((uint32_t *)arry2,(uint32_t *)arry1,sizeof(arry1));
    printf("result:%d\n",DMA_IsTrasmitOK((uint32_t *)arry2,(uint32_t *)arry1,sizeof(arry1)));
    printf("result:%s\n",arry2);
    while(1)
    {

    }
}
```
arry1 的数据复制到了 arry2
## 中断实现