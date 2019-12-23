## 字符流字节流区别
- 字符流：处理字符相关，如处理文本数据(TXT文件),Reader/Writer
- 字节流：处理字节相关，如声音图片等二进制数据，InputSteam/OutputStream
- 字节流以字节为单位(8bit)。<br>字符流以字符为单位，根据码表映
射字符，一次可能读取多个字节
- 字节流可以处理所有数据。<br>字符流只能处理字符类型的数据

- 功能不同，但是具有共性内容，通过不断抽象形成4个抽象类，抽象类下⾯有很多⼦类是具体的实现
<br>字符流 Reader/Writer
<br>字节流 InputStream/OutputStream


## InputStream
1. InputStream是输⼊字节流的⽗类，它是⼀个抽象类（⼀般⽤他的⼦类）:

    ```java
    int read() 
    讲解：从输⼊流中读取单个字节,返回0到255范围内的int字节值,字节数据可直接转换为int类型, 如果已经到达流末尾⽽没有可⽤的字节，则返回－1

    int read(byte[] buf)
    讲解：从输⼊流中读取⼀定数量的字节，并将其存储在缓冲区数组buf中,返回实际读取的字节
    数，如果已经到达流末尾⽽没有可⽤的字节，则返回－1

    long skip(long n)
    讲解：从输⼊流中跳过并丢弃 n 个字节的数据。

    int available()
    讲解：返回这个流中有多少个字节数，可以把buf数组⻓度定为这个

    void close() throws IOException
    讲解：关闭输⼊流并释放与该流关联的系统资源
    ```

2. 常⻅⼦类: 
    - FileInputStream:
        常用的构造函数
        ```java
        //传⼊⽂件所在地址
        public FileInputStream(String name) throws FileNotFoundException
        //传⼊⽂件对象
        public FileInputStream(File file) throws FileNotFoundException
        ```

        案例
        ```java
            String dir = "D:\\";
            String name = "a.txt";
            File file = new File(dir,name);
            InputStream is = new FileInputStream(file);
            
            //对于占一个字节的ASCII的字符 可以正常读取，对于汉字等unicode的字符不能正常读取，只能以乱码形式显示
            int read = is.read();
            System.out.println(read);
            System.out.println((char)read);
            is.close();
        ```
    


