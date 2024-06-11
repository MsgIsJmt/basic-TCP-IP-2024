#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

void error_handling(char *message); // 에러 처리 함수 선언

int main(int argc, char *argv[])
{
    // 변환할 IP 주소를 저장하는 포인터 변수 선언
    char *addr="127.232.124.79";
    // sockaddr_in 구조체 변수 선언
    struct sockaddr_in addr_inet;
    
    // inet_aton 함수를 통해 문자열 형태의 IP 주소를 네트워크 순서의 32비트 정수로 변환
    if(!inet_aton(addr, &addr_inet.sin_addr))
        error_handling("Conversion error"); // 변환 중 에러가 발생한 경우 에러 메시지 출력
    else
        // 변환된 IP 주소를 출력
        printf("Network ordered integer addr: %#x \n", addr_inet.sin_addr.s_addr); // 네트워크 순서의 32비트 정수 형태로 출력
    
    return 0;
}

// 에러 처리 함수
void error_handling(char *message)
{
    fputs(message, stderr); // 표준 오류 스트림에 메시지 출력
    fputc('\n', stderr); // 개행 출력
    exit(1); // 프로그램 종료
}
