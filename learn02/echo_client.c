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
    int sock; // 소켓 디스크립터
    char message[BUF_SIZE]; // 메시지를 저장할 배열
    int str_len; // 문자열 길이
    
    struct sockaddr_in serv_adr; // 서버 주소 정보 구조체
    
    if(argc!=3) { // 프로그램 실행 시 IP 주소와 포트 번호를 인자로 받지 않았을 경우 사용법 안내 후 종료
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }
    
    // 소켓 생성
    sock=socket(PF_INET, SOCK_STREAM, 0);   
    if(sock==-1)
        error_handling("socket() error");
    
    // 서버 주소 정보 구조체 초기화 및 설정
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family=AF_INET; // IPv4 주소 체계 사용
    serv_adr.sin_addr.s_addr=inet_addr(argv[1]); // 인자로 받은 IP 주소를 설정
    serv_adr.sin_port=htons(atoi(argv[2])); // 인자로 받은 포트 번호를 설정
    
    // 서버에 연결
    if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
        error_handling("connect() error!");
    else
        puts("Connected...........");
    
    while(1) 
    {
        // 사용자로부터 메시지 입력을 받음
        fputs("Input message(Q to quit): ", stdout);
        fgets(message, BUF_SIZE, stdin);
        
        // 사용자가 "q"나 "Q"를 입력하면 루프를 종료
        if(!strcmp(message,"q\n") || !strcmp(message,"Q\n"))
            break;

        // 입력한 메시지를 서버로 전송
        write(sock, message, strlen(message));
        
        // 서버로부터 메시지를 읽어들임
        str_len=read(sock, message, BUF_SIZE-1);
        message[str_len]=0;
        // 서버로부터 받은 메시지 출력
        printf("Message from server: %s", message);
    }
    
    // 소켓 닫기
    close(sock);
    return 0;
}

// 에러 핸들링 함수 정의
void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
