#include <stdio.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    // IP 주소를 문자열로 저장하는 포인터 변수 선언
    char *addr1="127.212.124.78"; // 유효한 IP 주소
    char *addr2="127.212.124.256"; // 잘못된 IP 주소

    // inet_addr 함수를 통해 문자열 형태의 IP 주소를 네트워크 순서의 정수 형태로 변환
    unsigned long conv_addr=inet_addr(addr1);
    // 변환 결과가 INADDR_NONE(전달된 문자열이 올바른 IP 주소 형식이 아님을 나타내는 값)인지 확인
    if(conv_addr==INADDR_NONE)
        printf("Error occured! \n"); // 변환 중 에러가 발생한 경우 에러 메시지 출력
    else
        printf("Network ordered integer addr: %#lx \n", conv_addr); // 변환된 IP 주소 출력
    
    // 잘못된 IP 주소에 대해 동일한 과정 반복
    conv_addr=inet_addr(addr2);
    if(conv_addr==INADDR_NONE)
        printf("Error occureded \n"); // 에러 메시지 출력
    else
        printf("Network ordered integer addr: %#lx \n\n", conv_addr); // 변환된 IP 주소 출력

    return 0;
}
