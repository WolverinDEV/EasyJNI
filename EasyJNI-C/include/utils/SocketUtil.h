//
// Created by wolverindev on 24.08.16.
//

#ifndef CBUNGEE_SOCKETUTIL_H
#define CBUNGEE_SOCKETUTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include "../connection/socket/Socket.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>

#include <netinet/in.h>
#include <netinet/tcp.h>

#include <err.h>
#include <errno.h>
#include <limits.h>
#include <netdb.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#if defined(TCP_CORK) && !defined(TCP_NOPUSH)
    #define TCP_NOPUSH TCP_CORK
#endif


using namespace std;

class SocketUtil {
public:
    static Socket* createTCPSocket(const char *host, uint16_t port) {
        int socket = createTCPFD(host,port);
        debugMessage("Created socket fd "+to_string(socket));
        if(socket > 0)
            return new Socket(socket);
        else
            return (Socket*) new int(socket);
    }

        static in_addr* getIPV4Bytes(const char *host){
            struct hostent *addr = gethostbyname(host);
            for(int i = 0; ; ++i)
            {
                char *temp = addr->h_addr_list[i];
                if(temp == NULL) //we reached the end of the list
                    break; //exit the loop.
                return reinterpret_cast<in_addr*>(temp);
            }
            return nullptr;
        }

        static int createTCPFD(const char *host, uint16_t port){
            int sockfd, n;
            struct sockaddr_in serv_addr;
            struct hostent *server;
            sockfd = socket(AF_INET, SOCK_STREAM, 0);
            if (sockfd < 0) {
                debugMessage("Cant open socket: "+string(strerror(errno)));
                return -1;
            }
            server = gethostbyname(host);
            if (server == NULL) {
                debugMessage("Cant resolve host: "+string(strerror(errno)));
                return -2;
            }
            bzero((char *) &serv_addr, sizeof(serv_addr));
            serv_addr.sin_family = AF_INET;
            bcopy((char *) server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);
            serv_addr.sin_port = htons(port);
            if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
                debugMessage("Error while connecting: "+string(strerror(errno)));
                close(sockfd);
                return -3;
            }
            return sockfd;
        }

    static int createTCPServerSocket(uint16_t port) {
        int sockfd;
        struct sockaddr_in serv_addr;
        int n;
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) {
            return -1;
        }
        bzero((char *) &serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons(port);

        int enable = 0;
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
            logError("setsockopt(SO_REUSEADDR) failed");

        int tcpMaxSegs = 20000;
        int sendBufferSize = 1024*1024*1024;  // 64k
        int reciveBufferSize = 425984;  // 64k
        size_t propsLength = sizeof(int);

       // if(setsockopt(sockfd, IPPROTO_TCP, TCP_MAXSEG, (void*) &tcpMaxSegs, sizeof(tcpMaxSegs)) != 0) logError("Cant tcp max segs!! Error: "+to_string(errno)+" / "+strerror(errno));

        //if(setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, (void*) &sendBufferSize, sizeof(sendBufferSize)) != 0) logError("Cant change send buffer size! Error: "+to_string(errno)+" / "+strerror(errno)); else logMessage("Changed buffer size to: "+to_string(sendBufferSize));
        //if(setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, (void*) &reciveBufferSize, propsLength) != 0) logError("Cant change recive buffer size! Error: "+to_string(errno)+" / "+strerror(errno));
        if(setsockopt(sockfd, IPPROTO_TCP, TCP_NOPUSH, &enable, sizeof enable) != 0) logError("Cant enable nopush! Error: "+to_string(errno)+" / "+strerror(errno));

        if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
            return -2;
        }
        listen(sockfd, 5);

        SocketUtil::displayOpetions(sockfd);
        logMessage("-------------------");
        return sockfd;
    }

        static void displayOpetions(int socketHandle){
            int iSocketOption = 0;
            socklen_t iSocketOptionLen = sizeof(int);;

            struct linger SocketOptionLinger;
            int iSocketOptionLingerLen = sizeof(struct linger);;

            getsockopt(socketHandle, IPPROTO_TCP, TCP_NODELAY, (char *)&iSocketOption, &iSocketOptionLen);
            printf("Socket TCP_NODELAY = %d\n", iSocketOption);

            getsockopt(socketHandle, IPPROTO_TCP, TCP_MAXSEG, (char *)&iSocketOption, &iSocketOptionLen);
            printf("Socket TCP_MAXSEG = %d\n", iSocketOption);

            getsockopt(socketHandle, IPPROTO_TCP, TCP_CORK, (char *)&iSocketOption, &iSocketOptionLen);
            printf("Socket TCP_CORK = %d\n", iSocketOption);

            getsockopt(socketHandle, IPPROTO_TCP, TCP_KEEPIDLE, (char *)&iSocketOption, &iSocketOptionLen);
            printf("Socket TCP_KEEPIDLE = %d\n", iSocketOption);

            getsockopt(socketHandle, IPPROTO_TCP, TCP_KEEPINTVL, (char *)&iSocketOption, &iSocketOptionLen);
            printf("Socket TCP_KEEPINTVL = %d\n", iSocketOption);

            getsockopt(socketHandle, IPPROTO_TCP, TCP_KEEPCNT, (char *)&iSocketOption, &iSocketOptionLen);
            printf("Socket TCP_KEEPCNT = %d\n", iSocketOption);

            getsockopt(socketHandle, IPPROTO_TCP, TCP_SYNCNT, (char *)&iSocketOption, &iSocketOptionLen);
            printf("Socket TCP_SYNCNT = %d\n", iSocketOption);

            getsockopt(socketHandle, IPPROTO_TCP, TCP_LINGER2, (char *)&iSocketOption, &iSocketOptionLen);
            printf("Socket TCP_LINGER2 = %d\n", iSocketOption);

            getsockopt(socketHandle, SOL_SOCKET, SO_REUSEADDR, (char *)&iSocketOption, &iSocketOptionLen);
            printf("Socket SO_REUSEADDR = %d\n", iSocketOption);

            getsockopt(socketHandle, SOL_SOCKET, SO_ERROR, (char *)&iSocketOption, &iSocketOptionLen);
            printf("Socket SO_ERROR = %d\n", iSocketOption);

            getsockopt(socketHandle, SOL_SOCKET, SO_BROADCAST, (char *)&iSocketOption, &iSocketOptionLen);
            printf("Socket SO_BROADCAST = %d\n", iSocketOption);

            getsockopt(socketHandle, SOL_SOCKET, SO_KEEPALIVE, (char *)&iSocketOption, &iSocketOptionLen);
            printf("Socket SO_KEEPALIVE = %d\n", iSocketOption);

            getsockopt(socketHandle, SOL_SOCKET, SO_SNDBUF, (char *)&iSocketOption, &iSocketOptionLen);
            printf("Socket SO_SNDBUF = %d\n", iSocketOption);

            getsockopt(socketHandle, SOL_SOCKET, SO_RCVBUF, (char *)&iSocketOption, &iSocketOptionLen);
            printf("Socket SO_RCVBUF = %d\n", iSocketOption);

            getsockopt(socketHandle, SOL_SOCKET, SO_LINGER, (char *)&SocketOptionLinger, &iSocketOptionLingerLen);
            printf("Socket SO_LINGER = %d  time = %d\n", SocketOptionLinger.l_onoff, SocketOptionLinger.l_linger);

            getsockopt(socketHandle, SOL_SOCKET, SO_RCVLOWAT, (char *)&iSocketOption, &iSocketOptionLen);
            printf("Socket SO_RCVLOWAT = %d\n", iSocketOption);
        }
};

#endif //CBUNGEE_SOCKETUTIL_H
