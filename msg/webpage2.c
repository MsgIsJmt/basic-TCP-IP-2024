#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sendfile.h>

#define TRUE 1

char webpage[] = "HTTP/1.1 200 OK\r\n"
                 "Server: Linux Web Server\r\n"
                 "Content-Type: text/html; charset=UTF-8\r\n\r\n"
                 "<!DOCTYPE html>\r\n"
                 "<html><head><title>나의 웹 페이지</title>\r\n"
                 "<link rel=\"icon\" href=\"data:,\">\r\n"
                 "<style>body {background-color: #FFFF00 }</style></head>\r\n"
                 "<body><center><h1>Hello world!!</h1><br>\r\n"
                 "<img src=\"myimage.jpg\"></center></body></html>\r\n";

int main(int argc, char *argv[])
{
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t sin_len = sizeof(clnt_addr);
    int serv_sock, clnt_sock;
    char buf[2048];
    int fdimg, img_size;
    int option = TRUE;
    char img_buf[700000];

    // 소켓 생성
    serv_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (serv_sock < 0)
    {
        perror("socket() 오류");
        exit(1);
    }

    // 소켓 옵션 설정: 주소 재사용
    setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(int));

    // 서버 주소 초기화 및 설정
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(atoi(argv[1]));

    // 소켓에 서버 주소 바인딩
    if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
    {
        perror("bind() 오류");
        close(serv_sock);
        exit(1);
    }

    // 클라이언트 연결 대기
    if (listen(serv_sock, 5) == -1)
    {
        perror("listen() 오류");
        close(serv_sock);
        exit(1);
    }

    while (1)
    {
        // 클라이언트 연결 수락
        clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &sin_len);
        if (clnt_sock < 0)
        {
            perror("accept() 오류");
            continue;
        }
        printf("새로운 클라이언트 연결...\n");

        // 클라이언트 요청 읽기
        read(clnt_sock, buf, sizeof(buf));
        printf("%s\n", buf);

        // 요청이 이미지 파일인지 확인
        if (strstr(buf, "GET /myimage.jpg") != NULL)
        {
            // 이미지 파일 열고 읽기
            fdimg = open("myimage.jpg", O_RDONLY);
            if (fdimg == -1)
            {
                perror("open() 오류");
                close(clnt_sock);
                continue;
            }
            img_size = read(fdimg, img_buf, sizeof(img_buf));
            if (img_size == -1)
            {
                perror("read() 오류");
                close(fdimg);
                close(clnt_sock);
                continue;
            }
            close(fdimg);

            // HTTP 응답 헤더 준비 및 전송
            sprintf(buf, "HTTP/1.1 200 OK\r\n"
                         "Server: Linux Web Server\r\n"
                         "Content-Type: image/jpeg\r\n"
                         "Content-Length: %ld\r\n\r\n", img_size);
            if (write(clnt_sock, buf, strlen(buf)) < 0)
            {
                perror("write() 오류");
                close(clnt_sock);
                continue;
            }

            // 이미지 데이터 전송
            if (write(clnt_sock, img_buf, img_size) < 0)
            {
                perror("write() 오류");
                close(clnt_sock);
                continue;
            }

            // 클라이언트 소켓 닫기
            close(clnt_sock);
        }
        else
        {
            // HTML 웹 페이지 전송
            if (write(clnt_sock, webpage, sizeof(webpage) - 1) == -1)
            {
                perror("write() 오류");
                close(clnt_sock);
                continue;
            }
            printf("HTML 페이지 전송...\n");

            // 클라이언트 소켓 닫기
            close(clnt_sock);
        }
    }

    // 서버 소켓 닫기
    close(serv_sock);
    return 0;
}
