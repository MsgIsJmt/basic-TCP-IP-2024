#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

void error_handling(char *message);

int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in send_adr;

    // 프로그램 실행 시 IP 주소와 포트 번호가 정확히 2개의 인자로 전달되지 않으면 사용법을 출력하고 종료합니다.
    if(argc != 3) {
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    // TCP 소켓 생성
    sock = socket(PF_INET, SOCK_STREAM, 0);

    // sockaddr_in 구조체 초기화
    memset(&send_adr, 0, sizeof(send_adr));
    send_adr.sin_family = AF_INET;
    send_adr.sin_addr.s_addr = inet_addr(argv[1]); // 전달받은 IP 주소를 네트워크 바이트 순서로 변환하여 설정
    send_adr.sin_port = htons(atoi(argv[2])); // 전달받은 포트 번호를 네트워크 바이트 순서로 변환하여 설정

    // 서버에 연결 요청
    if(connect(sock, (struct sockaddr*)&send_adr, sizeof(send_adr)) == -1)
        error_handling("connect() error!");

    // 소켓을 통해 "123"이라는 문자열을 서버로 전송
    write(sock, "123", strlen("123"));

    // 소켓 연결 종료
    close(sock);
    return 0;
}

// 오류 발생 시 메시지를 출력하고 프로그램을 종료하는 함수
void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
