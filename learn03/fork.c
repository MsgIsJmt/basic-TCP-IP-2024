#include <stdio.h>
#include <unistd.h>

int gval = 10;  // 전역 변수 선언 및 초기화

int main(int argc, char *argv[])
{
    pid_t pid;  // 프로세스 ID를 저장할 변수 선언
    int lval = 20;  // 지역 변수 선언 및 초기화

    gval++;  // 전역 변수 증가
    lval += 5;  // 지역 변수 증가

    pid = fork();  // 자식 프로세스 생성
    if (pid == 0) {  // 자식 프로세스인 경우
        gval += 2;  // 전역 변수 증가
        lval += 2;  // 지역 변수 증가
    } else {  // 부모 프로세스인 경우
        gval -= 2;  // 전역 변수 감소
        lval -= 2;  // 지역 변수 감소
    }

    if (pid == 0)  // 자식 프로세스인 경우
        printf("Child Proc: [%d, %d] \n", gval, lval);  // 자식 프로세스에서 전역 변수와 지역 변수 출력
    else  // 부모 프로세스인 경우
        printf("Parent Proc: [%d, %d] \n", gval, lval);  // 부모 프로세스에서 전역 변수와 지역 변수 출력

    return 0;
}
