
<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [无名管道](#无名管道)
  - [描述](#描述)
  - [函数](#函数)
  - [例子](#例子)
- [有名管道](#有名管道)
  - [描述](#描述-1)
  - [函数](#函数-1)
    - [1创建管道文件](#1创建管道文件)
    - [2判断文件是否存在](#2判断文件是否存在)
    - [3读写](#3读写)
    - [4有名管道阻塞](#4有名管道阻塞)

<!-- /code_chunk_output -->

# 无名管道
## 描述
用于父子兄弟有血缘关系进程间的通信
## 函数
```c
#include <unistd.h>
int pipe(int pipefd[2]);

```
```
pipefd: 数组，里面有2个元素的数组，
        pipefd[0]: 读端 文件描述符
        pipefd[1]: 写端 文件描述符
返回值：成功返回0  ，失败返回-1
```
## 例子
```c
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    int pipefd[2];
    if(pipe(pipefd)){
        perror("pipe error");
        exit(-1);
    }

    pid_t pt = fork();

    if(pt<0){
        perror("fork error");
        exit(-1);
    }else if(pt==0){
        //关闭写管道
        close(pipefd[1]);
        char buf[50]={0};
        //从读管道读数据
        read(pipefd[0],buf,sizeof(buf));
        printf("buf:%s\n",buf);
    }else{
        //关闭读管道
        close(pipefd[0]);
        char buf[50]={0};
        char buf1[50]={0};
        scanf("%s",buf);
        //往写管道里写数据
        write(pipefd[1],buf,strlen(buf));       
        read(pipefd[0],buf1,sizeof(buf1));

        //空的，因为读管道已经关闭了。
        printf("buf1:%s\n",buf1);
    }

    return 0;
}

```
# 有名管道
## 描述
用于进程间通信，有个管道文件
有名就是通过对这个管道文件进行读写从而通信
## 函数
### 1创建管道文件
```c
#include <sys/types.h>
#include <sys/stat.h>

int mkfifo(const char *pathname, mode_t mode);
        pathname: 管道名字
        mode： 权限 0666
        返回值： 成功返回0 失败返回-1
```
### 2判断文件是否存在
```c
#include <unistd.h>

int access(const char *pathname, int mode);
        pathname: 文件名字
        mode：	
            F_OK  
        返回值：成功返回0 失败返回-1
```
### 3读写
用文件IO进行读写read,write
### 4有名管道阻塞
当只有读端或者只有写端时会阻塞，只有读端和写端都存在程序才能继续执行