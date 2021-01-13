
<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [udp](#udp)
  - [广播](#广播)
    - [案例](#案例)
      - [发](#发)
      - [收](#收)
  - [组播](#组播)
- [tcp](#tcp)

<!-- /code_chunk_output -->

# udp
## 广播
### 案例
#### 发
```c
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>


int main(void)
{
    int mysocket=socket(AF_INET,SOCK_DGRAM,0); //AF_INET表示ipv4 UDP
    if(mysocket==-1)
    {
        perror("socket");
        exit(1);      //一般成功返回0，失败返回1
    }

    //设置广播的ip端口信息
    struct sockaddr_in dest_addr;
    dest_addr.sin_family = AF_INET; //ipv4
    dest_addr.sin_port=htons(10000);
    dest_addr.sin_addr.s_addr = inet_addr("192.168.50gc.255");

    //设置socket为广播
    int on = 1;
    setsockopt(mysocket,SOL_SOCKET,SO_BROADCAST,&on,sizeof(on));

    char buf[20];
    while (1)
    {
        bzero(buf,20);
        scanf("%s",buf);
        sendto(mysocket,buf,strlen(buf),0,(struct sockaddr *)&dest_addr,sizeof(dest_addr));
    }
    

    return 0;
}
```
#### 收
```c
#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>


int main()
{
    int fd = socket(AF_INET,SOCK_DGRAM,0);
    if(fd < 0)
    {
	perror("socket");
    }

    //设置udp
    int on = 1;
    setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
    struct sockaddr_in recv;
    recv.sin_family = AF_INET;
    recv.sin_port = htons(10000);
    recv.sin_addr.s_addr = inet_addr("192.168.50.255");
    int ret = bind(fd,(struct sockaddr *)&recv,sizeof(recv));
    if(ret < 0)
    {
	perror("bind");
    }

    char buf[30] = {};
    while(1)
    {
	bzero(buf,sizeof(buf));
	ret = recvfrom(fd,buf,sizeof(buf),0,NULL,NULL);
	if(ret < 0)
	{
	    perror("recvfrom");
	}
	printf("recvfrom:%s\n",buf);
    }

    // close(fd);
    return 0;
}

```
## 组播

# tcp
# unix域通信