#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024 // 버퍼 크기를 정의

void error_handling(char *message); // 에러 핸들링 함수의 프로토타입 선언

int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock; // 서버 소켓과 클라이언트 소켓을 위한 변수
    char message[BUF_SIZE]; // 메시지를 저장할 배열
    int str_len, i; // 문자열 길이 및 반복 변수
    
    struct sockaddr_in serv_adr; // 서버 주소 구조체
    struct sockaddr_in clnt_adr; // 클라이언트 주소 구조체
    socklen_t clnt_adr_sz; // 클라이언트 주소 구조체의 크기를 저장할 변수
    
    if(argc!=2) {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }
    
    // 소켓 생성
    serv_sock=socket(PF_INET, SOCK_STREAM, 0);   
    if(serv_sock==-1)
        error_handling("socket() error");
    
    // 서버 주소 구조체 초기화 및 설정
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family=AF_INET; // IPv4 주소 체계 사용
    serv_adr.sin_addr.s_addr=htonl(INADDR_ANY); // 모든 네트워크 인터페이스에 바인딩
    serv_adr.sin_port=htons(atoi(argv[1])); // 인자로 받은 포트 번호를 설정

    // 소켓에 주소 할당
    if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
        error_handling("bind() error");
    
    // 연결 요청 대기 상태로 진입
    if(listen(serv_sock, 5)==-1)
        error_handling("listen() error");
    
    clnt_adr_sz=sizeof(clnt_adr);

    // 최대 5개의 클라이언트와 연결을 수락하고 데이터를 송수신
    for(i=0; i<5; i++)
    {
        // 클라이언트의 연결 요청을 수락하고 클라이언트의 주소 정보를 clnt_adr에 저장
        clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
        if(clnt_sock==-1)
            error_handling("accept() error");
        else
            printf("Connected client %d \n", i+1);
        
        // 클라이언트로부터 데이터를 읽어들이고 다시 클라이언트에게 데이터를 전송
        while((str_len=read(clnt_sock, message, BUF_SIZE))!=0)
            write(clnt_sock, message, str_len);

        // 클라이언트 소켓 닫기
        close(clnt_sock);
    }

    // 서버 소켓 닫기
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
