#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

// 函数用于处理客户端连接
void *handleClient(void *arg) {
    int clientSocket = *((int *)arg);
    char msgBuf[] = "Hello, I'm server!";
    
    // 向客户端发送数据
    send(clientSocket, msgBuf, sizeof(msgBuf), 0);
    
    // 关闭客户端套接字
    close(clientSocket);
    
    return NULL;
}

int main() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == -1) {
        perror("socket error!");
        return 1;
    }
    
    struct sockaddr_in serverAddr = {};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(4567);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("bind error!");
        return 1;
    }
    
    if (listen(serverSocket, 5) == -1) {
        perror("listen error!");
        return 1;
    }
    
    struct sockaddr_in clientAddr = {};
    socklen_t clientAddrLen = sizeof(clientAddr);
    
    while (1) {
        int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen);
        if (clientSocket == -1) {
            perror("accept error!");
        } else {
            printf("New client connected from %s\n", inet_ntoa(clientAddr.sin_addr));
            
            // 创建一个线程来处理客户端连接
            pthread_t tid;
            if (pthread_create(&tid, NULL, handleClient, &clientSocket) != 0) {
                perror("pthread_create error!");
            }
            
            // 分离线程，线程结束后自动释放资源
            pthread_detach(tid);
        }
    }
    
    close(serverSocket);
    
    return 0;
}
