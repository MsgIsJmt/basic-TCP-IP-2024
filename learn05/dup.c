#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int cfd1, cfd2;  // 파일 디스크립터 변수 선언
    char str1[] = "Hi~ \n";  // 첫 번째 문자열
    char str2[] = "It's a nice day~ \n";  // 두 번째 문자열

    // 표준 출력(1번 파일 디스크립터)의 복사본 생성
    cfd1 = dup(1);  // cfd1에는 새로운 파일 디스크립터가 복사됨
    // cfd1의 복사본을 파일 디스크립터 7로 지정
    cfd2 = dup2(cfd1, 7);  // cfd1을 cfd2에 복사하고, cfd2를 7번 파일 디스크립터로 지정

    // 파일 디스크립터 값 출력
    printf("fd1=%d, fd2=%d \n", cfd1, cfd2);

    // cfd1을 통해 str1 문자열을 쓰기
    write(cfd1, str1, sizeof(str1));
    // cfd2(7번 파일 디스크립터)를 통해 str2 문자열을 쓰기
    write(cfd2, str2, sizeof(str2));
    
    // cfd1, cfd2 파일 디스크립터 닫기
    close(cfd1);
    close(cfd2);

    // 표준 출력(1번 파일 디스크립터)을 통해 str1 문자열을 쓰기
    write(1, str1, sizeof(str1));
    // 표준 출력(1번 파일 디스크립터)을 닫기
    close(1);
    // 다시 표준 출력(1번 파일 디스크립터)을 통해 str2 문자열을 쓰기 (하지만 출력되지 않음)
    write(1, str2, sizeof(str2));

    return 0;
}
