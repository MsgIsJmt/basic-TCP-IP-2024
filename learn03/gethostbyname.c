#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

void error_handling(char *message);  // 에러 처리 함수 프로토타입 선언

int main(int argc, char *argv[])
{
    int i;
    struct hostent *host;  // 호스트 정보를 저장하는 구조체 포인터 선언

    // 명령행 인자의 개수가 올바르지 않을 경우 사용 방법 출력 후 종료
    if (argc != 2) {
        printf("Usage : %s <addr>\n", argv[0]);
        exit(1);
    }

    // 주어진 호스트 이름에 대한 IP 주소 정보를 얻어옴
    host = gethostbyname(argv[1]);  // 주어진 호스트 이름에 대한 호스트 정보를 얻음
    if (!host)
        error_handling("gethost... error");  // 호스트 정보를 얻어오지 못한 경우 에러 처리

    printf("Official name: %s \n", host->h_name);  // 공식적인 호스트 이름 출력

    // 별칭(aliases) 정보 출력
    for (i = 0; host->h_aliases[i]; i++)
        printf("Aliases %d: %s \n", i + 1, host->h_aliases[i]);  // 별칭 정보 출력

    // 주소 유형 정보 출력 (IPv4 또는 IPv6)
    printf("Address type: %s \n", (host->h_addrtype == AF_INET) ? "AF_INET" : "AF_INET6");

    // IP 주소 정보 출력
    for (i = 0; host->h_addr_list[i]; i++)
        printf("IP addr %d: %s \n", i + 1, inet_ntoa(*(struct in_addr *)host->h_addr_list[i]));  // IP 주소 출력

    return 0;
}

// 에러 처리 함수
void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
