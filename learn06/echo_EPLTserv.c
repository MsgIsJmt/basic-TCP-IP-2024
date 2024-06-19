#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>

#define BUF_SIZE 4
#define EPOLL_SIZE 50

void error_handling(char *buf);

int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;
    socklen_t adr_sz;
    int str_len, i;
    char buf[BUF_SIZE];

    struct epoll_event *ep_events;
    struct epoll_event event;
    int epfd, event_cnt;

    if (argc != 2) {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    // 서버 소켓 생성
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1)
        error_handling("socket() error");

    // 서버 주소 설정
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    // 서버 소켓에 주소 할당
    if (bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("bind() error");

    // 연결 요청 대기열 설정
    if (listen(serv_sock, 5) == -1)
        error_handling("listen() error");

    // epoll 생성 및 이벤트 배열 할당
    epfd = epoll_create(EPOLL_SIZE);
    ep_events = malloc(sizeof(struct epoll_event) * EPOLL_SIZE);

    // 서버 소켓을 epoll 관찰 대상으로 등록
    event.events = EPOLLIN;
    event.data.fd = serv_sock;
    epoll_ctl(epfd, EPOLL_CTL_ADD, serv_sock, &event);

    while (1)
    {
        // epoll 이벤트 감시
        event_cnt = epoll_wait(epfd, ep_events, EPOLL_SIZE, -1);
        if (event_cnt == -1)
        {
            perror("epoll_wait() error");
            break;
        }

        printf("return epoll_wait\n");

        // 이벤트 발생한 모든 소켓에 대해 처리
        for (i = 0; i < event_cnt; i++)
        {
            if (ep_events[i].data.fd == serv_sock)  // 서버 소켓에서 이벤트 발생
            {
                adr_sz = sizeof(clnt_adr);
                clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
                if (clnt_sock == -1)
                {
                    perror("accept() error");
                    continue;
                }

                // 연결된 클라이언트 소켓을 epoll 관찰 대상으로 등록
                event.events = EPOLLIN;
                event.data.fd = clnt_sock;
                epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sock, &event);
                printf("connected client: %d \n", clnt_sock);
            }
            else  // 클라이언트 소켓에서 이벤트 발생
            {
                str_len = read(ep_events[i].data.fd, buf, BUF_SIZE);
                if (str_len == 0)  // 클라이언트 연결 종료
                {
                    epoll_ctl(epfd, EPOLL_CTL_DEL, ep_events[i].data.fd, NULL);
                    close(ep_events[i].data.fd);
                    printf("closed client: %d \n", ep_events[i].data.fd);
                }
                else  // 클라이언트로부터 데이터 수신 및 에코
                {
                    write(ep_events[i].data.fd, buf, str_len);  // 수신된 데이터를 클라이언트에게 다시 전송 (에코)
                }
            }
        }
    }

    close(serv_sock);  // 서버 소켓 닫기
    close(epfd);  // epoll 파일 디스크립터 닫기
    return 0;
}

void error_handling(char *buf)
{
    perror(buf);  // 에러 메시지 출력
    exit(1);  // 프로그램 종료
}
