#include <stdio.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    // 호스트 순서로 지정된 포트와 IP 주소를 저장할 변수 선언
    unsigned short host_port=0x1234;
    unsigned long host_addr=0x12345678;

    // 네트워크 순서로 변환된 포트와 IP 주소를 저장할 변수 선언
    unsigned short net_port;
    unsigned long net_addr;
    
    // htons 함수를 사용하여 호스트 순서의 포트를 네트워크 순서로 변환
    net_port=htons(host_port);
    // htonl 함수를 사용하여 호스트 순서의 IP 주소를 네트워크 순서로 변환
    net_addr=htonl(host_addr);
    
    // 변환된 포트와 주소를 출력
    printf("Host ordered port: %#x \n", host_port); // 호스트 순서의 포트 출력
    printf("Network ordered port: %#x \n", net_port); // 네트워크 순서의 포트 출력
    printf("Host ordered address: %#lx \n", host_addr); // 호스트 순서의 IP 주소 출력
    printf("Network ordered address: %#lx \n", net_addr); // 네트워크 순서의 IP 주소 출력
    
    return 0;
}
