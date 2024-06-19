#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
void error_handling(char *message);

int main(int argc, char *argv[])
{
	int sock;                // 소켓 파일 디스크립터
	char message[BUF_SIZE];  // 메시지를 저장할 버퍼
	int str_len;             // 읽은 문자열의 길이
	struct sockaddr_in serv_adr;  // 서버 주소 구조체
	FILE *readfp;            // 읽기 스트림
	FILE *writefp;           // 쓰기 스트림

	if(argc != 3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_STREAM, 0);   // TCP 소켓 생성
	if(sock == -1)
		error_handling("socket() error");

	memset(&serv_adr, 0, sizeof(serv_adr));  // 서버 주소 구조체 초기화
	serv_adr.sin_family = AF_INET;           // IPv4 주소 체계 사용
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);  // 서버 IP 주소 설정
	serv_adr.sin_port = htons(atoi(argv[2]));       // 서버 포트 번호 설정

	if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)  // 서버에 연결 요청
		error_handling("connect() error!");
	else
		puts("Connected...........");

	readfp = fdopen(sock, "r");    // 소켓을 읽기 모드로 파일 스트림 열기
	writefp = fdopen(sock, "w");   // 소켓을 쓰기 모드로 파일 스트림 열기

	while(1)
	{
		fputs("Input message(Q to quit): ", stdout);  // 사용자에게 메시지 입력 안내 출력
		fgets(message, BUF_SIZE, stdin);              // 사용자로부터 메시지 입력 받음
		if(!strcmp(message,"q\n") || !strcmp(message,"Q\n"))  // 사용자가 'q' 또는 'Q' 입력 시 종료
			break;

		fputs(message, writefp);  // 입력 받은 메시지를 서버로 전송
		fflush(writefp);          // 쓰기 버퍼 비우기 (서버로 전송)

 		fgets(message, BUF_SIZE, readfp);  // 서버로부터의 응답 읽기
		printf("Message from server: %s", message);  // 서버의 응답 출력
	}	

	fclose(writefp);   // 쓰기 파일 포인터 닫기
	fclose(readfp);    // 읽기 파일 포인터 닫기
	close(sock);       // 소켓 닫기
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);  // 오류 메시지 출력
	fputc('\n', stderr);
	exit(1);  // 프로그램 종료
}
