# 字符流字节流区别
- 字符流：处理字符相关，如处理文本数据(TXT文件),Reader/Writer
- 字节流：处理字节相关，如声音图片等二进制数据，InputSteam/OutputStream
- 字节流以字节为单位(8bit)。<br>字符流以字符为单位，根据码表映
射字符，一次可能读取多个字节
- 字节流可以处理所有数据。<br>字符流只能处理字符类型的数据

- 功能不同，但是具有共性内容，通过不断抽象形成4个抽象类，抽象类下面有很多⼦类是具体的实现
<br>字符流 Reader/Writer
<br>字节流 InputStream/OutputStream

# 字节流 
## InputStream
1. InputStream是输入字节流的父类，它是一个抽象类（一般用他的子类）:
    ```java
    int read() 
    讲解：从输入流中读取单个字节,返回0到255范围内的int字节值,字节数据可直接转换为int类型, 
    如果已经到达流末尾而没有可用的字节，则返回－1

    int read(byte[] buf)
    讲解：从输入流中读取一定数量的字节，并将其存储在缓冲区数组buf中,返回实际读取的字节
    数，如果已经到达流末尾而没有可用的字节，则返回－1

    long skip(long n)
    讲解：从输入流中跳过并丢弃 n 个字节的数据。

    int available()
    讲解：返回这个流中有多少个字节数，可以把buf数组长度定为这个

    void close() throws IOException
    讲解：关闭输入流并释放与该流关联的系统资源
    ```

2. 常见⼦类: 
    - FileInputStream:
        常用的构造函数
        ```java
        //传入文件所在地址
        public FileInputStream(String name) throws FileNotFoundException
        //传入文件对象
        public FileInputStream(File file) throws FileNotFoundException
        ```
        单字节读取案例
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
        缓冲读取文件案例(**中文会乱码**)
        ```java
        String dir = "D:\\";
        String name = "a.txt";
        File file = new File(dir,name);
        InputStream is = new FileInputStream(file);
        //字节数组缓冲buf,若buf长度为0则不读取任何字节并返回0;
        //每次读取的字节数最多等于buf长度
        //读取中文容易乱码，因为中文的字节可能会被截断
        byte[] buf = new byte[1024];
        //从流中读取的字节数
        int length;
        while ((length = is.read(buf))!=-1){
            //打印读取结果
            System.out.print(new String(buf,0,length));
        }
        is.close();
        ```
        缓冲读取文本案例(**中文不乱码**)
        ```java
        String dir = "D:\\";
        String name = "a.txt";
        File file = new File(dir,name);
        InputStream is = new FileInputStream(file);
        //字节数组缓冲buf的长度设置为流的大小
        byte[] buf = new byte[is.available()];
        //从流中读取的字节数
        int length;
        while ((length = is.read(buf))!=-1){
            //打印读取结果 不会乱码，注意编码格式一致
            System.out.print(new String(buf,0,length,"UTF-8"));
        }
        is.close();
        ```
        ***编码小知识(节省空间)***
            <br>GB2312只支持简体中文，中文占2个字节，英文占1个字节
            <br>GBK中英文都占2个字节
            <br>UTF-8中文占用3个字节,英文占1个字节
            <br>若文本中文多则推荐GBK、GB2312的方式存储

    - ByteArrayInputStream 字节数组输入流
    - ObjectInputStream 对象输入流



## OutputStream
1. OutputSteam是输出字节流的父类，他说一个抽象类
    ```java
    void write(int b)
    讲解：将指定的字节写入输出流

    void write(byte[] b)throws IOException
    讲解：将b.length个字节的byte数组写入当前输出流

    void flush() throws IOException
    讲解：write是写到缓冲区中，可以认为是内存中,当缓冲区满时系统会⾃动将缓冲区的内容写入
    文件，但是一般还有一部分有可能会留在内存这个缓冲区中, 所以需要调用flush空缓冲区数据。

    void close() throws IOException
    讲解：关闭输入流并释放与该流关联的系统资源
    ```
2. 常见子类: 
    - FileOutputStream:
        构造函数
        ```java
        //传入输出的文件地址
        public FileOutputStream(String name)
        //传入目标输出的文件对象
        public FileOutputStream(File file)
        //传入目标输出的文件对象, 是否可以追加内容
        public FileOutputStream(File file, boolean append)
        ```
        写入文件案例
        ```java
        String dir = "D:\\";
        String name = "b.txt";
        byte[] bytes = "我是内容".getBytes("UTF-8");
        //若文件不存在会自动创建
        OutputStream os = new FileOutputStream(dir+name);
        os.write(bytes);
        os.flush();
        os.close();
        ```
        文件追加数据案例
        ```java
        String dir = "D:\\";
        String name = "b.txt";
        byte[] bytes = "我是内容2".getBytes("UTF-8");
        //写入的内容不覆盖原文件，只在原文件的末尾追加数据
        OutputStream os = new FileOutputStream(dir+name,true);
        os.write(bytes);
        os.flush();
        os.close();
        ```

## IO包之缓冲Buffer输入输出流
1. 什么是缓冲 Buffer 它是内存空间的⼀部分，在内存空间中预留了⼀定的存储空间，这些存储空间 用来缓冲输入或输出的数据，这部分空间就叫做缓冲区，缓冲区是具有⼀定大小的，
2. 为啥要用缓冲:
    - 缓冲，缓和冲击，例如操作磁盘比内存慢的很多，所以不用缓冲区效率很低
    - 数据传输速度和数据处理的速度存在不平衡，⽐如你每秒要写100次硬盘，对系统冲击很⼤，浪费了⼤量时间在忙着处理开始写和结束写这两个事件，用buffer暂存起来，变成每10秒写一次硬盘，数据可以直接送往缓冲区，高速设备不用再等待低速设备，对系统的冲击就很小，写入效率高了。
3. Java IO包⾥⾯的两个缓冲类（高级流）:
    - BufferInputStream 和 BufferOutputStream
    - 采用包装设计模式（锦上添花）
4. BufferInputStream 缓冲字节输⼊流:
    - BufferedInputStream 通过预先读⼊⼀整段原始输⼊流数据⾄缓冲区中，⽽外界对BufferedInputStream的读取操作实际上是在缓冲区上进⾏，如果读取的数据超过了缓冲区的范围，那么BufferedInputStream负责重新从原始输⼊流中载⼊下⼀截数据填充缓冲区，然后外界继续通过缓冲区进⾏数据读取。
    - 好处：避免了⼤量的磁盘IO，原始的InputStream类实现的read是即时读取的，每⼀次读取都会是⼀次磁盘IO操作（哪怕只读取了1个字节的数据），如果数据量巨大，这样的磁盘消耗⾮常可怕。
    - 缓冲区的实现: 读取可以读取缓冲区中的内容，当读取超过缓冲区的内容后再进⾏⼀次磁盘IO，载⼊⼀段数据填充缓冲，下⼀次读取⼀般情况就直接可以从缓冲区读取，减少了磁盘IO。
    - 默认缓冲区⼤⼩是8k, int DEFAULT_BUFFER_SIZE = 8192;
    - 常见构造函数:
        ```java
        //对输⼊流进⾏包装，⾥⾯默认的缓冲区是8k
        public BufferedInputStream(InputStream in);

        //对输⼊流进⾏包装,指定创建具有指定缓冲区⼤⼩的
        public BufferedInputStream(InputStream in,int size);
        ```
    - 常⽤的方法
        ```java
        //从输⼊流中读取⼀个字节
        public int read();

        //从字节输⼊流中给定偏移量处开始将各字节读取到指定的 byte 数组中。
        public int read(byte[] buf,int off,int len);

        //关闭释放资源，关闭的时候这个流即可，InputStream会在⾥⾯被关闭
        void close();
        ```
    - 案例
# 字符流