#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

#define BUF_SIZE 30

void error_handling(char *message);
void urg_handler(int signo);

int acpt_sock;
int recv_sock;

int main(int argc, char *argv[])
{
    struct sockaddr_in recv_adr, serv_adr;
    int str_len, state;
    socklen_t serv_adr_sz;
    struct sigaction act;
    char buf[BUF_SIZE];

    // 프로그램 실행 시 포트 번호를 명시해야 함을 알리는 메시지 출력
    if(argc != 2) {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);    
    }
    
    // SIGURG 시그널 핸들러 설정
    act.sa_handler = urg_handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0; 
    
    // 서버 소켓 생성
    acpt_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&recv_adr, 0, sizeof(recv_adr));
    recv_adr.sin_family = AF_INET;
    recv_adr.sin_addr.s_addr = htonl(INADDR_ANY); // 모든 네트워크 인터페이스에서 접속 허용
    recv_adr.sin_port = htons(atoi(argv[1])); // 명시된 포트 번호로 설정

    // 소켓에 주소(IP, 포트) 바인딩
    if(bind(acpt_sock, (struct sockaddr*)&recv_adr, sizeof(recv_adr)) == -1)
        error_handling("bind() error");
    
    // 연결 요청 대기 상태로 진입
    listen(acpt_sock, 5);

    serv_adr_sz = sizeof(serv_adr);
    
    // 클라이언트의 연결 요청 수락
    recv_sock = accept(acpt_sock, (struct sockaddr*)&serv_adr, &serv_adr_sz);
    
    // 소켓의 소유자를 현재 프로세스로 설정하여 SIGURG 시그널을 받을 수 있도록 함
    fcntl(recv_sock, F_SETOWN, getpid());
    state = sigaction(SIGURG, &act, 0);
    
    // 데이터 수신 및 출력 루프
    while((str_len = recv(recv_sock, buf, sizeof(buf), 0)) != 0) 
    {
        if(str_len == -1)
            continue;
        buf[str_len] = '\0'; // 문자열 끝을 표시하는 널 문자 추가
        puts(buf); // 수신한 데이터를 화면에 출력
    }
    
    // 소켓 연결 종료
    close(recv_sock);
    close(acpt_sock);
    return 0; 
}

// SIGURG 시그널 핸들러 함수
void urg_handler(int signo)
{
    int str_len;
    char buf[BUF_SIZE];
    
    // OOB 데이터 수신
    str_len = recv(recv_sock, buf, sizeof(buf)-1, MSG_OOB);
    buf[str_len] = '\0'; // 문자열 끝을 표시하는 널 문자 추가
    printf("Urgent message: %s \n", buf); // 긴급 데이터를 출력
}

// 오류 발생 시 메시지 출력 후 프로그램 종료 함수
void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
