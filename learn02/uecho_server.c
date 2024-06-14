#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30  // 버퍼 사이즈 상수 정의

void error_handling(char *message);  // 에러 핸들링 함수 프로토타입 선언

int main(int argc, char *argv[])
{
    int serv_sock;  // 서버 소켓
    char message[BUF_SIZE];  // 메시지를 저장할 버퍼
    int str_len;  // 수신한 데이터의 길이
    socklen_t clnt_adr_sz;  // 클라이언트 주소의 길이를 저장할 변수

    struct sockaddr_in serv_adr, clnt_adr;  // 서버와 클라이언트 주소 구조체

    // 명령행 인자의 개수가 올바르지 않을 경우 사용 방법 출력 후 종료
    if (argc != 2) {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    // UDP 소켓 생성
    serv_sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (serv_sock == -1)
        error_handling("UDP socket creation error");

    // 서버 주소 구조체 초기화
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;  // IPv4 주소 체계
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);  // 서버 IP 주소는 자동으로 할당
    serv_adr.sin_port = htons(atoi(argv[1]));  // 명령행 인자로 전달받은 포트 번호 설정

    // 소켓에 주소 할당
    if (bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("bind() error");

    while (1) {
        clnt_adr_sz = sizeof(clnt_adr);  // 클라이언트 주소의 길이 초기화
        // 클라이언트로부터 메시지 수신
        str_len = recvfrom(serv_sock, message, BUF_SIZE, 0,
                           (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
        // 수신한 메시지를 클라이언트에게 다시 전송
        sendto(serv_sock, message, str_len, 0,
               (struct sockaddr*)&clnt_adr, clnt_adr_sz);
    }
    close(serv_sock);  // 소켓 닫기
    return 0;
}

// 에러 처리 함수
void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
