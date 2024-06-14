#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char *message); // 에러 핸들링 함수 프로토타입 선언

int main(int argc, char *argv[])
{
    int serv_sock; // 서버 소켓 디스크립터
    int clnt_sock; // 클라이언트 소켓 디스크립터

    struct sockaddr_in serv_addr; // 서버 주소 정보 구조체
    struct sockaddr_in clnt_addr; // 클라이언트 주소 정보 구조체
    socklen_t clnt_addr_size; // 클라이언트 주소 정보 구조체의 크기

    char message[] = "Hello World!"; // 전송할 메시지

    // 프로그램 실행 시 포트 번호를 인자로 받지 않았을 경우 사용법 안내 후 종료
    if (argc != 2) {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }
    
    // 소켓 생성
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1)
        error_handling("socket() error");
    
    // 서버 주소 구조체 초기화 및 설정
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET; // IPv4 주소 체계 사용
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); // 모든 네트워크 인터페이스에 바인딩
    serv_addr.sin_port = htons(atoi(argv[1])); // 인자로 받은 포트 번호를 설정

    // 소켓에 주소 할당
    if (bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error");
    
    // 연결 요청 대기 상태로 진입
    if (listen(serv_sock, 5) == -1)
        error_handling("listen() error");
    
    clnt_addr_size = sizeof(clnt_addr);  

    // 클라이언트로부터 연결 요청을 수락하고 클라이언트의 주소 정보를 clnt_addr에 저장
    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
    if (clnt_sock == -1)
        error_handling("accept() error");  
    
    // 클라이언트에게 메시지 전송
    write(clnt_sock, message, sizeof(message));
    
    // 클라이언트 소켓 및 서버 소켓 종료
    close(clnt_sock);   
    close(serv_sock);
    return 0;
}

// 에러 핸들링 함수 정의
void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
