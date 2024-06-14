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
    struct sockaddr_in my_adr, your_adr;  // 서버와 클라이언트 주소 구조체
    socklen_t adr_sz;  // 주소 구조체의 크기를 저장할 변수
    int str_len, i;  // 수신한 데이터의 길이와 반복문 변수

    // 명령행 인자의 개수가 올바르지 않을 경우 사용 방법 출력 후 종료
    if (argc != 2) {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    // UDP 소켓 생성
    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock == -1)
        error_handling("socket() error");

    // 소켓 주소 구조체 초기화
    memset(&my_adr, 0, sizeof(my_adr));
    my_adr.sin_family = AF_INET;  // IPv4 주소 체계
    my_adr.sin_addr.s_addr = htonl(INADDR_ANY);  // 서버 IP 주소는 자동으로 할당
    my_adr.sin_port = htons(atoi(argv[1]));  // 명령행 인자로 전달받은 포트 번호 설정

    // 소켓에 주소 할당
    if (bind(sock, (struct sockaddr*)&my_adr, sizeof(my_adr)) == -1)
        error_handling("bind() error");

    for (i = 0; i < 3; i++) {
        sleep(5);  // 5초 지연
        adr_sz = sizeof(your_adr);
        // 클라이언트로부터 메시지 수신
        str_len = recvfrom(sock, message, BUF_SIZE, 0,
                           (struct sockaddr*)&your_adr, &adr_sz);
        // 수신한 메시지와 번호 출력
        printf("Message %d: %s \n", i + 1, message);
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
