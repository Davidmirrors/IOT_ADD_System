

#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
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

#include "kernel_list.h"
#include "user.h"

#define USERDATA_MAXLEN 1024

int totalUser = 0;
struct user *clients;

void init_list()
{
    clients = calloc(1, sizeof(struct user));
    if(clients != NULL)
        INIT_LIST_HEAD(&clients->list);
}

unsigned randomID()
{
    srand(time(NULL));
    return rand()%100000;
}


// 群发消息
void broadcastMsg(int senderID, const char *msg)
{
    struct user *p;

    list_for_each_entry(p, &clients->list, list)
    {
        // 跳过发送者本身
        if(p->ID == senderID)
            continue;

        write(p->connfd, msg, strlen(msg));
    }
}

// 客户端处理线程
void *clientRoutine(void *arg)
{
    pthread_detach(pthread_self());

    // 将用户ID回送给客户端
    struct user *client = (struct user *)arg;
    unsigned id = htonl(client->ID);
    write(client->connfd, &id, sizeof(id));

    // 处理客户端数据
    char *buf = calloc(1, USERDATA_MAXLEN);
    int n;
    while(1)
    {
        bzero(buf, USERDATA_MAXLEN);
        n = read(client->connfd, buf, USERDATA_MAXLEN);

        if(n == -1)
        {
            perror("读取客户端消息失败");
            break;
        }

        if(n == 0)
        {
            printf("\r[%s:%hu]已下线\n", inet_ntoa(client->addr.sin_addr), ntohs(client->addr.sin_port));
            break;
        }

        broadcastMsg(client->ID, buf);

        printf("\r[%s:%hu]: %s\n", inet_ntoa(client->addr.sin_addr), ntohs(client->addr.sin_port), buf);
        fprintf(stderr, "\r（当前在线用户数：%d）", totalUser);

    }

    close(client->connfd);
    list_del(&client->list);
    free(client);
	fprintf(stderr, "\r（当前在线用户数：%d）", --totalUser);
}

int main(int argc, char **argv)
{
	// 创建TCP套接字
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);

	// 设置地址重复使用
	int on = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

	// 准备本机（服务器）地址结构体
	struct sockaddr_in addr;
	socklen_t len = sizeof(addr);
	bzero(&addr, len);

	// 绑定套接字与网络地址（IP+PORT）
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY); // 自动获取本机IP
	addr.sin_port = htons(50021); // 端口号PORT
	if(bind(sockfd, (struct sockaddr *)&addr, len) != 0)
	{
		perror("绑定端口失败"); 
		exit(0);
	}
    
	// 将sockfd设置为被动监听套接字 s
	listen(sockfd, 3);

    // 初始化用户链表 
    init_list(); 
    if(clients == NULL)
    {
        perror("初始化用户链表失败");
        exit(0);
    }


    // 主循环：每当来一个远程客户端，就分配一条专门的线程处理之
    while(1)
    {
        // 准备存储对端（客户端）的用户结构体
        struct user *newClient = calloc(1, sizeof(struct user));
        socklen_t len = sizeof(newClient->addr);

        if((newClient->connfd=accept(sockfd, (struct sockaddr *)&newClient->addr, &len)) < 0)
        {
            perror("accept失败");
            free(newClient);
            continue;
        }

        printf("\r[%s:%hu]连接成功\n", inet_ntoa(newClient->addr.sin_addr), ntohs(newClient->addr.sin_port));
        newClient->ID = randomID();

        // 将新客户端信息加入链表，并启动专用线程处理之
        list_add_tail(&newClient->list, &clients->list);

		fprintf(stderr, "（当前在线用户数：%d）", ++totalUser);

        pthread_t tid;
        pthread_create(&tid, NULL, clientRoutine, (void *)newClient);
    }

	return 0;
}
