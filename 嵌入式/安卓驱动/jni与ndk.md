
<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [说明](#说明)
- [jni](#jni)
  - [简介](#简介)
  - [步骤](#步骤)
    - [1编写代码](#1编写代码)
    - [2编译代码](#2编译代码)
      - [Android. mk文件](#android-mk文件)
  - [app与jni交互的](#app与jni交互的)
- [ndk](#ndk)
  - [简介](#简介-1)

<!-- /code_chunk_output -->

# 说明
jni与ndk在安卓中的应用。
# jni
## 简介
jni是jvm自带的。用来与c/c++程序相互调用。以及相互传参。
在安卓中。驱动代码都是c/c++。app是java开发的。app的java代码利用jni去调用驱动的c/c++代码来控制硬件
## 步骤
### 1编写代码
编写代码的环境必须要在Android源代码目录下操作。
因为需要依赖安卓源码的库文件，Android源代码必须要编译。
在安卓源码下新建文件夹（名字自定义）
### 2编译代码
#### Android. mk文件
安卓的makefile
模板
```
vim hardware/ril/rild/Android.mk
```
可执行程序模板
```Makefile
#获取当前文件夹
LOCAL_PATH:= $(call my-dir)
#清空除了LOCAL_PATH外的 LOCAL_XXX变量
include $(CLEAR_VARS)
# 源文件
LOCAL_SRC_FILES:= \
        rild.c
#目标文件
LOCAL_MODULE:= rild
#在任何情况下都编译当前目录
LOCAL_MODULE_TAGS := optional
#编译成可执行程序
include $(BUILD_EXECUTABLE)
```
动态库模板
```Makefile
#获取当前路径
LOCAL_PATH:= $(call my-dir)
#清空所有以LOCAL_开头的变量的值
include $(CLEAR_VARS)
#在任何情况下都编译当前目录
LOCAL_MODULE_TAGS := optional
#目标
LOCAL_MODULE:= libled_jni
#源文件
LOCAL_SRC_FILES:= led_jni.cpp
#依赖的动态库
LOCAL_SHARED_LIBRARIES := libutils
#指定包含jni.h
LOCAL_C_INCLUDES += $(JNI_H_INCLUDE)
#编译成动态库
include $(BUILD_SHARED_LIBRARY)
```
## app与jni交互的

# ndk
## 简介