### 图片转base64
```python
import os
import base64
image_path = "C:\\Users\\afa\\Desktop\\1.jpg"

#取出文件格式
temp_list = image_path.split(".")
temp_list.reverse()
image_type =temp_list[0]

if os.path.exists(image_path) :
    #转为二进制格式
    with open(image_path, "rb") as f:  
    #使用base64进行加密
    base64_data = base64.b64encode(f.read())
    base64_str = "data:image/"+image_type+";base64,"
    
    base64_str += str(base64_data,"utf-8")
    
```
-------------------------------
### base64转图片
```python

import os,base64 

img = base64.b64decode(value)
fh = open("pic.jpg","wb")
fh.write(img)
fh.close()
```