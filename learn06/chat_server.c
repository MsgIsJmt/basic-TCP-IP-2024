#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define BUF_SIZE 100   // 메시지 버퍼 크기
#define MAX_CLNT 256   // 최대 클라이언트 수

void *handle_clnt(void *arg);   // 클라이언트 처리 함수
void send_msg(char *msg, int len);   // 모든 클라이언트에게 메시지 전송 함수
void error_handling(char *msg);   // 에러 처리 함수

int clnt_cnt = 0;   // 현재 클라이언트 수
int clnt_socks[MAX_CLNT];   // 클라이언트 소켓을 저장하는 배열
pthread_mutex_t mutx;   // 뮤텍스 변수

int main(int argc, char *argv[]) 
{
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;
    int clnt_adr_sz;
    pthread_t t_id;

    // 인자로 포트 번호가 주어지지 않으면 사용법을 출력하고 종료
    if (argc != 2) {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    // 뮤텍스 초기화
    pthread_mutex_init(&mutx, NULL);

    // 서버 소켓 생성
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);

    // 서버 주소 설정
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET; 
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    // 서버 소켓에 주소 bind
    if (bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("bind() error");
    
    // 클라이언트의 접속을 대기하는 상태로 전환
    if (listen(serv_sock, 5) == -1)
        error_handling("listen() error");

    // 클라이언트의 접속을 계속해서 받아들이는 루프
    while (1) {
        clnt_adr_sz = sizeof(clnt_adr);
        // 클라이언트의 연결을 수락하고 클라이언트 소켓 생성
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);

        // 클라이언트 소켓 배열과 뮤텍스를 이용하여 클라이언트 소켓 정보 저장
        pthread_mutex_lock(&mutx);
        clnt_socks[clnt_cnt++] = clnt_sock;
        pthread_mutex_unlock(&mutx);

        // 클라이언트 처리를 위한 스레드 생성 및 분리
        pthread_create(&t_id, NULL, handle_clnt, (void*)&clnt_sock);
        pthread_detach(t_id);

        // 연결된 클라이언트의 IP 주소 출력
        printf("Connected client IP: %s \n", inet_ntoa(clnt_adr.sin_addr));
    }

    // 서버 소켓 닫기
    close(serv_sock);
    return 0;
}

// 클라이언트 처리 함수
void *handle_clnt(void *arg) 
{
    int clnt_sock = *((int*)arg);
    int str_len = 0, i;
    char msg[BUF_SIZE];

    // 클라이언트로부터 메시지를 읽어들이고 모든 클라이언트에게 전송
    while ((str_len = read(clnt_sock, msg, sizeof(msg))) != 0)
        send_msg(msg, str_len);

    // 클라이언트가 종료되면 해당 클라이언트 정보를 배열에서 삭제하고 소켓 닫기
    pthread_mutex_lock(&mutx);
    for (i = 0; i < clnt_cnt; i++) {
        if (clnt_sock == clnt_socks[i]) {
            while (i++ < clnt_cnt - 1)
                clnt_socks[i] = clnt_socks[i + 1];
            break;
        }
    }
    clnt_cnt--;
    pthread_mutex_unlock(&mutx);
    close(clnt_sock);

    return NULL;
}

// 모든 클라이언트에게 메시지 전송
void send_msg(char *msg, int len) 
{
    int i;
    pthread_mutex_lock(&mutx);
    for (i = 0; i < clnt_cnt; i++)
        write(clnt_socks[i], msg, len);
    pthread_mutex_unlock(&mutx);
}

// 에러 처리 함수
void error_handling(char *msg) 
{
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(1);
}
