#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

// 스레드가 실행할 함수 정의
void* thread_main(void *arg);

int main(int argc, char *argv[]) 
{
    pthread_t t_id;         // 새로 생성할 스레드의 ID
    int thread_param = 5;   // 스레드에 전달할 매개변수
    void * thr_ret;         // 스레드의 반환 값 저장할 포인터

    // pthread_create를 통해 새로운 스레드 생성
    if (pthread_create(&t_id, NULL, thread_main, (void*)&thread_param) != 0)
    {
        puts("pthread_create() error");
        return -1;
    }

    // pthread_join을 통해 생성한 스레드가 종료될 때까지 대기
    if (pthread_join(t_id, &thr_ret) != 0)
    {
        puts("pthread_join() error");
        return -1;
    }

    // 스레드가 반환한 메시지 출력
    printf("Thread return message: %s \n", (char*)thr_ret);

    // 메모리 해제
    free(thr_ret);

    return 0;
}

// 스레드가 실행할 함수 정의
void* thread_main(void *arg) 
{
    int i;
    int cnt = *((int*)arg);  // void 포인터를 int 포인터로 캐스팅하여 매개변수 값 획득
    char * msg = (char *)malloc(sizeof(char) * 50);  // 메모리 할당

    strcpy(msg, "Hello, I'am thread~ \n");  // 메시지 복사

    // 주어진 횟수(cnt)만큼 반복하여 실행
    for (i = 0; i < cnt; i++)
    {
        sleep(1);  // 1초 대기
        puts("running thread"); // 스레드가 실행 중임을 출력
    }

    return (void*)msg; // 스레드 종료 시 메시지 포인터 반환
}
