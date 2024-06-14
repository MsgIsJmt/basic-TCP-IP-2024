#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    pid_t pid = fork();  // fork() 함수를 호출하여 자식 프로세스 생성 후 그 프로세스의 PID를 저장하는 변수 선언

    if (pid == 0)  // 자식 프로세스인 경우
    {
        puts("Hi I'm a child process");  // "Hi I'm a child process" 메시지 출력
    }
    else  // 부모 프로세스인 경우
    {
        printf("Child Process ID: %d \n", pid);  // 자식 프로세스의 PID 출력
        sleep(30);  // 부모 프로세스가 30초 동안 대기
    }

    if (pid == 0)  // 자식 프로세스인 경우
        puts("End child process");  // "End child process" 메시지 출력
    else  // 부모 프로세스인 경우
        puts("End parent process");  // "End parent process" 메시지 출력

    return 0;  // 프로그램 종료
}
