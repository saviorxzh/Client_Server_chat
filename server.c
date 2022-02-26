/*************************************************************************
	> File Name: 3.server.c
	> Author: xuzheheng
	> Mail: 
	> Created Time: 2022年02月14日 星期一 20时06分07秒
 ************************************************************************/

// 服务器+错误处理+多线程

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <strings.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <pthread.h>

#define SERV_PORT 8000
#define MAXLINE 80

#define prrexit(msg) {\
    perror(msg);\
    exit(1);\
}

void *up_server(void *arg) {
    pthread_detach(pthread_self()); // 自己给自己收尸
    int connfd = (int)arg;
    char buf[MAXLINE];
    int n;
	while (1) {
        n = read(connfd, buf, MAXLINE);
        if (!strncmp(buf, "quit", 4)) break;
        write(1, buf, n);
        for (int i = 0; i < n; i++)
            buf[i] = toupper(buf[i]);
        write(connfd, buf, n);
    }
    close(connfd);
    return (void *)0;
}

int main() {

    struct sockaddr_in serveraddr, cliaddr;
    int listenfd, connfd;
    socklen_t cliaddr_len;

    char str[INET_ADDRSTRLEN]; // 宏等于16(xxx.xxx.xxx.xxx'\0')
    int n;

    listenfd = socket(AF_INET, SOCK_STREAM, 0); // AF_INET=IPv4,SOCK_STREAM=流式
    if (listenfd < 0)
        prrexit("socket");
    
    // 服务器ip地址：端口初始化
    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET; // man 7 ip
    serveraddr.sin_port = htons(SERV_PORT);
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY); // INADDR_ANY=所有IP地址

    if (bind(listenfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
        prrexit("bind");

    if (listen(listenfd, 2) < 0)
        prrexit("listen");

    printf("Accepting connections...\n");
    while (1) {
        cliaddr_len = sizeof(cliaddr);
        connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &cliaddr_len);
        if (connfd < 0) {
            prrexit("accept");
        }
        
        printf("received from %s:%d\n",\
              inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)),
               ntohs(cliaddr.sin_port));

        pthread_t tid;
        pthread_create(&tid, NULL, up_server, (void *)connfd);
        printf("new thread is %#lx\n", tid);

    }

    return 0;
}
