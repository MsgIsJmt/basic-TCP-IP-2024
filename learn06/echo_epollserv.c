#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

void error_handling(char *message);

int main(int argc, char *argv[])
{
    int sock;  // 소켓 디스크립터
    char message[BUF_SIZE];  // 데이터를 주고받을 버퍼
    int str_len;  // 데이터 길이 변수
    struct sockaddr_in serv_adr;  // 서버 주소 정보 구조체

    if(argc != 3) {  // 명령줄 인수가 부족한 경우 에러 메시지 출력 후 종료
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }
    
    // TCP 소켓 생성
    sock = socket(PF_INET, SOCK_STREAM, 0);   
    if(sock == -1)
        error_handling("socket() error");
    
    // 서버 주소 정보 초기화
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;  // IPv4 주소 체계 사용
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);  // 명령줄 인수로 전달된 서버 IP 주소 설정
    serv_adr.sin_port = htons(atoi(argv[2]));  // 명령줄 인수로 전달된 서버 포트 번호 설정
    
    // 서버에 연결 요청
    if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("connect() error!");
    else
        puts("Connected...........");

    // 사용자 입력과 데이터 송수신 반복
    while(1) 
    {
        fputs("Input message (Q to quit): ", stdout);
        fgets(message, BUF_SIZE, stdin);  // 사용자로부터 메시지 입력 받기
        
        // 사용자가 'q' 또는 'Q'를 입력하면 루프 종료
        if(!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
            break;

        // 소켓을 통해 서버로 메시지 전송
        write(sock, message, strlen(message));
        // 서버로부터 데이터 읽기
        str_len = read(sock, message, BUF_SIZE - 1);
        // 읽은 데이터 끝에 NULL 문자 추가하여 문자열로 만듦
        message[str_len] = 0;
        // 서버가 보낸 메시지 출력
        printf("Message from server: %s", message);
    }
    
    close(sock);  // 소켓 닫기
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);  // 에러 메시지 표준 에러 출력
    fputc('\n', stderr);  // 개행 추가
    exit(1);  // 프로그램 종료
}
