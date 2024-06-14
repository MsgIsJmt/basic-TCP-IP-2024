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
    char message[BUF_SIZE];  // 메시지를 저장할 버퍼
    int str_len;  // 수신한 데이터의 길이
    socklen_t adr_sz;  // 주소 구조체의 크기를 저장할 변수

    struct sockaddr_in serv_adr, from_adr;  // 서버와 수신된 메시지의 주소 구조체

    // 명령행 인자의 개수가 올바르지 않을 경우 사용 방법 출력 후 종료
    if (argc != 3) {
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    // UDP 소켓 생성
    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock == -1)
        error_handling("socket() error");

    // 서버 주소 구조체 초기화
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;  // IPv4 주소 체계
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);  // 명령행 인자로 전달받은 서버 IP 주소 설정
    serv_adr.sin_port = htons(atoi(argv[2]));  // 명령행 인자로 전달받은 서버 포트 번호 설정

    while (1) {
        // 사용자로부터 메시지 입력 받기
        fputs("Insert message(q to quit): ", stdout);
        fgets(message, sizeof(message), stdin);

        // 사용자가 "q" 또는 "Q"를 입력하면 루프 종료
        if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
            break;

        // 서버로 메시지 전송
        sendto(sock, message, strlen(message), 0,
               (struct sockaddr*)&serv_adr, sizeof(serv_adr));

        // 서버로부터 메시지 수신
        adr_sz = sizeof(from_adr);
        str_len = recvfrom(sock, message, BUF_SIZE, 0,
                           (struct sockaddr*)&from_adr, &adr_sz);

        // 수신한 메시지 출력
        message[str_len] = 0;  // 문자열 끝을 나타내는 NULL 문자로 설정
        printf("Message from server: %s", message);
    }
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
