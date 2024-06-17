#include <stdio.h>
#include <fcntl.h>  // 파일 제어를 위한 헤더 파일
#define BUF_SIZE 3   // 버퍼 크기 정의

int main(int argc, char *argv[])
{
	int fd1, fd2, len;  // 파일 디스크립터와 길이 변수 선언
	char buf[BUF_SIZE]; // 데이터를 저장할 버퍼 선언

	// news.txt 파일을 읽기 전용으로 엽니다.
	fd1 = open("news.txt", O_RDONLY);
	if (fd1 == -1) {  // 파일 열기 실패 시 오류 처리
		perror("open news.txt");
		return 1;
	}

	// cpy.txt 파일을 쓰기 전용으로 엽니다. 파일이 없으면 생성하고, 이미 있으면 내용을 초기화합니다.
	fd2 = open("cpy.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd2 == -1) {  // 파일 열기 실패 시 오류 처리
		perror("open cpy.txt");
		close(fd1);  // 열린 파일 디스크립터 닫기
		return 1;
	}

	// news.txt에서 데이터를 읽어 cpy.txt에 쓰는 과정
	while ((len = read(fd1, buf, sizeof(buf))) > 0) {  // 파일에서 데이터를 읽고, 읽은 바이트 수를 len에 저장
		write(fd2, buf, len);  // 읽은 데이터를 cpy.txt에 씁니다.
	}

	// 파일 닫기
	close(fd1);
	close(fd2);
	return 0;
}
