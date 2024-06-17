#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30  // 버퍼 크기 정의
void error_handling(char *message);  // 오류 처리 함수 원형 선언

int main(int argc, char *argv[])
{
	int recv_sock;           // 수신용 소켓 디스크립터
	int str_len;             // 수신된 데이터 길이
	char buf[BUF_SIZE];      // 데이터를 저장할 버퍼
	struct sockaddr_in adr;  // 소켓 주소 구조체
	struct ip_mreq join_adr; // 멀티캐스트 그룹 가입 정보 구조체
	
	// 프로그램 실행 시 멀티캐스트 그룹의 IP 주소와 포트 번호가 정확히 2개의 인자로 전달되지 않으면 사용법을 출력하고 종료합니다.
	if(argc != 3) {
		printf("Usage : %s <GroupIP> <PORT>\n", argv[0]);
		exit(1);
	}
  
	// UDP 소켓 생성
	recv_sock = socket(PF_INET, SOCK_DGRAM, 0);
	
	// 소켓 주소 구조체 초기화
	memset(&adr, 0, sizeof(adr));
	adr.sin_family = AF_INET;  // IPv4 주소 체계 사용
	adr.sin_addr.s_addr = htonl(INADDR_ANY);  // 모든 네트워크 인터페이스로부터 수신
	adr.sin_port = htons(atoi(argv[2]));  // 사용할 포트 번호 설정
	
	// 소켓에 주소 할당
	if(bind(recv_sock, (struct sockaddr*) &adr, sizeof(adr)) == -1)
		error_handling("bind() error");
	
	// 멀티캐스트 그룹 가입 설정
	join_adr.imr_multiaddr.s_addr = inet_addr(argv[1]);  // 멀티캐스트 그룹의 IP 주소 설정
	join_adr.imr_interface.s_addr = htonl(INADDR_ANY);   // 모든 네트워크 인터페이스 사용
	
	// 소켓 옵션 설정: IP_ADD_MEMBERSHIP을 사용하여 멀티캐스트 그룹에 가입
	setsockopt(recv_sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void*)&join_adr, sizeof(join_adr));
  
	// 데이터 수신 및 출력 루프
	while(1)
	{
		str_len = recvfrom(recv_sock, buf, BUF_SIZE-1, 0, NULL, 0);  // 데이터 수신
		if(str_len < 0)  // 수신 오류 발생 시 루프 종료
			break;
		buf[str_len] = '\0';  // 수신한 데이터를 문자열로 만들기 위해 NULL 문자 추가
		fputs(buf, stdout);   // 수신한 데이터를 표준 출력에 출력
	}
	
	close(recv_sock);  // 소켓 닫기
	return 0;
}

// 오류 발생 시 메시지를 출력하고 프로그램을 종료하는 함수
void error_handling(char *message)
{
	fputs(message, stderr);  // 오류 메시지 출력
	fputc('\n', stderr);     // 새 줄로 이동
	exit(1);                 // 프로그램 종료
}
