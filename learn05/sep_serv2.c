#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 1024

int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock;  // 서버 소켓과 클라이언트 소켓을 위한 파일 디스크립터 변수
    FILE *readfp, *writefp;    // 클라이언트와의 통신을 위한 파일 포인터

    struct sockaddr_in serv_adr, clnt_adr;  // 서버와 클라이언트의 주소 정보 구조체
    socklen_t clnt_adr_sz;  // 클라이언트 주소 구조체의 크기 변수
    char buf[BUF_SIZE] = {0,};  // 데이터를 주고받을 버퍼

    // 소켓 생성
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1) {
        perror("socket() error");
        exit(1);
    }

    // 서버 주소 구조체 초기화
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;  // IPv4 주소 체계 사용
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);  // 모든 네트워크 인터페이스에서 연결을 받아들임
    serv_adr.sin_port = htons(atoi(argv[1]));  // 명령줄 인수로 전달된 포트 번호 설정

    // 소켓에 주소 바인딩
    if (bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1) {
        perror("bind() error");
        exit(1);
    }

    // 연결 요청 대기 상태로 진입
    if (listen(serv_sock, 5) == -1) {
        perror("listen() error");
        exit(1);
    }

    // 클라이언트의 주소 정보를 저장할 구조체의 크기 설정
    clnt_adr_sz = sizeof(clnt_adr);

    // 클라이언트로부터 연결 요청 수락
    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
    if (clnt_sock == -1) {
        perror("accept() error");
        exit(1);
    }

    // 클라이언트와 통신을 위한 파일 포인터 생성
    readfp = fdopen(clnt_sock, "r");
    writefp = fdopen(dup(clnt_sock), "w");

    // 클라이언트에게 메시지 전송
    fputs("FROM SERVER: Hi~ client? \n", writefp);
    fputs("I love all of the world \n", writefp);
    fputs("You are awesome! \n", writefp);
    fflush(writefp);  // 버퍼 비우기

    // 쓰기 전용 소켓 종료
    shutdown(fileno(writefp), SHUT_WR);
    fclose(writefp);  // 쓰기 파일 포인터 닫기

    // 클라이언트로부터 데이터 수신 및 출력
    fgets(buf, sizeof(buf), readfp);  // 클라이언트가 보낸 데이터 읽기
    fputs(buf, stdout);  // 표준 출력에 클라이언트가 보낸 데이터 출력
    fclose(readfp);  // 읽기 파일 포인터 닫기

    // 서버 소켓 닫기
    close(serv_sock);

    return 0;
}
