
# 实现代码
## seq.h
```c
#ifndef _MYHEAD_H
#define _MYHEAD_H


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Seq
{
	int *data;   //动态数组   保存顺序表首地址
	int length;  //保存顺序表数据个数
	int size;    //保存顺序表总大小
}seq;

extern seq *seq_init(void);
extern void display(seq *p);
extern bool seq_insert(seq *p,int elem,int add);
extern void seq_del(seq *p,int add);
extern int seq_search(seq *p,int elem);
extern void seq_update(seq *p,int elem,int newelem);

#endif
```

## seq.c
```c
#include "myhead.h"

//1 顺序表初始化
seq *seq_init(void)
{
	seq *p=(seq *)malloc(sizeof(seq));
	p->data=(int *)malloc(sizeof(int)*10);
	p->length=0;
	p->size=10;
	
	return p;
}

//3 遍历顺序表
void display(seq *p)
{
	int i;
	for(i=0;i<p->length;i++)
	{
		printf("%d  ",p->data[i]);
	}
	
	printf("\n");
}

//4 添加某个数据到某个位置
bool seq_insert(seq *p,int elem,int add)   
// 1 3 5 7    elem =10    add=2  length=4
{
	if(add<1 || add>p->length+1)
	{
		return false;
	}
	
	if(p->size == p->length)
	{
		p->data=realloc(p->data,sizeof(int)*(p->size+1));
		p->size++;
	}
	
	int i;
	for(i=p->length;i>add-1;i--)
	{
		p->data[i]=p->data[i-1];
	}
	p->data[add-1]=elem;
	p->length++;
	
	return true;
}


//5 删除某个位置的元素
void seq_del(seq *p,int add)    // 1 3 5 7 9     add =2   1 5 7 9
{
	if(add<1 || add>p->length)
	{
		return;
	}
	
	int i;
	for(i=add;i<p->length;i++)
	{
		p->data[i-1]=p->data[i];
	}
	p->length--;
}

//6 查询某个数据所在的位置
int seq_search(seq *p,int elem)   //   1 3 5 7 9   
{
	int i;
	for(i=0;i<p->length;i++)
	{
		if(p->data[i]==elem)
		{
			return i+1;
		}
	}
	
	return -1;
}

//7 修改某个数据为新的数据
void seq_update(seq *p,int elem,int newelem)
{
	int add=seq_search(p, elem);
	if(add==-1)
	{
		printf("你要修改的数据不存在！\n");
		return;
	}
	
	p->data[add-1]=newelem;
	
}
```