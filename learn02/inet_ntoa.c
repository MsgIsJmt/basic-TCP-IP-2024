#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    struct sockaddr_in addr1, addr2;
    char *str_ptr;
    char str_arr[20];
   
    // addr1과 addr2의 IP 주소를 설정합니다.
    addr1.sin_addr.s_addr = htonl(0x1020304); // addr1을 1.2.3.4로 설정합니다.
    addr2.sin_addr.s_addr = htonl(0x1010101); // addr2를 1.1.1.1로 설정합니다.
    
    // addr1의 IP 주소를 문자열로 변환하여 str_ptr에 저장합니다.
    str_ptr = inet_ntoa(addr1.sin_addr);
    // str_ptr의 IP 주소 문자열을 str_arr에 복사합니다.
    strcpy(str_arr, str_ptr);
    // str_ptr의 IP 주소 문자열을 출력합니다.
    printf("Dotted-Decimal notation1: %s \n", str_ptr);
    
    // addr2의 IP 주소를 문자열로 변환합니다. (결과는 저장되지 않음)
    inet_ntoa(addr2.sin_addr);
    // str_ptr의 IP 주소 문자열을 출력합니다. (이전과 동일함)
    printf("Dotted-Decimal notation2: %s \n", str_ptr);
    // str_arr의 IP 주소 문자열을 출력합니다. (이전과 동일함)
    printf("Dotted-Decimal notation3: %s \n", str_arr);
    
    return 0;
}
