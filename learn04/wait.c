#include <stdio.h>      // 표준 입력/출력 라이브러리
#include <stdlib.h>     // 표준 라이브러리
#include <unistd.h>     // POSIX 운영 체제 인터페이스
#include <sys/wait.h>   // 자식 프로세스의 종료를 대기하기 위한 라이브러리

int main(int argc, char *argv[]) {
    int status;             // 자식 프로세스의 종료 상태를 저장할 변수
    pid_t pid = fork();     // 자식 프로세스를 생성하고, 그 결과를 저장할 변수

    if (pid == 0) {         // 자식 프로세스인 경우
        return 3;           // 종료 상태로 3을 반환하고 종료
    } else {                // 부모 프로세스인 경우
        printf("Child PID: %d \n", pid);  // 자식 프로세스의 PID를 출력
        pid = fork();       // 두 번째 자식 프로세스를 생성하고, 그 결과를 저장

        if (pid == 0) {     // 두 번째 자식 프로세스인 경우
            exit(7);        // 종료 상태로 7을 반환하고 종료
        } else {            // 부모 프로세스인 경우
            printf("Child PID: %d \n", pid);  // 두 번째 자식 프로세스의 PID를 출력
            wait(&status);  // 첫 번째 자식 프로세스의 종료를 대기하고, 종료 상태를 저장
            if (WIFEXITED(status))  // 자식 프로세스가 정상적으로 종료한 경우
                printf("Child send one: %d \n", WEXITSTATUS(status));  // 첫 번째 자식 프로세스의 종료 상태를 출력

            wait(&status);  // 두 번째 자식 프로세스의 종료를 대기하고, 종료 상태를 저장
            if (WIFEXITED(status))  // 자식 프로세스가 정상적으로 종료한 경우
                printf("Child send two: %d \n", WEXITSTATUS(status));  // 두 번째 자식 프로세스의 종료 상태를 출력

            sleep(30);      // 30초 동안 대기
        }
    }
    return 0;               // 프로그램 종료
}
