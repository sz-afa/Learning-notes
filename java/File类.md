
## 区分斜杠

##### 1、斜杠："/"与反斜杠："\\"
##### 2、反斜杠（\）是⼀个特殊的字符，被称为转义字符，⽤来转义后⾯⼀个字符。转义后的字符通常⽤于表示⼀个不可⻅的字符或具有特殊含义的字符，⽐如\\n则表示换⾏，\\?问号，\\"则表示双引号，\\'表示⼀个单引号等
##### 3、在Java中的字⺟前⾯加上反斜线"\"来表示常⻅的那些不能显示的ASCII字符，我们称之为转义字符
##### 4、例⼦：需要输出双引号的⼀段话
```java
Stringtitle="\"这个是带双引号的标题\"";
```
结果:<br>
\"这个是带双引号的标题\"
##### 5、转义的详情参看
https://baike.baidu.com/item/%E8%BD%AC%E4%B9%89%E5%AD%97%E7%AC%A6/86397?fr=aladdin


## File对象

##### 常⻅的构造函数
```java
String dir="D:\\";
String name="a.txt";
File file=new File(dir,name);

//⽂件的查询和判断
System.out.println("当前系统路径分隔符:"+File.separator);
System.out.println("基本路径getPath()="+file.getPath());
System.out.println("⽂件名getName()="+file.getName());
System.out.println("绝对路径getAbsolutePath="+file.getAbsolutePath());

System.out.println("⽗路径名getParent()="+file.getParent());

System.out.println("是否是绝对路径isAbsolute()="+file.isAbsolute());

System.out.println("是否是⼀个⽬录isDirectory()="+file.isDirectory());

System.out.println("是否是⼀个⽂件isFile()="+file.isFile());

System.out.println("⽂件或⽬录是否存在exists()="+file.exists());

System.out.println("⽬录中的⽂件和⽬录的名称所组成字符串数组list()");

//输出文件夹所有文件
File dirFile=new File(dir);
String[] arr=dirFile.list();
for(String temp : arr){
    System.out.println(temp);
}
//创建指定的⽬录
File mkdirFile=new File(dir+"/testdir");
mkdirFile.mkdir();
//删除指定的⽬录
mkdirFile.delete();

//创建多个层级的⽬录
File mkdirsFile=new File(dir+"/testdirs/test/dd");
mkdirsFile.mkdirs();
//创建⼀个新的⽂件
File newFile=new File(dir+"/testdir/newfile1.txt");
try{
    newFile.createNewFile();
}catch(IOExceptione){
    e.printStackTrace();
}
//删除⽂件
newFile.delete();

//删除文件夹
mkdirFile.delete();
mkdirsFile.delete();
```

