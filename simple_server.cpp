#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
 
int main() {
    //1. 建立一个socket(传入socket族，socket类型, 协议类型)
    int _sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (_sock == -1) {
        perror("scoket error!");
        return 1;
    }
    
    //2. 绑定用于接受客户端连接的网络端口bind
    sockaddr_in _sin = {};
    _sin.sin_family = AF_INET;      //协议族IPV4
    _sin.sin_port = htons(4567);    //端口号 host_to_net
    _sin.sin_addr.s_addr= INADDR_ANY; //不限定访问该服务器的ip
    if (bind(_sock, (sockaddr*)&_sin, sizeof(_sin)) == -1) {
        perror("bind error!\n");
        return 1;
    }
    
    //3. 监听网络端口listen
    if (listen(_sock, 5) == -1) { //套接字，最大允许连接数量
        perror("listen error!\n");
    }
    
    //4. 等待客户端连接accept
    sockaddr_in clientAddr = {};
    socklen_t nAddrLen = sizeof(sockaddr_in);
    int _cSock = -1;
    char msgBuf[] = "Hello, I'm server!\n";

    _cSock = accept(_sock, (sockaddr*)&clientAddr, &nAddrLen); //套接字，收到客户端socket地址，返回socket地址的大小
    if (_cSock == -1) {
        perror("client socket error!");
    }
    printf("new client IP = %s \n", inet_ntoa(clientAddr.sin_addr));

    //5. 向客户端发送数据send
    send(_cSock, msgBuf, sizeof(msgBuf) + 1, 0); //长度+1，将结尾符一并发送过去
    
    
    //6. 关闭套接字close socket
    close(_sock);
    
    printf("close\n");
    
    return 0;
}