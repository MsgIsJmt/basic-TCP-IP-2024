#include <stdio.h>      // 표준 입력/출력 라이브러리
#include <unistd.h>     // POSIX 운영 체제 인터페이스
#define BUF_SIZE 30     // 버퍼 크기 정의

int main(int argc, char *argv[]) {
    int fds[2];         // 파일 디스크립터 배열
    char str[] = "Who are you?";    // 전송할 문자열
    char buf[BUF_SIZE]; // 수신한 데이터를 저장할 버퍼
    pid_t pid;          // 프로세스 ID

    // 파이프 생성
    pipe(fds);
    pid = fork();       // 자식 프로세스 생성

    if (pid == 0) {     // 자식 프로세스인 경우
        close(fds[0]);  // 읽기용 파일 디스크립터 닫기
        write(fds[1], str, sizeof(str));   // 파이프를 통해 문자열 전송
    } else {            // 부모 프로세스인 경우
        close(fds[1]);  // 쓰기용 파일 디스크립터 닫기
        read(fds[0], buf, BUF_SIZE);       // 파이프로부터 데이터 읽기
        puts(buf);      // 읽은 데이터 출력
    }

    return 0;           // 프로그램 종료
}
