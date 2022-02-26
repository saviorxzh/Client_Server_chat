/*************************************************************************
	> File Name: 4.client.c
	> Author: xuzheheng
	> Mail: 
	> Created Time: 2022年02月14日 星期一 22时24分06秒
 ************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <strings.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <string.h>

#define SERV_PORT 8000
#define MAXLINE 80

int main() {
    struct sockaddr_in servaddr;
    char buf[MAXLINE] = {"hello tcp"};

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET; // man 7 ip
    servaddr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr); // 回环网络"127.0.0.1",老师ip"119.3.91.173"

    connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    int n;
	while (n = read(0, buf, MAXLINE)) {
        if (!strncmp(buf, "quit", 4)) break; // '\0'的存在
        write(sockfd, buf, n);
        n = read(sockfd, buf, MAXLINE);
        printf("response from server:\n");
        write(1, buf, n);
    }
    close(sockfd);

    return 0;
}
