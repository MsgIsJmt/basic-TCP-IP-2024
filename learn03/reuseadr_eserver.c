#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define TRUE 1
#define FALSE 0

// 에러를 처리하는 함수 프로토타입
void error_handling(char *message);

int main(int argc, char *argv[])
{
   int serv_sock, clnt_sock; // 서버 소켓과 클라이언트 소켓을 위한 파일 디스크립터
   char message[30]; // 메시지를 저장할 배열
   int option, str_len; // 옵션과 문자열 길이를 저장할 변수
   socklen_t optlen, clnt_adr_sz; // 옵션 길이와 클라이언트 주소 길이를 저장할 변수
   struct sockaddr_in serv_adr, clnt_adr; // 서버 주소와 클라이언트 주소를 저장할 구조체
   
   // 실행 인자가 올바르지 않으면 사용법을 출력하고 종료
   if(argc != 2) {
      printf("Usage : %s <port>\n", argv[0]);
      exit(1);
   }
   
   // 서버 소켓을 생성
   serv_sock = socket(PF_INET, SOCK_STREAM, 0);
   if(serv_sock == -1)
      error_handling("socket() error");

   /*
   optlen = sizeof(option);
   option = TRUE;   
   setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &option, optlen);
   */

   // 서버 주소 구조체를 초기화
   memset(&serv_adr, 0, sizeof(serv_adr));
   serv_adr.sin_family = AF_INET;
   serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
   serv_adr.sin_port = htons(atoi(argv[1]));

   // 서버 소켓에 주소를 바인딩
   if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
      error_handling("bind() error ");
   
   // 서버 소켓을 리슨 상태로 설정
   if(listen(serv_sock, 5) == -1)
      error_handling("listen error");

   // 클라이언트 소켓을 수락
   clnt_adr_sz = sizeof(clnt_adr);    
   clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);

   // 클라이언트 소켓에서 데이터를 읽고 다시 클라이언트로 전송
   while((str_len = read(clnt_sock, message, sizeof(message))) != 0)
   {
      write(clnt_sock, message, str_len); // 클라이언트에게 메시지를 전송
      write(1, message, str_len); // 터미널에 메시지를 출력
   }

   // 클라이언트 소켓을 닫음
   close(clnt_sock);
   return 0;
}

// 에러를 처리하는 함수 정의
void error_handling(char *message)
{
   fputs(message, stderr); // 에러 메시지를 표준 에러 출력에 출력
   fputc('\n', stderr); // 표준 에러 출력에 개행 문자 출력
   exit(1); // 프로그램을 비정상 종료
}

/*
root@com:/home/swyoon/tcpip# gcc echo_server.c -o echo_server
root@com:/home/swyoon/tcpip# ./echo_server 9190
*/
