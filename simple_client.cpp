//yyt

#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
 
#define SOCKET_ERROR -1
 
int main() {
    //1. 建立一个socket
    int _sock = socket(AF_INET, SOCK_STREAM, 0); //与服务器端不同，第三个参数无需声明使用TCP连接
    if (_sock == SOCKET_ERROR) {
        printf("socket build error!\n");
    } else {
        printf("socket build success!\n");
    }
    
    //2. 连接服务器
    sockaddr_in _sin = {};
    _sin.sin_family = AF_INET;
    _sin.sin_port = htons(4567);
    _sin.sin_addr.s_addr = inet_addr("127.0.0.1\n");
    int ret = connect(_sock, (sockaddr*)&_sin, sizeof(sockaddr_in));
    if (ret == SOCKET_ERROR) {
        printf("connect error!\n");
    } else {
        printf("connect success!\n");
    }
    
    //3. 接受服务器信息recv
    char recvBuf[256] = {};
    int nlen = recv(_sock, recvBuf, 256, 0); //返回接受数据的长度
    if (nlen > 0) {
        printf("message: %s\n", recvBuf);
    }
    
    //4. 关闭套接字close socket
    close(_sock);
    printf("close\n");
    
    return 0;
}