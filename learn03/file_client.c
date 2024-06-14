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
    int sd;  // 소켓 파일 디스크립터
    FILE *fp;  // 파일 포인터
    char buf[BUF_SIZE];  // 파일 데이터를 읽고 쓸 버퍼
    int read_cnt;  // 읽은 데이터의 길이

    struct sockaddr_in serv_adr;  // 서버 주소 구조체

    // 명령행 인자의 개수가 올바르지 않을 경우 사용 방법 출력 후 종료
    if (argc != 3) {
        printf("Usage: %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    // 수신된 파일을 저장할 파일 생성
    fp = fopen("receive.dat", "wb");  // "receive.dat" 파일을 바이너리 모드로 쓰기 전용으로 열기

    // TCP 소켓 생성
    sd = socket(PF_INET, SOCK_STREAM, 0);  // IPv4를 사용하는 스트림 소켓 생성
    if (sd == -1)
        error_handling("socket() error");

    // 서버 주소 구조체 초기화 및 설정
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;  // IPv4 주소 체계
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);  // 명령행 인자로 전달받은 서버 IP 주소 설정
    serv_adr.sin_port = htons(atoi(argv[2]));  // 명령행 인자로 전달받은 서버 포트 번호 설정

    // 서버에 연결 요청
    if (connect(sd, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("connect() error");

    // 서버로부터 파일 데이터를 읽어서 파일에 씀
    while ((read_cnt = read(sd, buf, BUF_SIZE)) != 0)
        fwrite((void*)buf, 1, read_cnt, fp);

    // 파일 데이터 수신 완료 메시지 출력
    puts("Received file data");

    // 서버에게 감사 메시지 전송
    write(sd, "Thank you", 10);

    // 파일 포인터와 소켓 닫기
    fclose(fp);
    close(sd);
    return 0;
}

// 에러 처리 함수
void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

