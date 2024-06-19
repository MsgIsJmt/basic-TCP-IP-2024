#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 1024

int main(int argc, char *argv[])
{
    int sock;  // 소켓 디스크립터
    char buf[BUF_SIZE];  // 데이터를 주고받을 버퍼
    struct sockaddr_in serv_addr;  // 서버 주소 정보 구조체

    FILE *readfp;  // 소켓으로부터 읽기 위한 파일 포인터
    FILE *writefp;  // 소켓으로 쓰기 위한 파일 포인터
    
    // 소켓 생성
    sock = socket(PF_INET, SOCK_STREAM, 0);
    // serv_addr 구조체 초기화
    memset(&serv_addr, 0, sizeof(serv_addr));
    // IPv4 주소 체계 사용
    serv_addr.sin_family = AF_INET;
    // IP 주소 설정
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    // 포트 번호 설정
    serv_addr.sin_port = htons(atoi(argv[2]));
  
    // 서버에 연결 요청
    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    // 소켓을 통해 읽기 위한 파일 포인터 생성
    readfp = fdopen(sock, "r");
    // 소켓을 통해 쓰기 위한 파일 포인터 생성
    writefp = fdopen(sock, "w");
  
    // 서버로부터 데이터를 읽어오는 루프
    while(1)
    {
        // 서버로부터 데이터를 읽어서 buf에 저장
        if (fgets(buf, sizeof(buf), readfp) == NULL) 
            break;
        // buf의 내용을 표준 출력에 출력
        fputs(buf, stdout);
        // 표준 출력 버퍼를 비움
        fflush(stdout);
    }  

    // 서버로 메시지를 전송
    fputs("FROM CLIENT: Thank you! \n", writefp);
    // 소켓 쓰기 버퍼를 비움
    fflush(writefp);
    // 파일 포인터를 닫음
    fclose(writefp);
    fclose(readfp);
    // 프로그램 종료
    return 0;
}
