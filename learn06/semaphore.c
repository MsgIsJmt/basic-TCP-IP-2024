#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

void *read(void *arg);
void *accu(void *arg);

static sem_t sem_one; // 세마포어 1 (read -> accu)
static sem_t sem_two; // 세마포어 2 (accu -> read)
static int num;       // 전역 변수로 공유될 값

int main(int argc, char *argv[])
{
    pthread_t id_t1, id_t2;
    
    // 세마포어 초기화
    sem_init(&sem_one, 0, 0); // sem_one 초기값 0
    sem_init(&sem_two, 0, 1); // sem_two 초기값 1 (처음에 accu 스레드가 시작하도록)

    // 스레드 생성
    pthread_create(&id_t1, NULL, read, NULL); // read 스레드 생성
    pthread_create(&id_t2, NULL, accu, NULL); // accu 스레드 생성

    // 스레드 종료 대기
    pthread_join(id_t1, NULL);
    pthread_join(id_t2, NULL);

    // 세마포어 삭제
    sem_destroy(&sem_one);
    sem_destroy(&sem_two);
    return 0;
}

// 입력을 받는 스레드 함수
void *read(void *arg)
{
    int i;
    for (i = 0; i < 5; i++)
    {
        fputs("Input num: ", stdout);

        sem_wait(&sem_two); // sem_two 락을 획득 (처음에는 1이므로 바로 진행)
        scanf("%d", &num);  // num 입력 받음
        sem_post(&sem_one); // sem_one 락 해제 (accu 스레드에게 통보)
    }
    return NULL;
}

// 누적을 계산하는 스레드 함수
void *accu(void *arg)
{
    int sum = 0, i;
    for (i = 0; i < 5; i++)
    {
        sem_wait(&sem_one); // sem_one 락을 획득 (read 스레드가 통보할 때까지 대기)
        sum += num;         // num 값을 누적
        sem_post(&sem_two); // sem_two 락 해제 (read 스레드에게 통보)
    }
    printf("Result: %d \n", sum); // 최종 결과 출력
    return NULL;
}
