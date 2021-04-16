#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "threadpool.h"

void enter(void* arg)
{
	printf("enter tack %p\n",arg);
	int cli_fd = *(int*)arg;
	char buf[4096];
	size_t buf_size = sizeof(buf);
	for(;;)
	{
		int ret = recv(cli_fd,buf,buf_size,0);
		if(0 >= ret || 0 == strcmp("quit",buf))
		{
			close(cli_fd);
			free(arg);
			printf("客户端退出!\n");
			return;
		}

		printf("recv:%s bits:%d\n",buf,ret);
		strcat(buf,":return");

		ret = send(cli_fd,buf,strlen(buf)+1,0);
		if(0 >= ret)
		{
			close(cli_fd);
			free(arg);
			printf("客户端退出!\n");
			return;
		}
	}
}

int main(int argc,const char* argv[])
{
	int svr_fd = socket(AF_INET,SOCK_STREAM,0);
	if(svr_fd < 0)
	{
		perror("socket");
		return EXIT_FAILURE;
	}

	struct sockaddr_in addr = {};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(argv[2]));
	addr.sin_addr.s_addr = inet_addr(argv[1]);
	socklen_t addrlen = sizeof(addr);

	if(bind(svr_fd,(struct sockaddr*)&addr,addrlen))
	{
		perror("bind");
		return EXIT_FAILURE;
	}

	if(listen(svr_fd,10))
	{
		perror("listen");
		return EXIT_FAILURE;
	}

	ThreadPool* thread = create_threadpool(10,20,enter);
	start_threadpool(thread);
	
	for(;;)
	{
		int* cli_fd_p = malloc(sizeof(int));
		*cli_fd_p = accept(svr_fd,(struct sockaddr*)&addr,&addrlen);
		push_threadpool(thread,cli_fd_p);
	}
}
