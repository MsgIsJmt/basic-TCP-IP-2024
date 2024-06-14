#include <stdio.h>      // 표준 입력/출력 라이브러리
#include <stdlib.h>     // 표준 라이브러리
#include <unistd.h>     // POSIX 운영 체제 인터페이스
#include <signal.h>     // 시그널 처리를 위한 라이브러리
#include <sys/wait.h>   // 자식 프로세스의 종료를 대기하기 위한 라이브러리

// SIGCHLD 시그널 핸들러 함수
void read_childproc(int sig) {
    int status;
    pid_t id = waitpid(-1, &status, WNOHANG);   // 종료된 자식 프로세스의 PID를 가져옴
    if (WIFEXITED(status)) {   // 자식 프로세스가 정상적으로 종료했을 경우
        printf("Removed proc id: %d \n", id);   // 종료된 자식 프로세스의 PID 출력
        printf("Child send: %d \n", WEXITSTATUS(status));   // 자식 프로세스가 보낸 종료 상태 출력
    }
}

int main(int argc, char *argv[]) {
    pid_t pid;
    struct sigaction act;
    act.sa_handler = read_childproc;    // SIGCHLD 시그널에 대한 핸들러 함수 설정
    sigemptyset(&act.sa_mask);          // 시그널 마스크 초기화
    act.sa_flags = 0;                   // 시그널 처리 옵션 설정

    sigaction(SIGCHLD, &act, 0);        // SIGCHLD 시그널에 대한 핸들러 등록

    pid = fork();   // 첫 번째 자식 프로세스 생성
    if (pid == 0) { // 첫 번째 자식 프로세스인 경우
        puts("Hi! I'm child process");
        sleep(10);
        return 12;  // 종료 상태로 12를 반환하고 종료
    } else {        // 부모 프로세스인 경우
        printf("Child proc id: %d \n", pid);  // 첫 번째 자식 프로세스의 PID 출력
        pid = fork();   // 두 번째 자식 프로세스 생성
        if (pid == 0) { // 두 번째 자식 프로세스인 경우
            puts("Hi! I'm child process");
            sleep(10);
            exit(24);   // 종료 상태로 24를 반환하고 종료
        } else {        // 부모 프로세스인 경우
            int i;
            printf("Child proc id: %d \n", pid);  // 두 번째 자식 프로세스의 PID 출력
            for (i = 0; i < 5; i++) {   // 5번 반복
                puts("wait...");        // "wait..."를 출력
                sleep(5);               // 5초 동안 대기
            }
        }
    }
    return 0;   // 프로그램 종료
}
