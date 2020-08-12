
<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [缓冲区](#缓冲区)
  - [说明](#说明)
  - [分类](#分类)
    - [1.无缓冲](#1无缓冲)
    - [2.行缓冲](#2行缓冲)
    - [3.全缓冲](#3全缓冲)
    - [4.特殊情况](#4特殊情况)
- [Stream流](#stream流)
  - [三个默认打开的流](#三个默认打开的流)
- [fflush()](#fflush)
  - [定义](#定义)
- [标准IO](#标准io)
  - [打开文件](#打开文件)
  - [一次读/写一个字符](#一次读写一个字符)
  - [一次读/写一行字符](#一次读写一行字符)
  - [一次读/写一块数据](#一次读写一块数据)
  - [出错函数](#出错函数)
  - [关闭文件](#关闭文件)
  - [文件指针的定位](#文件指针的定位)
  - [格式化输入输出语句（字符流）](#格式化输入输出语句字符流)
- [文件IO](#文件io)

<!-- /code_chunk_output -->

# 缓冲区
## 说明
缓冲区的分类不是看函数的，是看函数对什么流进行操作。    同一个函数，操作不同的流，就是不同的缓冲区类型。
## 分类
### 1.无缓冲
标准错误输出`perror()`
__flush条件__
```
无条件，一有数据就flush
```
### 2.行缓冲
标准输入流``printf()`` 
标准输出流``scanf()``
__flush条件__
```
\n
程序正常结束
fflush()
```
### 3.全缓冲
文本文件的操作
__flush条件__
```
缓冲区满
程序正常结束
fflush()
```
### 4.特殊情况
特殊情况：
在printf后有scanf会立刻刷新缓冲区;
while(getchar()!='\n');  用于清空输入缓冲区


# Stream流
## 三个默认打开的流
|  名称   | 名称  | 对象 | 文件描述符 |
|  ----  | ----  | ---- | ---- |
|标准输入|stdin|键盘| 0 |
标准输出|stdout|屏幕| 1 |
|标准错误输出|stderr|屏幕| 2 |
# fflush()
## 定义
```c
       #include <stdio.h>

       int fflush(FILE *stream);
```
在任何时刻，可以使用fflush强制刷新一个数据流
# 标准IO
## 打开文件
```c
#include <stdio.h>

FILE *fopen(const char *path, const char *mode);
path : 你要打开的文件路径   "1.txt"  == "./1.txt"
mode : 权限  它是一个字符串  代表接下来你要对文件做什么操作。
```
|  参数   | 读写  | 文件存在 | 文件指针 |
|  ----  | ----  | ---- | ---- |
|r| 只读 | 文件必须存在	| 文件指针指向文件起始位置 |
|r+| 读写 |	文件必须存在| 文件指针指向文件起始位置 |
|w|	只写 | 文件不存在就创建，文件存在就清空 | 文件指针指向文件起始位置 |
|w+|	读写| 文件不存在就创建，文件存在就清空|文件指针指向文件起始位置|
|a|	追加写|	文件不存在就创建，文件存在在文件末尾追加写	|文件指针指向文件末尾位置|
|a+| 读写 |	文件不存在就创建| 文件存在从起始位置读，在末尾位置写|

__返回值：成功返回流代表打开的文件,失败返回``NULL``__
## 一次读/写一个字符
```c
int getchar(void);        //从固定的流：标准输入流 获取一个字符	
int getc(FILE *stream);
int fgetc(FILE *stream);  //从指定的流中读取一个字符。
                          //成功返回读到的字符，失败或者读到文件末尾返回-1
```
```c
int putchar(int c);	 //往固定的流：标准输出流  写一个字符
int putc(int c, FILE *stream);
int fputc(int c, FILE *stream); //往指定流中输入字符c
```
## 一次读/写一行字符
读
```c
char *gets(char *s);  
//从固定流中：标准输入流 获取一行字符
//最后的\n用\0替换

char *fgets(char *s, int size, FILE *stream);   
//从指定流中 获取不大于size个字节的字符
//存到s指定的缓冲区中\n也会获取到，最后加\0,所以更安全
//返回值：成功返回存储数据的缓冲区地址，失败或者读到文件末尾返回NULL
```
```c
/*
gets一般不用，用fgets能实现gets的功能.  fgets(s,size,stdin)
如果遇到\n字符，就读停止
如果一行字符少于size个字节，读到\n结束。 (获取一行)
如果一行字符多于size个字节，读size-1个字节就结束.
*/
```
写
```c
int puts(const char *s);        
//输出s字符串到固定流：标准输出流 会自动在字符串末尾加\n

int fputs(const char *s, FILE *stream); 
//输出s字符串到指定流  
```
## 一次读/写一块数据
```c
#include <stdio.h>

size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);  
//从指定流中读nmemb块数据，每块size个字节大小，存到ptr中

size_t fwrite(void *ptr, size_t size, size_t nmemb,FILE *stream);  
//把ptr中，nmemb块数据，每块size个字节的数据，写到指定流中


返回值：成功返回块数   失败或者读到文件末尾，返回小的块数或0
void * ：可以读写任意类型(结构体)
```
## 出错函数
```c
int feof(FILE *stream);    //测试有无到文件末尾，如果到文件末尾返回非0     
```
```c
int ferror(FILE *stream);   //测试有无错误发生，如果有错误发生返回非0
```
```c
void clearerr(FILE *stream); //清除给定流 stream 的文件结束和错误标识符。
```
应用
```c
while(!feof(fp) && !ferror(fp))
{
  bzero(ch,20);
  ret=fread(ch,1,20,fp);
  fwrite(ch,1,ret,fp2);
}
```
## 关闭文件
```c
#include <stdio.h>

int fclose(FILE *fp);   
//关闭文件流fp：如果流被关闭了，将不能再对这个文件进行读写操作   清空流的缓冲区

返回值：成功返回0，失败返回-1
```
## 文件指针的定位
```c

int fseek(FILE *stream, long offset, int whence);
stream : 你要操作的文件
offset ：偏移量    >0 往后移   < 0 往前移 
whence :  SEEK_SET		文件起始位置
          SEEK_CUR		文件当前位置
          SEEK_END		文件末尾位置

long ftell(FILE *stream);    //返回文件指针的位置。最开始是0

void rewind(FILE *stream);   //将文件指针指向起始位置
```
## 格式化输入输出语句（字符流）
```c
int printf(const char *format, ...);			
//把字符串输出到固定流：stdout     
//把不同的数据类型转为字符流，写到标准输出流
int fprintf(FILE *stream, const char *format, ...);	
//把不同的数据类型，转为字符流，写入到指定流中

int sprintf(char *str, const char *format, ...);        
//字符串的拼接
int snprintf(char *str, size_t size, const char *format, ...);


int scanf(const char *format, ...);			
//从标准输入流获取一个字符串，把字符串转为不同类型的数据进行存放。
int fscanf(FILE *stream, const char *format, ...);
//从字符流中按指定格式读取 例如fscanf(fp,"a=%d b=%f",&a,&b);

int sscanf(const char *str, const char *format, ...);    
//字符串的切割
```
# 文件IO
