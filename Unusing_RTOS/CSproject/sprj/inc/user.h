#ifndef __USER_H
#define __USER_H

#include <sys/stat.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "kernel_list.h"

// 用户信息结构体
struct user
{
    unsigned ID;
    int connfd;
    struct sockaddr_in addr;

    struct list_head list;
};


// 自定义消息协议：
// 群发消息："..."
// 私聊消息："ID::..."

#endif