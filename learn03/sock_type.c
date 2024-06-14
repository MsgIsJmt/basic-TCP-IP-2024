#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>

// 에러를 처리하는 함수 프로토타입
void error_handling(char *message);

int main(int argc, char *argv[]) 
{
   int tcp_sock, udp_sock;  // TCP와 UDP 소켓을 위한 파일 디스크립터 변수들
   int sock_type;  // 소켓 타입을 저장할 변수
   socklen_t optlen;  // 옵션 길이 변수
   int state;  // 상태 변수
   
   optlen = sizeof(sock_type);  // 옵션 길이 초기화
   tcp_sock = socket(PF_INET, SOCK_STREAM, 0);  // TCP 소켓 생성
   udp_sock = socket(PF_INET, SOCK_DGRAM, 0);  // UDP 소켓 생성

   // SOCK_STREAM과 SOCK_DGRAM의 값 출력
   printf("SOCK_STREAM: %d \n", SOCK_STREAM);
   printf("SOCK_DGRAM: %d \n", SOCK_DGRAM);
   
   // TCP 소켓의 소켓 타입 옵션 가져오기
   state = getsockopt(tcp_sock, SOL_SOCKET, SO_TYPE, (void*)&sock_type, &optlen);
   if(state)
      error_handling("getsockopt() error!");
   printf("Socket type one: %d \n", sock_type);
   
   // UDP 소켓의 소켓 타입 옵션 가져오기
   state = getsockopt(udp_sock, SOL_SOCKET, SO_TYPE, (void*)&sock_type, &optlen);
   if(state)
      error_handling("getsockopt() error!");
   printf("Socket type two: %d \n", sock_type);
   
   return 0;
}

// 에러를 처리하는 함수 정의
void error_handling(char *message)
{
   fputs(message, stderr);  // 에러 메시지를 표준 에러 출력에 출력
   fputc('\n', stderr);  // 표준 에러 출력에 개행 문자 출력
   exit(1);  // 프로그램을 비정상 종료
}
