
https://blog.csdn.net/guoping16/article/details/6584058
## 常用shell命令
    ipcs -m  查看
    ipcrm -m shmid号  删除共享内存
## 函数
### ftok
生成key_t(类似UUID)
```c
#include <sys/types.h>
#include <sys/ipc.h>

key_t ftok(const char *pathname, int proj_id);

```
其中的pathname需要为一个文件路径，该路径必须是具体存在的，因为ftok在生成key值时需要使用到文件的inode值，因为inode值在文件系统中具有唯一性。
第二个参数proj_id为偏移值，这个值的末尾8位也同时参与到key的计算中，防止通过inode值进行生成时由于文件相同出现重复。
当函数执行成功，则会返回key_t键值，否则返回-1。
在一般的UNIX中，通常是将文件的索引节点取出，然后在前面加上子序号就得到key_t的值。
### shmget
得到一个共享内存标识符或创建一个共享内存对象
