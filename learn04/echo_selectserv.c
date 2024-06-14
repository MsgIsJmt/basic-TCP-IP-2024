#include <stdio.h>          // 표준 입력/출력 라이브러리
#include <stdlib.h>         // 표준 라이브러리
#include <string.h>         // 문자열 처리를 위한 라이브러리
#include <unistd.h>         // POSIX 운영 체제 인터페이스
#include <arpa/inet.h>      // 인터넷 주소 변환을 위한 라이브러리
#include <sys/socket.h>     // 소켓 통신을 위한 라이브러리
#include <sys/time.h>       // 시간 관련 함수를 위한 라이브러리
#include <sys/select.h>     // 다중 I/O 관련 함수를 위한 라이브러리

#define BUF_SIZE 100        // 버퍼 크기 정의

void error_handling(char *buf);    // 에러 처리 함수 선언

int main(int argc, char *argv[]) {
    int serv_sock, clnt_sock;       // 서버 소켓과 클라이언트 소켓
    struct sockaddr_in serv_adr, clnt_adr;  // 서버 및 클라이언트 주소 구조체
    struct timeval timeout;         // 타임아웃을 위한 구조체
    fd_set reads, cpy_reads;        // 파일 디스크립터 집합
    socklen_t adr_sz;               // 주소 구조체 크기
    int fd_max, str_len, fd_num, i; // 파일 디스크립터 정보 및 문자열 길이 변수
    char buf[BUF_SIZE];             // 버퍼

    if(argc != 2) {                 // 포트 번호가 입력되지 않은 경우
        printf("Usage : %s <port>\n", argv[0]);    // 사용법 출력
        exit(1);                    // 프로그램 종료
    }

    // 서버 소켓 생성
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0, sizeof(serv_adr));       // 서버 주소 구조체 초기화
    serv_adr.sin_family = AF_INET;                 // IPv4 주소 체계 사용
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);  // 모든 인터페이스로부터 연결 요청 수락
    serv_adr.sin_port = htons(atoi(argv[1]));      // 포트 번호 설정

    // 소켓에 주소 할당
    if(bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr)) == -1)
        error_handling("bind() error");

    // 연결 요청 대기열 생성
    if(listen(serv_sock, 5) == -1)
        error_handling("listen() error");

    // 파일 디스크립터 집합 초기화 및 서버 소켓 추가
    FD_ZERO(&reads);
    FD_SET(serv_sock, &reads);
    fd_max = serv_sock;

    while(1) {
        cpy_reads = reads;                      // 파일 디스크립터 집합 복사
        timeout.tv_sec = 5;                     // 타임아웃 초 설정
        timeout.tv_usec = 5000;                 // 타임아웃 마이크로초 설정

        // 이벤트 발생 대기 (select 함수 호출)
        if((fd_num = select(fd_max + 1, &cpy_reads, 0, 0, &timeout)) == -1)
            break;                              // 에러 발생 시 루프 탈출

        // 타임아웃 발생 시
        if(fd_num == 0)
            continue;                           // 다음 반복문 수행

        // 파일 디스크립터 집합을 순회하면서 이벤트 발생 확인
        for(i = 0; i < fd_max + 1; i++) {
            if(FD_ISSET(i, &cpy_reads)) {      // 파일 디스크립터에 이벤트가 발생한 경우
                if(i == serv_sock) {            // 연결 요청인 경우
                    adr_sz = sizeof(clnt_adr);
                    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);  // 클라이언트 소켓 생성
                    FD_SET(clnt_sock, &reads);  // 클라이언트 소켓을 파일 디스크립터 집합에 추가
                    if(fd_max < clnt_sock)      // 파일 디스크립터 최댓값 업데이트
                        fd_max = clnt_sock;
                    printf("connected client: %d \n", clnt_sock);   // 연결된 클라이언트 알림
                } else {                        // 데이터 수신인 경우
                    str_len = read(i, buf, BUF_SIZE);   // 클라이언트로부터 데이터 수신
                    if(str_len == 0) {          // 클라이언트 종료 요청인 경우
                        FD_CLR(i, &reads);      // 파일 디스크립터 집합에서 제거
                        close(i);               // 클라이언트 소켓 닫기
                        printf("closed client: %d \n", i);   // 클라이언트 종료 알림
                    } else {                    // 데이터 수신인 경우
                        write(i, buf, str_len); // 수신한 데이터를 클라이언트에게 다시 전송
                    }
                }
            }
        }
    }
    close(serv_sock);   // 서버 소켓 닫기
    return 0;           // 프로그램 종료
}

// 에러 처리 함수 정의
void error_handling(char *buf) {
    fputs(buf, stderr); // 에러 메시지 출력
    fputc('\n', stderr);    // 개행 문자 출력
    exit(1);            // 프로그램 종료
}
