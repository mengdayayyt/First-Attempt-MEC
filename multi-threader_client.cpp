#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>
#include <time.h>

#define NUM_THREADS 5 // 客户端线程数量

// 互斥锁
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// 函数用于处理客户端连接
void *handleServer(void *arg) {
    int randomNumber = rand() % 100;

    printf("%d one request start\n", randomNumber);
    int clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == -1) {
        perror("socket error!\n");
        return NULL;
    }

    // 服务器地址信息
    struct sockaddr_in serverAddr = {};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(4567); // 替换为实际服务器的端口号
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // 替换为实际服务器的IP地址

    // 连接到服务器
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("connect error!\n");
        close(clientSocket);
        return NULL;
    }
    printf("%d connect ok\n", randomNumber);

    // 接收来自服务器的消息
    char recvBuf[256];

    // 使用互斥锁来保护接收操作
    pthread_mutex_lock(&mutex);

    printf("%d receive1\n", randomNumber);
    ssize_t nlen = recv(clientSocket, recvBuf, sizeof(recvBuf), 0);
    printf("%d receive2\n", randomNumber);

    if (nlen > 0) {
        recvBuf[nlen] = '\0';
        printf("%d Received from server: %s\n", randomNumber, recvBuf);
    }

    pthread_mutex_unlock(&mutex);

    // 关闭客户端套接字
    close(clientSocket);
    printf("%d one request over\n", randomNumber);

    return NULL;    
}

int main() {
    pthread_t threads[NUM_THREADS];

    // 创建多个客户端线程
    for (int i = 0; i < NUM_THREADS; i++) {
        printf("new\n");
        // 创建一个线程来处理服务器连接
        if (pthread_create(&threads[i], NULL, handleServer, NULL) != 0) {
            perror("pthread_create error!\n");
            return 1;
        }
    }

    // 等待客户端线程完成
    for (int i = 0; i < NUM_THREADS; i++) {
        printf("&*\n");
        pthread_join(threads[i], NULL);
    }

    printf("all over!\n");

    return 0;
}
