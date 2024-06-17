#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUF_SIZE 30

void error_handling(char *message);

int main(int argc, char *argv[])
{
    int acpt_sock, recv_sock; // 수신 및 수락용 소켓 디스크립터 변수 선언
    struct sockaddr_in acpt_adr, recv_adr; // 수락 및 수신용 소켓 주소 구조체 변수 선언
    int str_len, state;
    socklen_t recv_adr_sz;
    char buf[BUF_SIZE];

    // 프로그램 실행 시 포트 번호가 정확히 1개의 인자로 전달되지 않으면 사용법을 출력하고 종료합니다.
    if(argc != 2) {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }
    
    // 수락용 소켓 생성
    acpt_sock = socket(PF_INET, SOCK_STREAM, 0);

    // sockaddr_in 구조체 초기화
    memset(&acpt_adr, 0, sizeof(acpt_adr));
    acpt_adr.sin_family = AF_INET;
    acpt_adr.sin_addr.s_addr = htonl(INADDR_ANY); // 모든 네트워크 인터페이스에서 접속 허용
    acpt_adr.sin_port = htons(atoi(argv[1])); // 전달받은 포트 번호를 네트워크 바이트 순서로 변환하여 설정

    // 소켓에 주소(IP, 포트) 바인딩
    if(bind(acpt_sock, (struct sockaddr*)&acpt_adr, sizeof(acpt_adr)) == -1)
        error_handling("bind() error");

    // 연결 요청 대기 상태로 진입
    listen(acpt_sock, 5);

    // 클라이언트의 연결 요청 수락
    recv_adr_sz = sizeof(recv_adr);
    recv_sock = accept(acpt_sock, (struct sockaddr*)&recv_adr, &recv_adr_sz);

    // 수신 버퍼에서 데이터를 peek 하여 확인하고, MSG_PEEK 및 MSG_DONTWAIT 플래그를 사용하여 블로킹 없이 검사
    while(1)
    {
        str_len = recv(recv_sock, buf, sizeof(buf)-1, MSG_PEEK | MSG_DONTWAIT);
        if(str_len > 0)
            break; // 데이터를 peek 했을 때 데이터가 있는 경우 루프 종료
    }

    buf[str_len] = '\0'; // 수신한 데이터에 널 문자 추가하여 문자열로 만듦
    printf("Buffering %d bytes: %s \n", str_len, buf); // peek 한 데이터 출력

    // 실제 데이터를 수신하여 버퍼에 저장하고 출력
    str_len = recv(recv_sock, buf, sizeof(buf)-1, 0);
    buf[str_len] = '\0'; // 수신한 데이터에 널 문자 추가하여 문자열로 만듦
    printf("Read again: %s \n", buf); // 수신한 데이터 출력

    // 소켓 연결 종료
    close(acpt_sock);
    close(recv_sock);
    return 0; 
}

// 오류 발생 시 메시지를 출력하고 프로그램을 종료하는 함수
void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
