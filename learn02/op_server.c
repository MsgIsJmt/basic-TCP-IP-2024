#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024 // 버퍼 크기 정의
#define OPSZ 4        // 오퍼랜드 크기 정의

void error_handling(char *message); // 에러 핸들링 함수의 프로토타입 선언
int calculate(int opnum, int opnds[], char op); // 계산 함수의 프로토타입 선언

int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock; // 서버와 클라이언트 소켓 디스크립터
    char opinfo[BUF_SIZE]; // 오퍼레이션 정보를 저장할 배열
    int result, opnd_cnt, i; // 결과, 오퍼랜드 개수, 반복 변수
    int recv_cnt, recv_len; // 수신 카운트, 수신 길이
    struct sockaddr_in serv_adr, clnt_adr; // 서버와 클라이언트 주소 정보 구조체
    socklen_t clnt_adr_sz; // 클라이언트 주소 정보 구조체의 크기
    
    if(argc != 2) { // 포트 번호를 인자로 받지 않았을 경우 사용법 안내 후 종료
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }
    
    // 소켓 생성
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);   
    if(serv_sock == -1)
        error_handling("socket() error");
    
    // 서버 주소 정보 구조체 초기화 및 설정
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET; // IPv4 주소 체계 사용
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY); // 모든 인터페이스에서 연결을 수신할 수 있도록 설정
    serv_adr.sin_port = htons(atoi(argv[1])); // 인자로 받은 포트 번호를 설정
    
    // 소켓에 주소 할당
    if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("bind() error");
    
    // 클라이언트의 연결 요청 대기
    if(listen(serv_sock, 5) == -1)
        error_handling("listen() error");  
    
    clnt_adr_sz = sizeof(clnt_adr);

    for(i = 0; i < 5; i++)
    {
        opnd_cnt = 0;
        // 클라이언트의 연결 수락 및 오퍼랜드 개수 수신
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);    
        read(clnt_sock, &opnd_cnt, 1); // 오퍼랜드 개수 수신
        
        recv_len = 0;
        // 오퍼랜드 및 오퍼레이터 수신
        while((opnd_cnt * OPSZ + 1) > recv_len)
        {
            recv_cnt = read(clnt_sock, &opinfo[recv_len], BUF_SIZE - 1);
            recv_len += recv_cnt;
        }
        // 계산 수행 및 결과 전송
        result = calculate(opnd_cnt, (int*)opinfo, opinfo[recv_len - 1]);
        write(clnt_sock, (char*)&result, sizeof(result));
        close(clnt_sock); // 클라이언트 소켓 닫기
    }
    
    close(serv_sock); // 서버 소켓 닫기
    return 0;
}

// 계산 함수 정의
int calculate(int opnum, int opnds[], char op)
{
    int result = opnds[0], i;
    
    switch(op)
    {
        case '+':
            for(i = 1; i < opnum; i++) 
                result += opnds[i];
            break;
        case '-':
            for(i = 1; i < opnum; i++) 
                result -= opnds[i];
            break;
        case '*':
            for(i = 1; i < opnum; i++) 
                result *= opnds[i];
            break;
    }
    return result;
}

// 에러 핸들링 함수 정의
void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
