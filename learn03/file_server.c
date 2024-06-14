#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30  // 파일을 읽고 쓸 버퍼 크기

void error_handling(char *message);  // 에러 처리 함수 프로토타입 선언

int main(int argc, char *argv[])
{
    int serv_sd, clnt_sd;  // 서버와 클라이언트 소켓 파일 디스크립터
    FILE *fp;  // 파일 포인터
    char buf[BUF_SIZE];  // 파일 데이터를 저장할 버퍼
    int read_cnt;  // 읽은 데이터의 길이

    struct sockaddr_in serv_adr, clnt_adr;  // 서버와 클라이언트 주소 구조체
    socklen_t clnt_adr_sz;  // 클라이언트 주소 구조체의 크기를 저장할 변수

    // 명령행 인자의 개수가 올바르지 않을 경우 사용 방법 출력 후 종료
    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    // 파일 열기
    fp = fopen("file_server.c", "rb");  // "file_server.c" 파일을 바이너리 모드로 읽기 전용으로 열기

    // TCP 소켓 생성
    serv_sd = socket(PF_INET, SOCK_STREAM, 0);  // IPv4를 사용하는 스트림 소켓 생성
    if (serv_sd == -1)
        error_handling("socket() error");

    // 서버 주소 구조체 초기화
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;  // IPv4 주소 체계
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);  // 서버 IP 주소는 자동으로 할당
    serv_adr.sin_port = htons(atoi(argv[1]));  // 명령행 인자로 전달받은 포트 번호 설정

    // 소켓에 주소 할당
    if (bind(serv_sd, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("bind() error");

    // 연결 요청 대기 상태 진입
    if (listen(serv_sd, 5) == -1)
        error_handling("listen() error");

    clnt_adr_sz = sizeof(clnt_adr);
    // 클라이언트의 연결 요청 수락
    clnt_sd = accept(serv_sd, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
    if (clnt_sd == -1)
        error_handling("accept() error");

    // 파일 데이터 전송
    while (1) {
        // 파일에서 데이터 읽기
        read_cnt = fread((void*)buf, 1, BUF_SIZE, fp);
        if (read_cnt < BUF_SIZE) {
            // 파일의 끝에 도달하면 남은 데이터만 전송하고 루프 종료
            write(clnt_sd, buf, read_cnt);
            break;
        }
        // 읽은 데이터를 클라이언트에게 전송
        write(clnt_sd, buf, BUF_SIZE);
    }

    // 클라이언트에게 더 이상 데이터를 보낼 수 없음을 알리기 위해 연결 종료
    shutdown(clnt_sd, SHUT_WR);
    
    // 클라이언트로부터 메시지 수신 및 출력
    read(clnt_sd, buf, BUF_SIZE);
    printf("Message from client: %s \n", buf);

    // 파일 포인터와 소켓 닫기
    fclose(fp);
    close(clnt_sd);
    close(serv_sd);
    return 0;
}

// 에러 처리 함수
void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
