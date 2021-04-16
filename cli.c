#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc,const char* argv[])
{
	// 创建socket
	int cli_fd = socket(AF_INET,SOCK_STREAM,0);
	if(0 > cli_fd)
	{
		perror("socket");
		return EXIT_FAILURE;
	}

	// 准备地址
	struct sockaddr_in addr = {};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(6789);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	socklen_t addrlen = sizeof(addr);

	// 连接服务器
	if(connect(cli_fd,(struct sockaddr*)&addr,addrlen))
	{
		perror("connect");
		return EXIT_FAILURE;
	}

	char buf[4096] = {};
	size_t buf_size = sizeof(buf);

	for(;;)
	{
		printf(">>>");
		gets(buf);

		// 发送请求
		int ret = write(cli_fd,buf,strlen(buf)+1);
		if(0 >= ret)
		{
			printf("服务器正在升级，请稍候再试!\n");
			break;
		}

		if(0 == strcmp(buf,"quit"))
		{
			printf("结束通信!\n");
			break;
		}

		// 接收响应
		ret = read(cli_fd,buf,buf_size);
		if(0 >= ret)
		{
			printf("服务器正在升级，请稍候再试!\n");
			break;
		}
		printf("read:%s bits:%d\n",buf,ret);
	}
	
	// 关闭
	close(cli_fd);
}
