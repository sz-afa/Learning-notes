
<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [说明](#说明)
- [jni](#jni)
  - [简介](#简介)
  - [步骤](#步骤)
    - [1编写代码](#1编写代码)
    - [2编译代码](#2编译代码)
      - [加载mm等命令](#加载mm等命令)
      - [Android. mk文件](#android-mk文件)
  - [app与jni交互的](#app与jni交互的)
    - [app](#app)
    - [编译动态链接库](#编译动态链接库)
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
#### 加载mm等命令
```shell
source ../build/envsetup.sh
```
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
### app
```java
package com.example.farsightdemo.jniInterface;

public class LedNative {
    //  1加载动态库
    static {    //静态代码优先执行 比构造方法还要优先
        System.loadLibrary("led_jni");  //加载 /system/lib/libled_jni.so文件
    }
    //  2声明本地方法
    public native  int openDev();
    public native  int closeDev();
    public native  int devOn();
    public native  int devOff();
}
```
```java
package com.example.farsightdemo;

import androidx.appcompat.app.AppCompatActivity;

import android.graphics.drawable.LevelListDrawable;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

import com.example.farsightdemo.jniInterface.LedNative;

public class MainActivity extends AppCompatActivity {

    LedNative ledNative;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
        ledNative = new LedNative();
        ledNative.openDev();
    }

    public void openled(View view){
        Toast.makeText(MainActivity.this, "开灯", Toast.LENGTH_SHORT).show();
        ledNative.devOn();
    }

    public void closeled(View view){
        Toast.makeText(MainActivity.this, "关灯", Toast.LENGTH_SHORT).show();
        ledNative.devOff();

    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        ledNative.closeDev();
    }

}
``` 

### 编译动态链接库
`可以参考模板android4.4/development/samples/SimpleJNI`
jni.cpp
```cpp
#include "jni.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <utils/Log.h>


int fd;
char on;

typedef union {
    JNIEnv* env;
    void* venv;
} UnionJNIEnvToVoid;

//返回值是java的int类型所以是jint
jint open_led(JNIEnv *env, jobject thiz){
    fd = open("/dev/led01",O_RDWR);
    if(fd < 0)
    {	
        return -1;
    }
    return 0;
}
jint close_led(JNIEnv *env, jobject thiz){
    close(fd);
    return 0;
}
jint led_on(JNIEnv *env, jobject thiz){
    jint ret;
    on = '1';
    ret = write(fd, &on, sizeof(on));
    if(ret < 0)
    {
        return -1;
    }
    return 0;
}
jint led_off(JNIEnv *env, jobject thiz){
    jint ret;
    on = '0';
    ret = write(fd, &on, sizeof(on));
    if(ret < 0)
    {
        return -1;
    }
    return 0;
}

//构建映射表
static JNINativeMethod led_jni_method[] = {
    {"openDev", "()I",(void*)open_led},
    {"closeDev", "()I",(void*)close_led},
    {"devOn", "()I",(void*)led_on},
    {"devOff", "()I",(void*)led_off},
};

// 1实现JNI_OnLoad 返回值正确返回JNI_VERSION_1_4 错误返回负数
jint JNI_OnLoad(JavaVM* vm, void* reserved)
{

    jint ret;
    // jni环境变量对象
    JNIEnv* env = NULL;
    

    // 参数1 被初始化的env
    // 参数2 jni版本
    // 返回值 正确0 错误负数
    ret = vm->GetEnv((void **)&env, JNI_VERSION_1_4);
    if (ret != JNI_OK) {
        return -1;
    }

    //找到java类对象
    jclass clazz = env->FindClass("com/example/farsightdemo/jniInterface/LedNative");
    if(clazz == NULL)
    {
        return -1;
        
    }

    // 参数1 java方法所在的包类信息
    // 参数2 映射表
    // 参数3 映射表个数
    // 返回值 错误为负数
    ret = env->RegisterNatives(clazz, led_jni_method, 
                            sizeof(led_jni_method)/sizeof(led_jni_method[0]));
    if(ret < 0)
    {
        return -1;
    }
    
    ret = JNI_VERSION_1_4;
    return ret;

}

```
# ndk
## 简介