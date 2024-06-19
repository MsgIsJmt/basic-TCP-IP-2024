#include <stdio.h>
#include <fcntl.h>  // 파일 제어를 위한 헤더 파일

int main(void)
{
	FILE *fp;  // 파일 포인터 선언
	int fd = open("data.dat", O_WRONLY | O_CREAT | O_TRUNC);  // 파일 열기

	if (fd == -1) {  // 파일 열기 실패 시 오류 처리
		fputs("file open error", stdout);
		return -1;  // 프로그램 종료
	}

	fp = fdopen(fd, "w");  // 파일 디스크립터를 파일 포인터로 변환하여 연다
	fputs("Network C programming \n", fp);  // 파일에 문자열 쓰기
	fclose(fp);  // 파일 포인터 닫기 (파일도 함께 닫힘)
	
	return 0;  // 프로그램 정상 종료
}
