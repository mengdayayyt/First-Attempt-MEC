#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

void handleClient(int clientSocket) {
    char recvBuf[128];

    int randomNumber = rand() % 100;

    while (1) {
        // 接收客户端的请求数据
        int nLen = recv(clientSocket, recvBuf, sizeof(recvBuf), 0);
        if (nLen <= 0) {
            printf("%d Client has disconnected!\n",randomNumber);
            break;
        }

        // 处理请求
        if (strcmp(recvBuf, "over") == 0) {
            printf("%d Client has requested to disconnect.\n",randomNumber);
            break;
        }

        // 向客户端发送相同的数据
        send(clientSocket, recvBuf, nLen, 0);
    }

    // 关闭客户端套接字
    close(clientSocket);
}

int main() {
    // 建立一个socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == -1) {
        perror("Socket error!");
        return 1;
    }

    // 绑定用于接受客户端连接的网络端口
    struct sockaddr_in serverAddr = {};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(4567);    // 端口号 host_to_net
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Bind error!\n");
        return 1;
    }

    // 监听网络端口
    if (listen(serverSocket, 5) == -1) { // 套接字，最大允许连接数量
        perror("Listen error!\n");
        return 1;
    }

    printf("Server is listening on port 4567...\n");

    while (1) {
        // 等待客户端连接
        struct sockaddr_in clientAddr = {};
        socklen_t nAddrLen = sizeof(sockaddr_in);
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &nAddrLen);
        if (clientSocket == -1) {
            perror("Accept error!\n");
            continue;
        }

        printf("New client connected from %s:%d\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

        // 创建子进程处理客户请求
        pid_t childPid = fork();
        if (childPid < 0) {
            perror("Fork error!\n");
        } else if (childPid == 0) { // 子进程
            close(serverSocket); // 子进程不需要监听套接字
            handleClient(clientSocket);
            exit(0); // 处理完客户端请求后退出子进程
        } else { // 父进程
            close(clientSocket); // 父进程不需要客户端套接字，关闭它
            waitpid(childPid, NULL, WNOHANG); // 父进程回收子进程资源
        }
    }

    // 关闭服务器套接字
    close(serverSocket);


    return 0;
}
