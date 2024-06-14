#include <stdio.h>      // 표준 입력/출력 라이브러리
#include <unistd.h>     // POSIX 운영 체제 인터페이스
#define BUF_SIZE 30     // 버퍼 크기 정의

int main(int argc, char *argv[]) {
    int fds[2];                 // 파일 디스크립터 배열
    char str1[] = "Who are you?";               // 첫 번째 문자열
    char str2[] = "Thank you for your message";  // 두 번째 문자열
    char buf[BUF_SIZE];        // 수신한 데이터를 저장할 버퍼
    pid_t pid;                  // 프로세스 ID

    // 파이프 생성
    pipe(fds);
    pid = fork();               // 자식 프로세스 생성

    if (pid == 0) {             // 자식 프로세스인 경우
        write(fds[1], str1, sizeof(str1));    // 파이프를 통해 첫 번째 문자열 전송
        sleep(2);               // 2초 동안 대기
        read(fds[0], buf, BUF_SIZE);           // 파이프로부터 데이터 읽기
        printf("Child proc output: %s \n",  buf);   // 읽은 데이터 출력
    } else {                    // 부모 프로세스인 경우
        read(fds[0], buf, BUF_SIZE);           // 파이프로부터 데이터 읽기
        printf("Parent proc output: %s \n", buf);  // 읽은 데이터 출력
        write(fds[1], str2, sizeof(str2));    // 파이프를 통해 두 번째 문자열 전송
        sleep(3);               // 3초 동안 대기
    }
    return 0;                   // 프로그램 종료
}
