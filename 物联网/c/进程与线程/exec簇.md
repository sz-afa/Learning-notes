
<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [exec函数簇的作用](#exec函数簇的作用)
- [种类](#种类)

<!-- /code_chunk_output -->

## exec函数簇的作用
我们用fork函数创建新进程后，经常会在子进程中调用exec函数去执行另外一个程序。当进程调用exec函数时，该进程被完全替换为新程序。因为调用exec函数并不创建新进程，所以前后进程的ID并没有改变。
## 种类
exec一共有六种函数，都是以exec开头。
```c
#include <unistd.h>
int execl(const char *path, const char *arg, ...);
int execlp(const char *file, const char *arg, ...);
int execle(const char *path, const char *arg,..., char * const envp[]);
int execv(const char *path, char *const argv[]);
int execvp(const char *file, char *const argv[]);
int execvpe(const char *file, char *const argv[],char *const envp[]);
```
参数
```
path 执行的程序名，包含路径

arg.. 可执行程序所带的参数，第一个参数为不带路径的可执行文件名字，且arg必须以NULL结束。

file：如果参数file中包含/，则就将其视为路径名，否则就按 PATH环境变量，在它所指定的各目录中搜寻可执行文件。
```
返回值
```
失败返回-1
```