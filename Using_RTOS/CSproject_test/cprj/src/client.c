

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
#define QUICK_MSG_MAX 10

int sockfd;
int msgNum = 0;
sem_t s;
unsigned id;

void *recvMsg(void *arg)
{
	int n,k,l,dot_flag = 0;
	char *buf = calloc(1, 1024);
	char SORH[10];
	char Decode[20];	
	char *recbuf = calloc(1, 1024);
	
	struct REC_Date
	{
		char	humi_int[10];			//湿度的整数部分
		char	humi_deci[10];			//湿度的整数部分
		char	temp_int[10];	 		//温度的整数部分
		char	temp_deci[10];			//温度的小数部分
		char	bh1750_date[10];		//光照强度
	}RD;


	while(1)
	{
		bzero(buf, 1024);
		bzero(recbuf, 1024);

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
		printf("\r\n\r\n/**********************************/\r\n");
		printf("/*************REC_TEST*************/\r\n");
		printf("/**********************************/\r\n");
		if(buf[0] == 'A' && buf[7] == 'B')
		{
			RD.humi_int[0] = buf[1];
			RD.humi_int[1] = buf[2];
			RD.humi_deci[0] = buf[3];
			RD.temp_int[0] = buf[4];
			RD.temp_int[1] = buf[5];
			RD.temp_deci[0] = buf[6];

			printf("\r\nHumidity:%s.%s %%RH\r\n",RD.humi_int,RD.humi_deci);
//			printf("humi_deci:%s\r\n",RD.humi_deci);
			printf("\r\nTemperature:%s.%s ℃\r\n",RD.temp_int,RD.temp_deci);
//			printf("temp_deci:%s\r\n",RD.temp_deci);
		}

		if(buf[7] == 'B' && buf[strlen(buf)-4] == 'C')
		{
			for(l = 0; l <  10; l++)
			{
				if(buf[l + 9] == 'D') break;
				RD.bh1750_date[l] = buf[l + 8];
			}
			printf("\r\nLight:%s LX\r\n",RD.bh1750_date);
		}
		printf("\r\n收到消息：%s\r\n\r\n",buf);
		fprintf(stderr, "\r（我的ID：%u）", ntohl(id));
	}
//帧头	h_i		h_d		t_i		t_d		DHT结束帧		b_date	光强结束帧	帧尾
// A	xx		x		xx		x			B			xxxx		C		D
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
		printf("你说话太快了，休息一下吧~ \n");
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
