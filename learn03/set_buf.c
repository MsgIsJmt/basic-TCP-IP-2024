#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>

void error_handling(char *message);  // 에러 처리 함수 프로토타입 선언

int main(int argc, char *argv[])
{
    int sock;  // 소켓 파일 디스크립터
    int snd_buf = 1024 * 3, rcv_buf = 1024 * 3;  // 송신 버퍼 크기와 수신 버퍼 크기 초기화
    int state;  // 상태 변수
    socklen_t len;  // 소켓 옵션 값을 저장할 변수의 크기를 나타내는 데이터 타입

    // TCP 소켓 생성
    sock = socket(PF_INET, SOCK_STREAM, 0);  // IPv4를 사용하는 스트림 소켓 생성
    if (sock == -1)
        error_handling("socket() error");  // 소켓 생성에 실패한 경우 에러 처리

    // 수신 버퍼 크기 설정
    state = setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void*)&rcv_buf, sizeof(rcv_buf));  // 수신 버퍼 크기를 설정함
    if (state)
        error_handling("setsockopt() error!");  // 소켓 옵션 설정에 실패한 경우 에러 처리

    // 송신 버퍼 크기 설정
    state = setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void*)&snd_buf, sizeof(snd_buf));  // 송신 버퍼 크기를 설정함
    if (state)
        error_handling("setsockopt() error!");  // 소켓 옵션 설정에 실패한 경우 에러 처리

    // 송신 버퍼 크기 조회
    len = sizeof(snd_buf);
    state = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void*)&snd_buf, &len);  // 송신 버퍼 크기를 조회하여 가져옴
    if (state)
        error_handling("getsockopt() error!");  // 소켓 옵션 조회에 실패한 경우 에러 처리

    // 수신 버퍼 크기 조회
    len = sizeof(rcv_buf);
    state = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void*)&rcv_buf, &len);  // 수신 버퍼 크기를 조회하여 가져옴
    if (state)
        error_handling("getsockopt() error!");  // 소켓 옵션 조회에 실패한 경우 에러 처리

    // 송신 버퍼 크기와 수신 버퍼 크기 출력
    printf("Input buffer size: %d \n", rcv_buf);  // 수신 버퍼 크기 출력
    printf("Output buffer size: %d \n", snd_buf);  // 송신 버퍼 크기 출력

    return 0;
}

// 에러 처리 함수
void error_handling(char *message)
{
    fputs(message, stderr);  // 에러 메시지 출력
    fputc('\n', stderr);  // 개행
    exit(1);  // 프로그램 종료
}
