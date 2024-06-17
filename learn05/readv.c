#include <stdio.h>
#include <sys/uio.h> // readv 함수를 위한 헤더 파일
#define BUF_SIZE 100

int main(int argc, char *argv[])
{
    struct iovec vec[2]; // iovec 구조체 배열 선언
    char buf1[BUF_SIZE] = {0,}; // 첫 번째 버퍼 초기화
    char buf2[BUF_SIZE] = {0,}; // 두 번째 버퍼 초기화
    int str_len;

    // 첫 번째 iovec 구조체 설정
    vec[0].iov_base = buf1; // 첫 번째 버퍼 시작 주소
    vec[0].iov_len = 5; // 첫 번째 버퍼 길이 (5바이트)

    // 두 번째 iovec 구조체 설정
    vec[1].iov_base = buf2; // 두 번째 버퍼 시작 주소
    vec[1].iov_len = BUF_SIZE; // 두 번째 버퍼 길이 (전체 버퍼 크기)

    // readv 함수를 사용하여 표준 입력(0)으로부터 vec 배열에 있는 두 개의 버퍼로 데이터를 한 번에 읽어옴
    str_len = readv(0, vec, 2); // 2는 vec 배열의 원소 개수

    // 읽은 바이트 수 출력
    printf("Read bytes: %d \n", str_len);

    // 첫 번째 버퍼에 저장된 메시지 출력
    printf("First message: %s \n", buf1);

    // 두 번째 버퍼에 저장된 메시지 출력
    printf("Second message: %s \n", buf2);

    return 0;
}
