
<!-- @import "[TOC]" {cmd="toc" depthFrom=1 dekvpthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [文件类型:](#文件类型)
- [流程:](#流程)
  - [1.预处理:](#1预处理)
  - [2.编译](#2编译)
  - [3.汇编](#3汇编)
  - [4.链接](#4链接)

<!-- /code_chunk_output -->


# 文件类型:
| 后缀名 | 类型 |  
| :----: | :---- |
| .c | c源代码 | 
| .h | 程序包含的头文件 | 
| .i | 预处理后的c源代码 |
| .s | 汇编源代码 |
| .o | 编译后文件 |
# 流程:
## 1.预处理:
命令
```bash
gcc -E test.c -o test.i
```
展开头文件。替换宏。删除注释。条件编译。
## 2.编译
命令
```
gcc -S test.i -o test.s
```
-S让编译器在编译之后停止，不进行后续过程
## 3.汇编
```
gcc -c test.s -o test.o
```
将汇编代码转换成二进制机器码
## 4.链接
```
gcc test.o -o test
```