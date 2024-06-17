#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define TTL 64         // TTL 값 설정 (Time-To-Live)
#define BUF_SIZE 30    // 버퍼 크기 설정
void error_handling(char *message);

int main(int argc, char *argv[])
{
	int send_sock;                  // 송신용 소켓 디스크립터
	struct sockaddr_in mul_adr;     // 멀티캐스트 주소 구조체
	int time_live = TTL;            // TTL 값 설정 변수
	FILE *fp;                       // 파일 포인터
	char buf[BUF_SIZE];             // 데이터 버퍼

	// 프로그램 실행 시 멀티캐스트 그룹의 IP 주소와 포트 번호가 정확히 2개의 인자로 전달되지 않으면 사용법을 출력하고 종료합니다.
	if(argc != 3){
		printf("Usage : %s <GroupIP> <PORT>\n", argv[0]);
		exit(1);
	}
  	
	// 송신용 UDP 소켓 생성
	send_sock = socket(PF_INET, SOCK_DGRAM, 0);

	// 멀티캐스트 주소 구조체 초기화
	memset(&mul_adr, 0, sizeof(mul_adr));
	mul_adr.sin_family = AF_INET;
	mul_adr.sin_addr.s_addr = inet_addr(argv[1]);  // 멀티캐스트 그룹의 IP 주소 설정
	mul_adr.sin_port = htons(atoi(argv[2]));       // 멀티캐스트 포트 번호 설정
	
	// 소켓 옵션 설정: IP_MULTICAST_TTL 옵션을 사용하여 TTL 값을 설정
	setsockopt(send_sock, IPPROTO_IP, IP_MULTICAST_TTL, (void*)&time_live, sizeof(time_live));
	
	// 파일 열기
	if((fp=fopen("news.txt", "r"))==NULL)
		error_handling("fopen() error");

	// 파일의 끝까지 데이터를 읽어서 멀티캐스트로 전송 (브로드캐스팅)
	while(!feof(fp))
	{
		fgets(buf, BUF_SIZE, fp);  // 파일에서 한 줄씩 데이터 읽기
		sendto(send_sock, buf, strlen(buf), 0, (struct sockaddr*)&mul_adr, sizeof(mul_adr));  // 멀티캐스트로 데이터 전송
		sleep(2);  // 2초간 sleep
	}

	fclose(fp);         // 파일 닫기
	close(send_sock);   // 소켓 닫기
	return 0;
}

// 오류 발생 시 메시지를 출력하고 프로그램을 종료하는 함수
void error_handling(char *message)
{
	fputs(message, stderr);  // 오류 메시지 출력
	fputc('\n', stderr);     // 새 줄로 이동
	exit(1);                 // 프로그램 종료
}
