#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

void error_handling(char *message); // 에러 처리 함수 선언

int main(int argc, char* argv[])
{
    int sock; // 소켓 파일 디스크립터
    struct sockaddr_in serv_addr; // 서버 주소 정보를 담는 구조체
    char message[30]; // 서버로부터 받을 메시지를 저장할 배열
    int str_len=0; // 읽은 메시지의 길이
    int idx=0, read_len=0; // 배열 인덱스, read 함수의 반환 값 저장 변수

    // 인자 개수 확인
    if(argc!=3){
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    // 소켓 생성
    sock=socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1)
        error_handling("socket() error");

    // 서버 주소 설정
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family=AF_INET; // IPv4 주소 체계 사용
    serv_addr.sin_addr.s_addr=inet_addr(argv[1]); // 서버 IP 주소 설정
    serv_addr.sin_port=htons(atoi(argv[2])); // 서버 포트 번호 설정

    // 서버에 연결
    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1)
        error_handling("connect() error!");

    // 서버로부터 메시지 읽기
    // 한 번에 1바이트씩 읽어와 message 배열에 저장
    while(read_len=read(sock, &message[idx++], 1))
    {
        if(read_len==-1)
            error_handling("read() error!");

        str_len+=read_len; // 읽은 바이트 수를 누적
    }

    // 받은 메시지와 read 함수 호출 횟수 출력
    printf("Message from server: %s \n", message);
    printf("Function read call count: %d \n", str_len);

    close(sock); // 소켓 닫기
    return 0;
}

// 에러 처리 함수
void error_handling(char *message)
{
    fputs(message, stderr); // 표준 오류 스트림에 메시지 출력
    fputc('\n', stderr); // 개행 출력
    exit(1); // 프로그램 종료
}
