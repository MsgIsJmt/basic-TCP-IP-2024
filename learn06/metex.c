#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREAD   100

void *thread_inc(void *arg);
void *thread_des(void *arg);

long long num = 0; // 전역 변수로 선언된 값
pthread_mutex_t mutex; // 뮤텍스 선언

int main(int argc, char *argv[]) 
{
    pthread_t thread_id[NUM_THREAD]; // 스레드 핸들을 저장할 배열
    int i;
    
    pthread_mutex_init(&mutex, NULL); // 뮤텍스 초기화 함수

    // NUM_THREAD 개수만큼 스레드를 생성하고 실행
    for (i = 0; i < NUM_THREAD; i++)
    {
        if (i % 2)
            pthread_create(&(thread_id[i]), NULL, thread_inc, NULL); // 홀수 인덱스는 증가 스레드 생성
        else
            pthread_create(&(thread_id[i]), NULL, thread_des, NULL); // 짝수 인덱스는 감소 스레드 생성
    }

    // 모든 스레드의 종료를 기다림
    for (i = 0; i < NUM_THREAD; i++)
        pthread_join(thread_id[i], NULL);

    printf("result: %lld \n", num); // 최종 계산된 값 출력
    pthread_mutex_destroy(&mutex); // 뮤텍스 해제 함수
    return 0;
}

// 증가 스레드 함수 정의
void *thread_inc(void *arg) 
{
    int i;
    pthread_mutex_lock(&mutex); // 뮤텍스 락을 획득
    for (i = 0; i < 50000000; i++)
        num += 1; // num 값을 5000만 번 증가시킴
    pthread_mutex_unlock(&mutex); // 뮤텍스 언락
    return NULL; // 스레드 종료
}

// 감소 스레드 함수 정의
void *thread_des(void *arg)
{
    int i;
    for (i = 0; i < 50000000; i++)
    {
        pthread_mutex_lock(&mutex); // 뮤텍스 락을 획득
        num -= 1; // num 값을 5000만 번 감소시킴
        pthread_mutex_unlock(&mutex); // 뮤텍스 언락
    }
    return NULL; // 스레드 종료
}
