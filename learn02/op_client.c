#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024 // 버퍼 크기 정의
#define RLT_SIZE 4    // 결과의 크기 정의
#define OPSZ 4        // 오퍼랜드의 크기 정의

void error_handling(char *message); // 에러 핸들링 함수의 프로토타입 선언

int main(int argc, char *argv[])
{
    int sock; // 소켓 디스크립터
    char opmsg[BUF_SIZE]; // 오퍼레이션 메시지를 저장할 배열
    int result, opnd_cnt, i; // 결과, 오퍼랜드 개수, 반복 변수
    struct sockaddr_in serv_adr; // 서버 주소 정보 구조체
    
    if(argc!=3) { // 프로그램 실행 시 IP 주소와 포트 번호를 인자로 받지 않았을 경우 사용법 안내 후 종료
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }
    
    // 소켓 생성
    sock = socket(PF_INET, SOCK_STREAM, 0);   
    if(sock == -1)
        error_handling("socket() error");
    
    // 서버 주소 정보 구조체 초기화 및 설정
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET; // IPv4 주소 체계 사용
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]); // 인자로 받은 IP 주소를 설정
    serv_adr.sin_port = htons(atoi(argv[2])); // 인자로 받은 포트 번호를 설정
    
    // 서버에 연결
    if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("connect() error!");
    else
        puts("Connected...........");

    // 오퍼랜드 개수 입력
    fputs("Operand count: ", stdout);
    scanf("%d", &opnd_cnt);
    opmsg[0] = (char)opnd_cnt; // 오퍼랜드 개수를 메시지에 저장
    
    // 오퍼랜드 입력
    for(i=0; i<opnd_cnt; i++)
    {
        printf("Operand %d: ", i+1);
        scanf("%d", (int*)&opmsg[i*OPSZ+1]); // 오퍼랜드를 메시지에 저장
    }
    fgetc(stdin); // 입력 버퍼 비우기
    fputs("Operator: ", stdout);
    scanf("%c", &opmsg[opnd_cnt*OPSZ+1]); // 오퍼레이터 입력
    write(sock, opmsg, opnd_cnt*OPSZ+2); // 서버에 메시지 전송
    read(sock, &result, RLT_SIZE); // 서버로부터 결과 수신
    
    printf("Operation result: %d \n", result); // 결과 출력
    close(sock); // 소켓 닫기
    return 0;
}

// 에러 핸들링 함수 정의
void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
