#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUF_SIZE 30

void error_handling(char *message);

int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in recv_adr;

    // 실행 시 인자가 3개가 아닌 경우, 사용 방법을 출력하고 종료합니다.
    if(argc != 3) {
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    // TCP 소켓을 생성합니다.
    sock = socket(PF_INET, SOCK_STREAM, 0);

    // sockaddr_in 구조체를 초기화합니다.
    memset(&recv_adr, 0, sizeof(recv_adr));
    recv_adr.sin_family = AF_INET;
    recv_adr.sin_addr.s_addr = inet_addr(argv[1]); // IP 주소를 네트워크 바이트 순서로 변환합니다.
    recv_adr.sin_port = htons(atoi(argv[2])); // 포트 번호를 네트워크 바이트 순서로 변환합니다.

    // 서버에 연결을 시도합니다.
    if(connect(sock, (struct sockaddr*)&recv_adr, sizeof(recv_adr)) == -1)
        error_handling("connect() error!");

    // write와 send를 사용하여 데이터를 송신합니다. send 함수에서는 MSG_OOB 플래그를 사용하여 긴급 데이터 전송을 시도합니다.

    // 1. "123"을 소켓에 기본 데이터로 전송합니다.
    write(sock, "123", strlen("123"));

    // 2. "4"를 소켓에 MSG_OOB 플래그와 함께 전송합니다 (긴급 데이터).
    send(sock, "4", strlen("4"), MSG_OOB);

    // 3. "567"을 소켓에 기본 데이터로 전송합니다.
    write(sock, "567", strlen("567"));

    // 4. "890"을 소켓에 MSG_OOB 플래그와 함께 전송합니다 (긴급 데이터).
    send(sock, "890", strlen("890"), MSG_OOB);

    // 소켓을 닫고 프로그램을 종료합니다.
    close(sock);
    return 0;
}

// 오류 발생 시 메시지를 출력하고 프로그램을 종료하는 함수입니다.
void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
