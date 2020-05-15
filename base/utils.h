#ifndef UTILS_H
#define UTILS_H

#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "log.h"

typedef struct sockaddr SA;

// 将文件描述符设置为非阻塞
void SetNonblocking(int fd)
{
    int flags = fcntl(fd, F_GETFL);
    flags |= O_NONBLOCK;
    if (fcntl(fd, F_SETFL, flags) == -1) {
        LOG_ERROR("SetNonblocking error[%s]\n", strerror(errno));
        exit(-1);
    }
}

int Socket(int domain, int type)
{
    int fd = socket(domain, type, 0);
    if (fd == -1) {
        LOG_ERROR("Socket error[%s]\n", strerror(errno));
        exit(-1);
    }
    return fd;
}

void Bind(int fd, )

#endif  // UTILS_H