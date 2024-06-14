#include <stdio.h>          // 표준 입력/출력 라이브러리
#include <stdlib.h>         // 표준 라이브러리
#include <string.h>         // 문자열 처리를 위한 라이브러리
#include <unistd.h>         // POSIX 운영 체제 인터페이스
#include <signal.h>         // 시그널 처리를 위한 라이브러리
#include <sys/wait.h>       // 자식 프로세스의 종료를 대기하기 위한 라이브러리
#include <arpa/inet.h>      // 인터넷 주소 변환을 위한 라이브러리
#include <sys/socket.h>     // 소켓 통신을 위한 라이브러리

#define BUF_SIZE 30         // 버퍼 크기 정의

void error_handling(char *message);        // 에러 메시지 출력 함수 선언
void read_childproc(int sig);              // 자식 프로세스의 종료를 처리하는 함수 선언

int main(int argc, char *argv[]) {
    int serv_sock, clnt_sock;               // 서버 소켓, 클라이언트 소켓
    struct sockaddr_in serv_adr, clnt_adr;  // 서버 주소 구조체, 클라이언트 주소 구조체

    pid_t pid;                              // 프로세스 ID
    struct sigaction act;                   // 시그널 액션 구조체
    socklen_t adr_sz;                       // 주소 구조체 크기 변수
    int str_len, state;                     // 문자열 길이, 상태 변수
    char buf[BUF_SIZE];                     // 버퍼

    if (argc != 2) {                        // 프로그램 실행 시 포트 번호가 입력되지 않은 경우
        printf("Usage : %s <port>\n", argv[0]);    // 사용법 출력
        exit(1);                            // 프로그램 종료
    }

    act.sa_handler = read_childproc;        // 시그널 핸들러 함수 설정
    sigemptyset(&act.sa_mask);              // 시그널 마스크 초기화
    act.sa_flags = 0;                       // 시그널 처리 옵션 초기화
    state = sigaction(SIGCHLD, &act, 0);    // SIGCHLD 시그널에 대한 핸들러 등록

    // 서버 소켓 생성
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0, sizeof(serv_adr));    // 서버 주소 구조체 초기화
    serv_adr.sin_family = AF_INET;              // IPv4 주소 체계 사용
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);  // 서버의 IP 주소 설정 (INADDR_ANY: 자동으로 IP 주소 설정)
    serv_adr.sin_port = htons(atoi(argv[1]));   // 서버의 포트 번호 설정

    // 서버 소켓에 주소 할당
    if (bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("bind() error");
    
    // 클라이언트로부터의 연결 요청 대기
    if (listen(serv_sock, 5) == -1)
        error_handling("listen() error");

    while (1) {
        adr_sz = sizeof(clnt_adr);
        // 클라이언트로부터의 연결 수락
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
        if (clnt_sock == -1)    // 클라이언트 소켓 생성 실패 시
            continue;
        else                    // 클라이언트와 연결된 경우
            puts("new client connected...");

        pid = fork();           // 자식 프로세스 생성
        if (pid == -1) {        // 자식 프로세스 생성 실패 시
            close(clnt_sock);   // 클라이언트 소켓 닫기
            continue;
        }
        if (pid == 0) {         // 자식 프로세스인 경우
            close(serv_sock);   // 부모 프로세스의 서버 소켓 닫기

            // 클라이언트로부터 메시지를 읽어들여서 다시 전송하는 동작 수행
            while ((str_len = read(clnt_sock, buf, BUF_SIZE)) != 0)
                write(clnt_sock, buf, str_len);
            
            close(clnt_sock);   // 클라이언트 소켓 닫기
            puts("client disconnected...");    // 클라이언트 연결 종료 메시지 출력
            return 0;           // 자식 프로세스 종료
        } else {                // 부모 프로세스인 경우
            close(clnt_sock);   // 자식 프로세스와의 통신용 클라이언트 소켓 닫기
        }
    }
    close(serv_sock);           // 서버 소켓 닫기
    return 0;                   // 프로그램 종료
}

// 자식 프로세스의 종료를 처리하는 함수
void read_childproc(int sig) {
    pid_t pid;
    int status;

    // 종료된 자식 프로세스의 PID를 가져와서 상태를 출력
    pid = waitpid(-1, &status, WNOHANG);
    printf("removed proc id: %d \n", pid);
}

// 에러 메시지 출력 함수
void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

