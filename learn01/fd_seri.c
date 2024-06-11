#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>

int main(void)
{   
    int fd1, fd2, fd3; // 파일 디스크립터 변수 선언

    // 소켓 생성
    fd1=socket(PF_INET, SOCK_STREAM, 0);
    // 파일 생성 및 열기
    fd2=open("test.dat", O_CREAT|O_WRONLY|O_TRUNC);
    // 소켓 생성
    fd3=socket(PF_INET, SOCK_DGRAM, 0);
    
    // 파일 디스크립터 출력
    printf("file descriptor 1: %d\n", fd1);
    printf("file descriptor 2: %d\n", fd2);
    printf("file descriptor 3: %d\n", fd3);
    
    // 파일 디스크립터 닫기
    close(fd1);
    close(fd2);
    close(fd3);
    return 0;
}
