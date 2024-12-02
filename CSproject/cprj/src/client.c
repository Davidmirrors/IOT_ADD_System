

#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <signal.h>
#include <string.h>
#include <strings.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <semaphore.h>

// 快速信息最大条数
#define QUICK_MSG_MAX 5

int sockfd;
int msgNum = 0;
sem_t s;
unsigned id;

void *recvMsg(void *arg)
{
	int n,k,l;
	char *buf = calloc(1, 1024);
	char SORH[10];
	char RELAY_FLAG[10];

	while(1)
	{
		bzero(buf, 1024);

		n = read(sockfd, buf, 1024);
		if(n == -1)
		{
			perror("读取网络数据失败");
			break;
		}
		if(n == 0)
		{
			printf("对端已关闭\n");
			break;
		}

		for(k = 0; k < strlen(buf) ; k++)
		{
			if(buf[k] == 'N')
			{
				for(l = 0; l <  5; l++)
				{
					if(buf[k + l + 1] == 'D') break;
					SORH[l] = buf[k + l + 1];
				}
			}
			if(buf[k] == 'D')
			{
				RELAY_FLAG[0] = buf[k + 1];
				RELAY_FLAG[1] = buf[k + 2];
				RELAY_FLAG[2] = buf[k + 3];
				RELAY_FLAG[3] = buf[k + 4];
				RELAY_FLAG[4] = buf[k + 5];
				if(buf[k] == 'E') break;
			}
		}		

		printf("收到：%s\n", buf);
		printf("温度：%c%c.%c  湿度：%c%c.%c\n", buf[4],buf[5],buf[6] , buf[1],buf[2],buf[3]);
		printf("土壤湿度：%s\n", SORH);
		if(RELAY_FLAG[0] == '0') printf("灌溉中\n");
		if(RELAY_FLAG[0] == '1') printf("通风风扇工作中或抽水水泵工作中，或土壤湿度合适，灌溉停止\n");

		if(RELAY_FLAG[2] == '0' && RELAY_FLAG[3] == '0') printf("蓄水池水位低\n");
		if(RELAY_FLAG[2] == '0' && RELAY_FLAG[3] == '1') printf("蓄水池水位充足\n");
		if(RELAY_FLAG[2] == '1' && RELAY_FLAG[3] == '1') printf("蓄水池已满\n");
		if(RELAY_FLAG[2] == '1' && RELAY_FLAG[3] == '0') printf("蓄水池水位传感器故障\n");
		if(RELAY_FLAG[1] == '0') printf("水泵抽水中\n");
		if(RELAY_FLAG[1] == '1') printf("蓄水池水位合适或通风风扇工作中，抽水停止\n");
		
		if(RELAY_FLAG[4] == '0') printf("通风风扇工作中\n");
		if(RELAY_FLAG[4] == '1') printf("通风风扇停止\n");

		fprintf(stderr, "\r（我的ID：%u）", ntohl(id));
	}
}

void *f1(void *arg)
{
	while(1)
	{
		if(0<msgNum && msgNum<=QUICK_MSG_MAX)
			msgNum--;

		sleep(1);
	}
}

void *f2(void *arg)
{
	while(1)
	{
		sem_wait(&s);
		printf("你说话太快了，喝杯茶休息一下吧~ \n");
		fprintf(stderr, "\r（我的ID：%u）", ntohl(id));
	}
}

void recvOOB(int sig)
{
    char oob;
    recv(sockfd, &oob, 1, MSG_OOB); //接收对端的OOB带外数据
	printf("指定的ID不存在，或对方已下线。\n");
	fprintf(stderr, "\r（我的ID：%u）", ntohl(id));
}

int main(int argc, char **argv)
{
	if(argc != 2)
	{
		printf("请指定服务端IP\n");
		exit(0);
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	// 准备地址结构体
	struct sockaddr_in addr;
	bzero(&addr, sizeof(addr));

	// 填入正确的服务器的IP+PORT
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(argv[1]);
	addr.sin_port = htons(50021);

	// 对服务器发起连接请求...
	if(connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == 0)
	{
		printf("连接成功！\n");
		read(sockfd, &id, sizeof(id));
		fprintf(stderr, "\r（我的ID：%u）", ntohl(id));
	}
	else
	{
		perror("connect()失败");
		exit(0);
	}


	// 若私聊对端不存在，服务器将发送一个OOB数据过来提醒
	signal(SIGURG, recvOOB);
	fcntl(sockfd, F_SETOWN, getpid());


	// 接收其他客户端的消息
	pthread_t tid;
	pthread_create(&tid, NULL, recvMsg, NULL);


	// 防刷屏
	sem_init(&s, 0, 0);
	pthread_create(&tid, NULL, f1, NULL);
	pthread_create(&tid, NULL, f2, NULL);

	// 向服务器发送吐槽信息
	char *buf = calloc(1, 1024);
	while(1)
	{
		bzero(buf, 1024);
		fgets(buf, 1024, stdin);

		// 当发送消息过快时，触发警告
		msgNum++;
		if(msgNum >= QUICK_MSG_MAX)
		{
			msgNum = QUICK_MSG_MAX;
			sem_post(&s);
		}
		else
			write(sockfd, buf, strlen(buf));

		fprintf(stderr, "\r（我的ID：%u）", ntohl(id));
	}

	return 0;
}
