#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include "../base/log.h"
#include "../base/util.h"

class Acceptor {
public:
    Acceptor(int port)
    {
        m_listen_fd = Socket(AF_INET, SOCK_STREAM);
        
        struct sockaddr_in svr_addr;
        memset(&svr_addr, 0, sizeof(svr_addr));
        svr_addr.sin_family = AF_INET;
        svr_addr.sin_port = htons(port);
        svr_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        
        int ret = bind(m_listen_fd, (SA *)svr_addr, sizeof(svr_addr));
        if (ret < )
        Listen(m_listen_fd, 1024);
    }

private:
    int m_listen_fd;
    int m_port;
};

#endif  // ACCEPTOR_H