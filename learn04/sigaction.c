#include <stdio.h>      // 표준 입력/출력 라이브러리
#include <unistd.h>     // POSIX 운영 체제 인터페이스
#include <signal.h>     // 시그널 처리를 위한 라이브러리

// SIGALRM 시그널 핸들러 함수
void timeout(int sig) {
    if (sig == SIGALRM)    // 받은 시그널이 SIGALRM인 경우
        puts("Time out!"); // "Time out!"을 출력

    alarm(2);   // 2초 후에 SIGALRM 시그널을 발생시키도록 알람 설정
}

int main(int argc, char *argv[]) {
    int i;
    struct sigaction act;   // 시그널 처리를 위한 구조체
    act.sa_handler = timeout;   // 시그널 핸들러 함수를 설정
    sigemptyset(&act.sa_mask);  // 시그널 마스크를 비움
    act.sa_flags = 0;           // 시그널 처리 옵션 설정

    sigaction(SIGALRM, &act, 0);    // SIGALRM 시그널에 대한 핸들러 설정

    alarm(2);   // 2초 후에 SIGALRM 시그널을 발생시키도록 알람 설정

    for (i = 0; i < 3; i++) {   // 3번 반복
        puts("wait...");        // "wait..."를 출력
        sleep(100);             // 100초 동안 대기
    }
    return 0;   // 프로그램 종료
}
