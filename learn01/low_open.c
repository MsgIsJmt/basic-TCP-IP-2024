#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
void error_handling(char* message); // 에러 처리 함수 선언

int main(void)
{
    int fd; // 파일 디스크립터
    char buf[]="Let's go!\n"; // 파일에 쓸 데이터

    // 파일 열기
    fd=open("data.txt", O_CREAT|O_WRONLY|O_TRUNC); // data.txt 파일을 생성-쓰기전용-이전에 있었던 내용은 지우고
    if(fd==-1)
        error_handling("open() error!");
    printf("file descriptor: %d \n", fd); // 파일 디스크립터 출력

    // 파일에 데이터 쓰기
    if(write(fd, buf, sizeof(buf))==-1)
        error_handling("write() error!");

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
