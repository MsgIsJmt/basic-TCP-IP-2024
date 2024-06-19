#include <stdio.h>
#include <pthread.h>

// 스레드가 실행할 함수 정의
void* thread_main(void *arg);

int main(int argc, char *argv[]) 
{
    pthread_t t_id;       // 새로 생성할 스레드의 ID
    int thread_param = 5; // 스레드에 전달할 매개변수

    // pthread_create를 통해 새로운 스레드 생성
    if (pthread_create(&t_id, NULL, thread_main, (void*)&thread_param) != 0)
    {
        puts("pthread_create() error");
        return -1;
    }

    sleep(10);  // 메인 스레드가 10초 동안 대기
    puts("end of main"); // 메인 스레드 종료 메시지 출력
    return 0;
}

// 스레드가 실행할 함수 정의
void* thread_main(void *arg) 
{
    int i;
    int cnt = *((int*)arg); // void 포인터를 int 포인터로 캐스팅하여 매개변수 값 획득

    // 주어진 횟수(cnt)만큼 반복하여 실행
    for (i = 0; i < cnt; i++)
    {
        sleep(1);  // 1초 대기
        puts("running thread"); // 스레드가 실행 중임을 출력
    }

    return NULL; // 스레드 종료
}
