#include <stdio.h>      // 표준 입력/출력 라이브러리
#include <unistd.h>     // POSIX 운영 체제 인터페이스
#include <sys/wait.h>   // 자식 프로세스의 종료를 대기하기 위한 라이브러리

int main(int argc, char *argv[]) {
    int status;                 // 자식 프로세스의 종료 상태를 저장할 변수
    pid_t pid = fork();         // 자식 프로세스를 생성하고, 그 결과를 저장할 변수

    if (pid == 0) {             // 자식 프로세스인 경우
        sleep(15);              // 15초 동안 대기
        return 24;              // 종료 상태로 24를 반환하고 종료
    } else {                    // 부모 프로세스인 경우
        while (!waitpid(-1, &status, WNOHANG)) {  // 모든 자식 프로세스의 종료를 대기하는 동안 반복
            sleep(3);           // 3초 동안 대기
            puts("sleep 3sec.");   // "sleep 3sec."를 출력
        }

        if (WIFEXITED(status)) // 자식 프로세스가 정상적으로 종료한 경우
            printf("Child send %d \n", WEXITSTATUS(status));  // 자식 프로세스의 종료 상태를 출력
    }
    return 0;   // 프로그램 종료
}
