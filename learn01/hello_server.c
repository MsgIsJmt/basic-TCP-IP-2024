#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char *message); // 에러 처리 함수 선언

int main(int argc, char *argv[])
{
    int serv_sock; // 서버 소켓 파일 디스크립터
    int clnt_sock; // 클라이언트 소켓 파일 디스크립터

    struct sockaddr_in serv_addr; // 서버 주소 정보를 담는 구조체
    struct sockaddr_in clnt_addr; // 클라이언트 주소 정보를 담는 구조체
    socklen_t clnt_addr_size; // 클라이언트 주소 구조체의 크기

    char message[]="Hello World! I'm MSG"; // 클라이언트에게 전송할 메시지

    // 포트 번호 입력 확인
    if(argc!=2){
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    // 서버 소켓 생성
    serv_sock=socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1)
        error_handling("socket() error");

    // 서버 주소 설정
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family=AF_INET; // IPv4 주소 체계 사용
    serv_addr.sin_addr.s_addr=htonl(INADDR_ANY); // 모든 인터페이스에서 들어오는 연결 수락
    serv_addr.sin_port=htons(atoi(argv[1])); // 인자로 받은 포트 번호 설정

    // 소켓과 서버 주소를 바인딩
    if(bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr))==-1 )
        error_handling("bind() error");

    // 연결 요청 대기열 설정
    if(listen(serv_sock, 5)==-1)
        error_handling("listen() error");

    clnt_addr_size=sizeof(clnt_addr);

    // 클라이언트의 연결 요청 수락
    clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
    if(clnt_sock==-1)
        error_handling("accept() error");

    // 클라이언트에게 메시지 전송
    write(clnt_sock, message, sizeof(message));
    close(clnt_sock); // 클라이언트 소켓 닫기
    close(serv_sock); // 서버 소켓 닫기
    return 0;
}

// 에러 처리 함수
void error_handling(char *message)
{
    fputs(message, stderr); // 표준 오류 스트림에 메시지 출력
    fputc('\n', stderr); // 개행 출력
    exit(1); // 프로그램 종료
}
