#include <stdio.h>
#include <sys/uio.h> // writev 함수를 위한 헤더 파일

int main(int argc, char *argv[])
{
    struct iovec vec[2]; // iovec 구조체 배열 선언
    char buf1[] = "ABCDEFG"; // 첫 번째 버퍼
    char buf2[] = "1234567"; // 두 번째 버퍼
    int str_len;

    // 첫 번째 iovec 구조체 설정
    vec[0].iov_base = buf1; // 버퍼 시작 주소
    vec[0].iov_len = 3; // 버퍼 길이

    // 두 번째 iovec 구조체 설정
    vec[1].iov_base = buf2; // 버퍼 시작 주소
    vec[1].iov_len = 4; // 버퍼 길이
    
    // writev 함수를 사용하여 vec 배열에 있는 버퍼들의 데이터를 한 번에 표준 출력(1)으로 출력
    str_len = writev(1, vec, 2); // 2는 vec 배열의 원소 개수

    puts(""); // 줄 바꿈
    printf("Write bytes: %d \n", str_len); // 쓰여진 바이트 수 출력
    return 0;
}
