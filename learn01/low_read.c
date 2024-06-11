#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#define BUF_SIZE 100 // 버퍼 크기를 상수로 정의
void error_handling(char* message); // 에러 처리 함수 선언

int main(void)
{
    int fd; // 파일 디스크립터
    char buf[BUF_SIZE]; // 파일에서 읽은 데이터를 저장할 버퍼

    // 파일 열기
    fd=open("data.txt", O_RDONLY);
    if(fd==-1)
        error_handling("open() error!");

    printf("file descriptor: %d \n" , fd); // 파일 디스크립터 출력

    // 파일에서 데이터 읽기
    if(read(fd, buf, sizeof(buf))==-1)
        error_handling("read() error!");

    printf("file data: %s", buf); // 읽은 데이터 출력

    close(fd); // 파일 닫기
    return 0;
}

// 에러 처리 함수
void error_handling(char* message)
{
    fputs(message, stderr); // 표준 오류 스트림에 메시지 출력
    fputc('\n', stderr); // 개행 출력
    exit(1); // 프로그램 종료
}
