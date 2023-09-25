#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

int main() {
    // 创建一个socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == -1) {
        perror("Socket error!");
        return 1;
    }

    // 服务器地址信息
    struct sockaddr_in serverAddr = {};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(4567);  // 服务器端口
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // 服务器IP地址

    // 连接到服务器
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Connect error!\n");
        return 1;
    }

    printf("Connected to the server.\n");

    while (1) {
        char sendBuf[128];
        printf("Enter a message (or 'over' to disconnect): ");
        fgets(sendBuf, sizeof(sendBuf), stdin);

        // 发送消息到服务器
        send(clientSocket, sendBuf, strlen(sendBuf), 0);

        // 如果用户输入'over'，退出循环
        if (strcmp(sendBuf, "over\n") == 0) {
            break;
        }

        char recvBuf[128];
        // 接收服务器的响应
        int nLen = recv(clientSocket, recvBuf, sizeof(recvBuf), 0);
        if (nLen > 0) {
            recvBuf[nLen] = '\0';
            printf("Received from server: %s\n", recvBuf);
        }
    }

    // 关闭客户端套接字
    close(clientSocket);
    printf("close connection!\n");

    return 0;
}
