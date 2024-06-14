#include <stdio.h>      // 표준 입력/출력 라이브러리
#include <stdlib.h>     // 표준 라이브러리
#include <string.h>     // 문자열 처리를 위한 라이브러리
#include <unistd.h>     // POSIX 운영 체제 인터페이스
#include <arpa/inet.h>  // 인터넷 주소 변환을 위한 라이브러리
#include <sys/socket.h> // 소켓 통신을 위한 라이브러리

#define BUF_SIZE 30     // 버퍼 크기 정의

void error_handling(char *message);            // 에러 메시지 출력 함수 선언
void read_routine(int sock, char *buf);        // 읽기 루틴 함수 선언
void write_routine(int sock, char *buf);       // 쓰기 루틴 함수 선언

int main(int argc, char *argv[]) {
    int sock;                                   // 소켓
    pid_t pid;                                  // 프로세스 ID
    char buf[BUF_SIZE];                         // 버퍼
    struct sockaddr_in serv_adr;                // 서버 주소 구조체

    if (argc != 3) {                            // 프로그램 실행 시 IP 주소와 포트 번호가 입력되지 않은 경우
        printf("Usage : %s <IP> <port>\n", argv[0]);    // 사용법 출력
        exit(1);                                // 프로그램 종료
    }
    
    // 소켓 생성
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1)                             // 소켓 생성 실패 시
        error_handling("socket() error");

    memset(&serv_adr, 0, sizeof(serv_adr));     // 서버 주소 구조체 초기화
    serv_adr.sin_family = AF_INET;              // IPv4 주소 체계 사용
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);   // 서버의 IP 주소 설정
    serv_adr.sin_port = htons(atoi(argv[2]));   // 서버의 포트 번호 설정

    // 서버로 연결 요청
    if (connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("connect() error");

    // 자식 프로세스 생성하여 쓰기 루틴 수행
    pid = fork();
    if (pid == -1) {                            // 자식 프로세스 생성 실패 시
        close(sock);                            // 소켓 닫기
        error_handling("fork() error");
    }
    if (pid == 0)                               // 자식 프로세스인 경우
        write_routine(sock, buf);
    else                                        // 부모 프로세스인 경우
        read_routine(sock, buf);

    close(sock);                                // 소켓 닫기
    return 0;                                   // 프로그램 종료
}

// 읽기 루틴 함수
void read_routine(int sock, char *buf) {
    while (1) {
        int str_len = read(sock, buf, BUF_SIZE);    // 서버로부터 메시지 읽기
        if (str_len == 0)                           // 서버로부터 받은 메시지가 없는 경우
            return;                                 // 함수 종료

        buf[str_len] = 0;                           // 버퍼의 끝에 널 문자 추가
        printf("Message from server: %s", buf);     // 서버로부터 받은 메시지 출력
    }
}

// 쓰기 루틴 함수
void write_routine(int sock, char *buf) {
    while (1) {
        fgets(buf, BUF_SIZE, stdin);               // 키보드 입력을 통해 메시지 읽기
        if (!strcmp(buf, "q\n") || !strcmp(buf, "Q\n")) {   // 입력이 'q'인 경우
            shutdown(sock, SHUT_WR);                // 소켓의 쓰기 스트림 종료
            return;                                 // 함수 종료
        }
        write(sock, buf, strlen(buf));              // 서버로 메시지 전송
    }
}

// 에러 메시지 출력 함수
void error_handling(char *message) {
    fputs(message, stderr);         // 에러 메시지 출력
    fputc('\n', stderr);            // 개행 문자 출력
    exit(1);                        // 프로그램 종료
}
