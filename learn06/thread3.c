#include <stdio.h>
#include <pthread.h>

// 스레드가 실행할 함수 선언
void * thread_summation(void * arg); 

int sum = 0; // 전역 변수로 선언된 합계 변수

int main(int argc, char *argv[])
{
    pthread_t id_t1, id_t2; // 두 개의 스레드 ID 변수 선언
    int range1[] = {1, 5};  // 첫 번째 스레드에 전달할 범위
    int range2[] = {6, 10}; // 두 번째 스레드에 전달할 범위
    
    // 두 개의 스레드 생성
    pthread_create(&id_t1, NULL, thread_summation, (void *)range1);
    pthread_create(&id_t2, NULL, thread_summation, (void *)range2);

    // 각 스레드가 종료될 때까지 기다림
    pthread_join(id_t1, NULL);
    pthread_join(id_t2, NULL);

    // 최종 계산된 결과 출력
    printf("result: %d \n", sum);
    return 0;
}

// 스레드가 실행할 함수 정의
void * thread_summation(void * arg) 
{
    int start = ((int*)arg)[0]; // 전달받은 배열의 첫 번째 요소를 시작 값으로 설정
    int end = ((int*)arg)[1];   // 전달받은 배열의 두 번째 요소를 종료 값으로 설정

    // 시작부터 종료까지 반복하면서 합을 계산
    while (start <= end)
    {
        sum += start; // 전역 변수 sum에 start 값을 더함
        start++;      // start 값을 증가시킴
    }

    return NULL; // 스레드의 반환 값으로 NULL 반환
}
