
<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [说明](#说明)
- [编译环境](#编译环境)
- [主机操作](#主机操作)
  - [创建相关目录](#创建相关目录)
  - [下载并解压有关的源码](#下载并解压有关的源码)
  - [移植x264](#移植x264)
    - [编译x264](#编译x264)
  - [移植FFMPEG](#移植ffmpeg)
    - [编译ffmpeg](#编译ffmpeg)
  - [导入开发板](#导入开发板)
  - [测试](#测试)

<!-- /code_chunk_output -->

# 说明
ffmpeg 中带有264的解码,没有编码,需要添加x264。libx264是一个自由的H.264编码库，是x264项目的一部分，使用广泛，ffmpeg的H.264实现就是用的libx264。
# 编译环境
|环境|版本|
|:----|:----|
|主机|ubuntu16_64|
|arm开发板|s5pv210|
|x264|20171212|
|ffmpeg|3.4.1|
|arm-gcc|4.5.1|
# 主机操作
## 创建相关目录
```shell
mkdir ffmpeg
cd ffmpeg
mkdir  compressed  install  source
```
## 下载并解压有关的源码
```shell
cd compressed 
wget http://download.videolan.org/pub/videolan/x264/snapshots/x264-snapshot-20171212-2245.tar.bz2
wget http://ffmpeg.org/releases/ffmpeg-3.4.1.tar.bz2
tar -C ../source -xf ffmpeg-3.4.1.tar.bz2
tar -C ../source -xf x264-snapshot-20171212-2245.tar.bz2
```
## 移植x264
### 编译x264
```shell
cd ../source/x264-snapshot-20171212-2245
```
执行脚本mk_x264.sh
```shell
##
#    mk_x264.sh
#    SCHIPS
#    schips@dingtalk.com
#    https://gitee.com/schips/
#    Mon 02 Sep 2019 07:15:41 PM HKT
##
#!/bin/sh
OUTPUT=/home/afa/ffmpeg/install/x264
MYGCC=arm-none-linux-gnueabi

mkdir -p $OUTPUT && rm $OUTPUT/* -rf 

CC=$MYGCC-gcc \
./configure \
--host=$MYGCC \
--enable-shared \
--enable-static \
--enable-pic \
--prefix=$OUTPUT \
--cross-prefix=$MYGCC- \
--disable-asm

make && make install
exit 0
```
## 移植FFMPEG
```shell
cd ../ffmpeg3.4.1
```
### 编译ffmpeg
执行mk_ffmpeg3.4.1.sh
```shell
##
#    mk_ffmpeg3.4.1.sh
#    SCHIPS
#    schips@dingtalk.com
#    https://gitee.com/schips/
#    Mon 02 Sep 2019 08:05:53 PM HKT
##
#!/bin/sh
OUTPUT=/home/afa/ffmpeg/install/ffmpeg
X264_DIR=/home/afa/ffmpeg/install/x264/
MYGCC=arm-none-linux-gnueabi

mkdir $OUTPUT -p && rm $OUTPUT/* -rf

./configure \
--cross-prefix=$MYGCC- \
--enable-cross-compile \
--target-os=linux \
--cc=$MYGCC-gcc \
--arch=arm \
--prefix=$OUTPUT \
--enable-shared \
--enable-static \
--enable-gpl \
--enable-nonfree \
--enable-ffmpeg \
--disable-ffplay \
--enable-ffserver \
--enable-swscale \
--enable-pthreads \
--disable-armv5te \
--disable-armv6 \
--disable-armv6t2 \
--disable-yasm \
--disable-stripping \
--enable-libx264 \
--extra-cflags=-I$X264_DIR/include \
--extra-ldflags=-L$X264_DIR/lib

make clean && make && make install

exit 0
```

## 导入开发板
拷贝 x264中的make install产生的文件到板子根目录
拷贝 ffmpeg的make install产生的文件到板子根目录

## 测试
```shell
ffmpeg -f video4linux2 -s 320x240 -i /dev/video0 /tmp/test.avi 
```
video4linux2 代表使用V4L2协议
/dev/video0代表摄像头设备 
/tmp/test.avi代表输出路径