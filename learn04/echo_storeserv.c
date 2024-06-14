#include <stdio.h>          // 표준 입력/출력 라이브러리
#include <stdlib.h>         // 표준 라이브러리
#include <string.h>         // 문자열 처리를 위한 라이브러리
#include <unistd.h>         // POSIX 운영 체제 인터페이스
#include <signal.h>         // 시그널 처리를 위한 라이브러리
#include <sys/wait.h>       // 자식 프로세스 상태 확인을 위한 라이브러리
#include <arpa/inet.h>      // 인터넷 주소 변환을 위한 라이브러리
#include <sys/socket.h>     // 소켓 통신을 위한 라이브러리

#define BUF_SIZE 100        // 버퍼 크기 정의

void error_handling(char *message);            // 에러 메시지 출력 함수 선언
void read_childproc(int sig);                  // 자식 프로세스의 종료 신호 처리 함수 선언

int main(int argc, char *argv[]) {
    int serv_sock, clnt_sock;               // 서버 소켓과 클라이언트 소켓
    struct sockaddr_in serv_adr, clnt_adr;  // 서버 및 클라이언트 주소 구조체
    int fds[2];                             // 파일 디스크립터 배열
    pid_t pid;                              // 프로세스 ID
    struct sigaction act;                   // 시그널 액션 구조체
    socklen_t adr_sz;                       // 주소 구조체 크기
    int str_len, state;                     // 문자열 길이 및 상태 변수
    char buf[BUF_SIZE];                     // 버퍼
    
    if(argc != 2) {                         // 포트 번호가 입력되지 않은 경우
        printf("Usage : %s <port>\n", argv[0]);    // 사용법 출력
        exit(1);                            // 프로그램 종료
    }

    act.sa_handler = read_childproc;        // 시그널 핸들러 설정
    sigemptyset(&act.sa_mask);              // 시그널 집합 초기화
    act.sa_flags = 0;                       // 시그널 플래그 초기화
    state = sigaction(SIGCHLD, &act, 0);    // SIGCHLD 시그널에 대한 핸들러 설정

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);  // 서버 소켓 생성
    memset(&serv_adr, 0, sizeof(serv_adr));       // 서버 주소 구조체 초기화
    serv_adr.sin_family = AF_INET;                 // IPv4 주소 체계 사용
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);  // 모든 인터페이스로부터 연결 요청 수락
    serv_adr.sin_port = htons(atoi(argv[1]));      // 포트 번호 설정

    if(bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr)) == -1)  // 소켓에 주소 할당
        error_handling("bind() error");

    if(listen(serv_sock, 5) == -1)                 // 연결 요청 대기열 생성
        error_handling("listen() error");

    pipe(fds);                                      // 파이프 생성
    pid = fork();                                   // 자식 프로세스 생성

    if(pid == 0) {                                  // 자식 프로세스인 경우
        FILE * fp = fopen("echomsg.txt", "wt");     // 파일 오픈
        char msgbuf[BUF_SIZE];                      // 메시지 버퍼
        int i, len;                                 // 루프 변수 및 문자열 길이

        for(i = 0; i < 10; i++) {                  // 10번 반복
            len = read(fds[0], msgbuf, BUF_SIZE);   // 파이프로부터 데이터 읽기
            fwrite((void*)msgbuf, 1, len, fp);      // 파일에 데이터 쓰기
        }
        fclose(fp);                                 // 파일 닫기
        return 0;                                   // 자식 프로세스 종료
    }

    while(1) {                                      // 무한 루프
        adr_sz = sizeof(clnt_adr);                  // 클라이언트 주소 구조체 크기 설정
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);   // 클라이언트 연결 수락
        if(clnt_sock == -1)                         // 클라이언트 소켓 생성 실패 시
            continue;                               // 다음 반복문 수행
        else                                        // 클라이언트 소켓 생성 성공 시
            puts("new client connected...");        // 연결된 클라이언트 알림
        
        pid = fork();                               // 자식 프로세스 생성
        if(pid == 0) {                              // 자식 프로세스인 경우
            close(serv_sock);                       // 서버 소켓 닫기
            while((str_len = read(clnt_sock, buf, BUF_SIZE)) != 0) {   // 클라이언트로부터 메시지 읽기
                write(clnt_sock, buf, str_len);     // 클라이언트로 메시지 전송
                write(fds[1], buf, str_len);        // 파이프를 통해 메시지 전송
            }
            close(clnt_sock);                       // 클라이언트 소켓 닫기
            puts("client disconnected...");         // 클라이언트 연결 종료 알림
            return 0;                               // 자식 프로세스 종료
        } else {                                    // 부모 프로세스인 경우
            close(clnt_sock);                       // 클라이언트 소켓 닫기
        }
    }

    close(serv_sock);                               // 서버 소켓 닫기
    return 0;                                       // 프로그램 종료
}

// 자식 프로세스의 종료 신호 처리 함수
void read_childproc(int sig) {
    pid_t pid;                                      // 프로세스 ID
    int status;                                     // 프로세스 상태

    pid = waitpid(-1, &status, WNOHANG);            // 자식 프로세스의 상태 확인
    printf("removed proc id: %d \n", pid);          // 종료된 자식 프로세스의 ID 출력
}

// 에러 메시지 출력 함수
void error_handling(char *message) {
    fputs(message, stderr);                         // 에러 메시지 출력
    fputc('\n', stderr);                            // 개행 문자 출력
    exit(1);                                        // 프로그램 종료
}
