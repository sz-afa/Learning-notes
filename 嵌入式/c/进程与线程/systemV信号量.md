
<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [简介](#简介)
- [使用](#使用)
  - [创建信号量](#创建信号量)
  - [对system V信号量赋初值](#对system-v信号量赋初值)

<!-- /code_chunk_output -->

# 简介
信号量是一种不同进程或不同线程间的同步方法,有System V信号量和Posix信号量。
# 使用
## 创建信号量
system V信号量使用semget函数进行申请。semget函数在成功申请到信号量之后，会返回一个信号量的标记值，用于之后对信号量的控制和访问。
该函数调用如下：
```c
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int semget(key_t key, int nsems, int semflg);
```
其中key为键值需要具有唯一性，如果这个值相同那么对应的信号量相同。该值一般由ftok函数产生，ftok函数负责产生一个在系统中具有唯一性的值。ftok函数如下：
```c
key_t ftok(const char *pathname, int proj_id);
```
其中的pathname需要为一个文件路径，该路径必须是具体存在的，因为ftok在生成key值时需要使用到文件的inode值，因为inode值在文件系统中具有唯一性。
第二个参数proj_id为偏移值，这个值的末尾8位也同时参与到key的计算中，防止通过inode值进行生成时由于文件相同出现重复。

semget的第二个参数为nsem，即所需申请的system V信号量组中信号量的数目。
第三个参数semflg负责指定这个system V信号量的的读写权限，该权限和system V的作用范围相关，如果希望信号量的适用范围尽可能广，那么可以设置为0666。常用：IPC_CREAT | 0666 IPC_EXCL

例子
```c
a
```

## 对system V信号量赋初值
```c
semctl(data_req_p_id, 0, SETVAL, sem_union);
需要注意的是上面赋初值的使用方式是semctl较为简单的使用方式，semctl本质上有很多调用方式。
```
- data_rep_p_id是信号量的标记值即semget函数的返回值。
- 第二个参数0指明操作信号量组中的第几个信号量，（system V信号量以组的方式进行分配）
- 第三个参数SETVAL是对应的命令，该参数指明semctl函数应该进行的操作为赋值操作
- 第四个参数sem_union是一个联合体，负责传递具体的初值定义如下：
```c
union semun {
    int val;                        /* value for SETVAL */
    struct semid_ds *buf;           /* buffer for IPC_STAT, IPC_SET */
    unsigned short int *array;      /* array for GETALL, SETALL */
    struct seminfo *__buf;          /* buffer for IPC_INFO */
};
```
生成system V信号量之后，需要对system V信号量赋予初值。需要注意的时，对于system V信号量来说，其创建和赋初值是分开的，这将导致一个system V信号量在赋值时可能会出现多个进程同时赋值导致初值不确定的情况，不过这一点可以通过下面的代码进行避免
```c

```
