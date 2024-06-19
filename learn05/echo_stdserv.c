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
	int serv_sock, clnt_sock;  // 서버 소켓, 클라이언트 소켓 변수 선언
	char message[BUF_SIZE];    // 메시지를 저장할 버퍼
	int str_len, i;

	struct sockaddr_in serv_adr;  // 서버 주소 구조체
	struct sockaddr_in clnt_adr;  // 클라이언트 주소 구조체
	socklen_t clnt_adr_sz;        // 클라이언트 주소 구조체 크기 변수
	FILE *readfp;                 // 파일 스트림으로 열린 소켓을 읽기 위한 파일 포인터
	FILE *writefp;                // 파일 스트림으로 열린 소켓을 쓰기 위한 파일 포인터

	if(argc != 2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);  // TCP 소켓 생성
	if(serv_sock == -1)
		error_handling("socket() error");

	memset(&serv_adr, 0, sizeof(serv_adr));       // 서버 주소 초기화
	serv_adr.sin_family = AF_INET;                // IPv4 주소 체계 사용
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY); // 모든 IP 주소에서 연결 요청 수락
	serv_adr.sin_port = htons(atoi(argv[1]));     // 포트 설정

	if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)  // 소켓에 주소 할당
		error_handling("bind() error");

	if(listen(serv_sock, 5) == -1)  // 연결 요청 대기 큐 설정
		error_handling("listen() error");

	clnt_adr_sz = sizeof(clnt_adr);  // 클라이언트 주소 구조체 크기 초기화

	for(i = 0; i < 5; i++)  // 최대 5개의 클라이언트 접속 처리
	{
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);  // 클라이언트의 연결 수락
		if(clnt_sock == -1)
			error_handling("accept() error");
		else
			printf("Connected client %d \n", i + 1);

		readfp = fdopen(clnt_sock, "r");    // 클라이언트 소켓을 읽기 모드로 파일 스트림 열기
		writefp = fdopen(clnt_sock, "w");   // 클라이언트 소켓을 쓰기 모드로 파일 스트림 열기

		while(!feof(readfp))  // 읽기 파일 포인터가 EOF가 아닌 동안 반복
		{
			fgets(message, BUF_SIZE, readfp);  // 클라이언트로부터 메시지 읽기
			fputs(message, writefp);           // 읽은 메시지를 클라이언트에게 다시 보내기
			fflush(writefp);                   // 쓰기 버퍼 비우기
		}

		fclose(readfp);   // 읽기 파일 포인터 닫기
		fclose(writefp);  // 쓰기 파일 포인터 닫기
	}

	close(serv_sock);  // 서버 소켓 닫기
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);  // 오류 메시지 출력
	fputc('\n', stderr);
	exit(1);  // 프로그램 종료
}
