#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30  // 메시지 버퍼 크기 정의

void error_handling(char *message);  // 에러 처리 함수 프로토타입 선언

int main(int argc, char *argv[])
{
    int sock;  // 소켓 파일 디스크립터
    char msg1[] = "Hi!";  // 메시지 1
    char msg2[] = "I'm another UDP host!";  // 메시지 2
    char msg3[] = "Nice to meet you";  // 메시지 3

    struct sockaddr_in your_adr;  // 수신할 상대방 주소 구조체
    socklen_t your_adr_sz;  // 상대방 주소 구조체의 크기를 저장할 변수

    // 명령행 인자의 개수가 올바르지 않을 경우 사용 방법 출력 후 종료
    if (argc != 3) {
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    // UDP 소켓 생성
    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock == -1)
        error_handling("socket() error");

    // 상대방 주소 구조체 초기화
    memset(&your_adr, 0, sizeof(your_adr));
    your_adr.sin_family = AF_INET;  // IPv4 주소 체계
    your_adr.sin_addr.s_addr = inet_addr(argv[1]);  // 명령행 인자로 전달받은 상대방 IP 주소 설정
    your_adr.sin_port = htons(atoi(argv[2]));  // 명령행 인자로 전달받은 상대방 포트 번호 설정

    // 각 메시지를 상대방에게 전송
    sendto(sock, msg1, sizeof(msg1), 0,
           (struct sockaddr*)&your_adr, sizeof(your_adr));
    sendto(sock, msg2, sizeof(msg2), 0,
           (struct sockaddr*)&your_adr, sizeof(your_adr));
    sendto(sock, msg3, sizeof(msg3), 0,
           (struct sockaddr*)&your_adr, sizeof(your_adr));

    close(sock);  // 소켓 닫기
    return 0;
}

// 에러 처리 함수
void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
