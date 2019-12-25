@[TOC]

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
1. OutputSteam是输出字节流的父类，他是一个抽象类
    ```java
    void write(int b)
    讲解：将指定的字节写入输出流

    void write(byte[] b)throws IOException
    讲解：将b.length个字节的byte数组写入当前输出流

    void flush() throws IOException
    讲解：write是写到缓冲区中，可以认为是内存中,当缓冲区满时系统会自动将缓冲区的内容写入
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

## 字节缓冲输入输出流
1. 什么是缓冲 Buffer 它是内存空间的一部分，在内存空间中预留了一定的存储空间，这些存储空间 用来缓冲输入或输出的数据，这部分空间就叫做缓冲区，缓冲区是具有一定大小的，
2. 为啥要用缓冲:
    - 缓冲，缓和冲击，例如操作磁盘比内存慢的很多，所以不用缓冲区效率很低
    - 数据传输速度和数据处理的速度存在不平衡，比如你每秒要写100次硬盘，对系统冲击很大，浪费了大量时间在忙着处理开始写和结束写这两个事件，用buffer暂存起来，变成每10秒写一次硬盘，数据可以直接送往缓冲区，高速设备不用再等待低速设备，对系统的冲击就很小，写入效率高了。
3. Java IO包里面的两个缓冲类（高级流）:
    - BufferedInputStream 和 BufferedOutputStream
    - 采用包装设计模式（锦上添花）
4. BufferInputStream 缓冲字节输入流:
    - BufferedInputStream 通过预先读入一整段原始输入流数据至缓冲区中，而外界对BufferedInputStream的读取操作实际上是在缓冲区上进行，如果读取的数据超过了缓冲区的范围，那么BufferedInputStream负责重新从原始输入流中载入下一截数据填充缓冲区，然后外界继续通过缓冲区进行数据读取。
    - 好处：避免了大量的磁盘IO，原始的InputStream类实现的read是即时读取的，每一次读取都会是一次磁盘IO操作（哪怕只读取了1个字节的数据），如果数据量巨大，这样的磁盘消耗⾮常可怕。
    - 缓冲区的实现: 读取可以读取缓冲区中的内容，当读取超过缓冲区的内容后再进行一次磁盘IO，载入一段数据填充缓冲，下一次读取一般情况就直接可以从缓冲区读取，减少了磁盘IO。
    - 默认缓冲区大小是8k, int DEFAULT_BUFFER_SIZE = 8192;
    - 常见构造函数:
        ```java
        //对输入流进行包装，里面默认的缓冲区是8k
        public BufferedInputStream(InputStream in);

        //对输入流进行包装,指定创建具有指定缓冲区大小的
        public BufferedInputStream(InputStream in,int size);
        ```
    - 常用的方法
        ```java
        //从输入流中读取一个字节
        public int read();

        //从字节输入流中给定偏移量处开始将各字节读取到指定的 byte 数组中。
        public int read(byte[] buf,int off,int len);

        //关闭释放资源，关闭的时候这个流即可，InputStream会在里面被关闭
        void close();
        ```
5. BufferedOutputStream 缓冲字节输出流:
    - 常见构造器:
        ```java
        //对输出流进行包装,里面默认的缓冲区是8k
        public BufferedOutputStream(OutputStream out);

        //对输出流进行包装,指定创建具有指定缓冲区大小的
        public BufferedOutputStream(OutputStream out,int size);
        ```
    - 常用的3个方法:
        ```java
        //向输出流中输出一个字节
        public void write(int b);

        //将指定 byte 数组中从偏移量 off 开始的 len 个字节写入缓冲的输出流。
        public void write(byte[] buf,int off,int len);

        //刷新此缓冲的输出流，强制使所有缓冲的输出字节被写出到底层输出流中。
        public void flush();

        //关闭释放资源，关闭的时候这个流即可，OutputStream会在里面被关闭, JDK7新特性try(在这里声明的会自动关闭){}
        void close();
        ```
6. Buffer输入输出流实战
    - 文件拷贝:
        ```java
        String dir = "D:\\";
        String name = "b.txt";
        File file = new File(dir,name);

        //字节输入流和字节缓冲输入流
        InputStream is = new FileInputStream(file);//传入参数可以是file 也可以是 文件路径
        BufferedInputStream bis = new BufferedInputStream(is,8192);//size默认是8192byte

        //字节输出流和字节缓冲输出流
        //传入参数可以是file 也可以是文件路径。append为true时，为追加数据 false为覆盖数据
        OutputStream os = new FileOutputStream(dir+"copy.txt",false);
        BufferedOutputStream bos = new BufferedOutputStream(os,8192);//size默认是8192byte

        int length;
        byte[] buf = new byte[1024];

        while ((length = bis.read(buf))!=-1){
            bos.write(buf,0,length);
        }

        //刷新缓冲区，才可以保证数据全部传输完成
        bos.flush();

        //is和os不用关闭，bis和bos关闭的时候会关闭is和os
        bis.close();
        bos.close();
        ```

# 字符流
## Reader
1. Reader是输入字符流的父类，它是一个抽象类, 部分库不推荐使用Reader/Writer，所以简单了解即可
2. 常用方法:
    ```java
    int read()
    讲解：一个字符一个字符的读,只能用来操作文本(不能读图片 音频 视频)
    返回的数据的大小根据字符的大小而变化（最大为四个字节）
    所以返回的数据类型是int需要强转成char才能正常显示

    int read(char cbuf[])
    讲解：从输入字符流中读取一定数量的字符，并将其存储在缓冲区数组cbuf中, 返回实际读取的字符
    数，如果已经到达流末尾而没有可用的字节，则返回－1

    void close() throws IOException
    讲解：关闭输入流并释放与该流关联的系统资源
    ```
3. 常见子类:
    - FileReader 用来读取字符文件的实现类:
        ```java
        public FileReader(String fileName) throws FileNotFoundException {
            super(new FileInputStream(fileName));
        }
        public FileReader(File file) throws FileNotFoundException {
            super(new FileInputStream(file));
        }
        ```
    - 单字符案例:
        ```java
        //读取中文显示出来, java运行时采用utf16编码，多数汉字占2个字节，一个char就够，少数占4个
        //字节，需要两个char来表示
        String dir = "D:\\";
        String name = "b.txt";
        File file = new File(dir,name);

        Reader reader = new FileReader(file);

        int ch;

        while ( (ch = reader.read())!=-1 ){
            System.out.println((char)ch);
        }
        reader.close();
        ```
    - 多字符读取案例:
        ```java
        String dir = "D:\\";
        String name = "b.txt";
        File file = new File(dir,name);

        Reader input = new FileReader(file);

        char[] chars = new char[1024];//一次性读取最大1024个字符

        int len = input.read(chars);

        System.out.println("读取内容为:"+ new String(chars,0,len));

        input.close();
        ```
    - StringReader
## Writer
1. Writer是输出字符流的⽗类，它是一个抽象类:
    ```java
    public void write(int c) throws IOException
    讲解：直接将int型数据作为参数的话，是不会写入数字的，而是现将数字按照ascll码表转换为相应的字符，然后写入

    public void write(String str) throws IOException
    讲解：要想实现数字和中文的写入，必须要用String为参数的Write

    public abstract void write(char cbuf[], int off, int len) throws  IOException
    讲解：将cbuf字符数组的一部分写入到流中，但能不能写len个字符取决于cbuf中是否有那么多

    void flush() throws IOException
    讲解：write是写到缓冲区中，可以认为是内存中,当缓冲区满时系统会⾃动将缓冲区的内容写入文件，
    但是一般还有一部分有可能会留在内存这个缓冲区中, 所以需要调用flush空缓冲区数据。
    对⽐BufferWriter,它需要实时查表，效率低，其实缓冲区IO的各个都有，只不过很⼩被忽略,
    OutputStream都有flush⽅法，看⼦类是否有重写

    void close() throws IOException
    讲解：关闭输入流并释放与该流关联的系统资源
    ```
2. 常见子类:
    - FileWriter 用来写出字符文件的实现类:
        ```java
        public FileWriter(String fileName) throws IOException
        讲解：如果文件不存在，这会⾃动创建。如果文件存在，则会覆盖

        public FileWriter(File file) throws IOException
        讲解：如果文件不存在，这会⾃动创建。如果文件存在，则会覆盖

        public FileWriter(String fileName, boolean append) throws  IOException
        讲解：加入true参数，会实现对文件的续写,使用false则会实现对文件的覆盖

        public FileWriter(File file, boolean append) throws IOException
        讲解：加入true参数，会实现对文件的续写,使用false则会实现对文件的覆盖
        ```
    - 案例:
        ```java
        String dir = "D:\\";
        String name = "b.txt";
        //true为追加数据
        Writer writer = new FileWriter(dir+name,false);
        //直接写入int数值是不会写入数字的，会查表转换成对应字符
        writer.write(23567);
        //写入String 若需要写入数字请用字符串的形式
        writer.write("第二段内容");
        writer.flush();
        writer.close();
        ```
3. StringWriter类
## 字符缓冲输入输出流
1. Buffered Reader字符缓冲输入流
    - 为了提高了单个字符读写的效率，进行字符批量的读写; 为了提高字符流读写的效率，引入了缓冲机制
    - 采用包装设计模式（锦上添花）
    - 简介：当BufferedReader在读取文本文件时，会先尽量从文件中读入字符数据并放满缓冲区，而之后若使用read()⽅法，会先从缓冲区中进行读取。如果缓冲区数据不⾜，才会再从文件中读取
    - 构造函数:
        ```java
        BufferedReader(Reader in)
        BufferedReader(Reader in, int sz)
        //讲解：创建一个使用指定⼤⼩输入缓冲区的缓冲字符输入流。
        ```
    - 常用API:
        ```java
        boolean ready()
        //讲解：判断此流是否已准备好被读取，依赖其他流，所以一般需要做判断

        int read()
        //讲解：读取单个字符

        int read(char[] cbuf, int off, int len) 
        //讲解：读取一部分字符到数组⾥面，从数组下标off处放置length⻓度的字符
        
        String readLine()
        //讲解:读取一整行文本行，返回一整行字符串，如果读到行尾了就返回null,注意返回的一行字符中不包含换行符

        void close() 
        //讲解：关闭流释放资源
        ```
    - 单字符读取案例:
        ```java
        String dir = "D:\\";
        String name = "c.txt";
        Reader reader = new FileReader(dir+name);
        BufferedReader br = new BufferedReader(reader);

        if(br.ready()==false){
            System.out.println("文件流暂时无法读取");
            return;
        }

        int size;

        char[] chars = new char[1024];
        while ((size = br.read(chars))!=-1){
            System.out.println(new String(chars,0,size));
        }
        br.close();
        ```
    - 行读取案例:
        ```java
        String dir = "D:\\";
        String name = "c.txt";
        Reader reader = new FileReader(dir+name);
        BufferedReader br = new BufferedReader(reader);
        if(br.ready()==false){
            System.out.println("文件流暂时无法读取");
            return;
        }
        String line;

        while ((line = br.readLine())!=null){
            System.out.println(line);
        }
        br.close();
        ```
2. BufferedWriter:
    - 简介：写入的数据并不会先输出到目的地，而是先存储至缓冲区中。如果缓冲区中的数据满了，才会一次对目的地进行写出
    - 构造函数:
        ```java
        BufferedWriter(Writer out) 
        BufferedWriter(Writer out, int size)
        ```
    - 常用API:
        ```java
        void write(int c)
        讲解：写入一个字符,会根据码表进行转换

        void write(char[] cbuf, int off, int len)
        讲解：写入字符数组的一部分，通过off和len控制。

        void write(String s, int off, int len) 
        讲解：写入字符数组的一部分，通过off和len控制。

        void newLine() 
        讲解：写如一个换行符合

        void close() 
        讲解：关闭输入流并释放与该流关联的系统资源

        void flush()
        讲解：write是写到缓冲区中，可以认为是内存中,当缓冲区满时系统会⾃动将缓冲区的内容写入文件，但是一般还有一部分有可能会留在内存这个缓冲区中, 所以需要调用flush空缓冲区数据。
        ```
    - 案例:
        ```java
        String dir = "D:\\";
        String name = "c.txt";
        //true为追加数据
        Writer writer = new FileWriter(dir + name,false);

        BufferedWriter bw = new BufferedWriter(writer);

        char ch = '阿';
        bw.write(ch);
        bw.write('发');
        //换行
        bw.newLine();
        bw.write("第二行");
        bw.close();
        ```

# 字符流字节流桥梁
## 背景
1. 计算机存储的单位是字节，从持久设备读取到程序中是解码，从程序写到持久设备中是编码不管是编码还是解码，不同字符集编解码成 字符 需要不同的个数，因此字节流读取容易出错，⽐如乱码。
2. 因此需要一个流，把字节流读取的字节进行缓冲后，在通过字符集解码成字符返回
## InputStreamReader
1. 简介: 
    将字节流转换为字符流, 字节流通向字符流的桥梁,如果不指定字符集编码，则解码过程将使用平台默认的字符编码，如：UTF-8。文本文件存储是A编码，然后如果以B编码进行读取则会乱码。
2. 构造函数:
    ```java
    //使用系统默认编码集
    public InputStreamReader(InputStream in)
    //指定指定编码集创建对象
    public InputStreamReader(InputStream in, String charsetName)
    ```
3. 常用API:
    ```java
    int read()
    讲解：读取单个字符
    int read(char[] cbuf, int off, int len) 
    讲解：读取一部分字符到数组⾥面，从数组下标off处放置length⻓度的字符
    int read(char[] cbuf)
    讲解：将读取到的字符存到数组中，返回读取的字符数
    void close() 
    讲解：关闭流释放资源
    ```
4. 案例:
    ```java
    String dir = "D:\\";
    String name = "c.txt";

    InputStream is = new FileInputStream(dir+name);
    //inputStream 转 Reader
    InputStreamReader isr = new InputStreamReader(is,"UTF-8");
    //缓冲Reader
    BufferedReader br = new BufferedReader(isr);

    String line;
    while ((line=br.readLine())!=null){
        System.out.println(line);
    }
    is.close();
    isr.close();
    br.close();
    ```
## OutputStreamWriter
1. 简介：
    将字符流转换为字节流(看源码解释), 字符流通向字节流的桥梁,如果不指定字符集编码，则编码过程将使用平台默认的字符编码，如：GBK
2. 构造函数：
    ```java
    //使用系统默认编码集
    public OutputStreamWriter(OutputStream out)
    //指定指定编码集创建对象
    public OutputStreamWriter(OutputStream out, String charsetName)
    ```
3. 常用API:
    ```java
    void write(int c)
    讲解：写入一个字符
    void write(char[] cbuf, int off, int len)
    讲解：写入字符数组的一部分，通过off和len控制
    void write(String s, int off, int len) 
    讲解：写入字符数组的一部分，通过off和len控制。
    void newLine() 
    讲解：写如一个换行符合
    void close() 
    讲解：关闭输入流并释放与该流关联的系统资源
    void flush()
    讲解：write是写到缓冲区中，可以认为是内存中,当缓冲区满时系统会⾃动将缓冲区的内容写入文件，但是一般还有一部分有可能会留在内存这个缓冲区中, 所以需要调用flush空缓冲区数据
    ```
4. 案例:
    ```java
    String dir = "D:\\";
    String name = "c.txt";
    //true为追加数据
    OutputStream os = new FileOutputStream(dir+name,false);
    OutputStreamWriter writer = new OutputStreamWriter(os,"UTF-8");
    BufferedWriter bw = new BufferedWriter(writer);

    bw.write('第');
    bw.write('一');
    bw.write('行');
    //换行
    bw.newLine();
    bw.write("第二行");
    bw.flush();
    os.close();
    writer.close();
    bw.close();
    ```
# IO异常处理
## 写法一(老式不推荐)
```java
try {
    FileInputStream fis = new FileInputStream("a.txt");
    BufferedInputStream bis = new BufferedInputStream(fis);
    FileOutputStream fos = new FileOutputStream("copy.txt");
    BufferedOutputStream bos = new BufferedOutputStream(fos);
    int size;
    byte[] buf = new byte[1024];
    while ((size = bis.read(buf)) != -1) {
        //此处抛出异常时流无法正常关闭，容易造成资源浪费。
        bos.write(buf, 0, size);
    }
    //刷新此缓冲的输出流，才可以保证数据全部输出完成,close会⾃动关闭
    bos.flush();
    //关闭的时候只需要关闭最外层的流就行了，源码⾥面会⾃动关闭inputstream对象的
    bis.close();
    bos.close();
} catch (Exception e) {
    e.printStackTrace();
}
```
## 写法二(jdk<=6)
```java
BufferedInputStream bis = null;
BufferedOutputStream bos = null;
try {
    FileInputStream fis = new FileInputStream("a.txt");
    bis = new BufferedInputStream(fis);
    FileOutputStream fos = new FileOutputStream("copy.txt");
    bos = new BufferedOutputStream(fos);
    int size;
    byte[] buf = new byte[1024];
    while ((size = bis.read(buf)) != -1) {
        bos.write(buf, 0, size);
    }
} catch (Exception e) {
    e.printStackTrace();
} finally {
    if (bis != null) {
        try {
            bis.close();
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            if (bos != null) {
                try {
                    bos.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }
}
```
## 写法三(jdk>6)
1. JDK7之后的写法，JDK9⼜进行了改良，但是变化不⼤，记住下面的写法即可
2. 需要关闭的资源只要实现了java.lang.AutoCloseable，就可以⾃动被关闭
3. try()⾥⾯可以定义多个资源，它们的关闭顺序是,最后在try()定义的资源先关闭。
4. 代码:
```java
String dir = "D:\\";
String name = "c.txt";
try(
        FileInputStream is = new FileInputStream(dir+name);
        BufferedInputStream bis = new BufferedInputStream(is);
        FileOutputStream os = new FileOutputStream(dir+"copy.txt");
        BufferedOutputStream bos = new BufferedOutputStream(os);
    ) {
    int size;
    byte[] buf = new byte[1024];
    while ((size = bis.read(buf))!=-1){
        bos.write(buf,0,size);
    }

}catch (Exception e){
    e.printStackTrace();
}
```