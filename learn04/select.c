#include <stdio.h>          // 표준 입력/출력 라이브러리
#include <unistd.h>         // POSIX 운영 체제 인터페이스
#include <sys/time.h>       // 시간 관련 함수를 위한 라이브러리
#include <sys/select.h>     // select 함수를 위한 라이브러리

#define BUF_SIZE 30         // 버퍼 크기 정의

int main(int argc, char *argv[]) {
    fd_set reads, temps;        // 파일 디스크립터 집합
    int result, str_len;        // select 함수 결과 및 문자열 길이 변수
    char buf[BUF_SIZE];         // 버퍼
    struct timeval timeout;     // 타임아웃 설정 구조체

    FD_ZERO(&reads);            // 파일 디스크립터 집합 초기화
    FD_SET(0, &reads);          // 표준 입력(콘솔)에 대한 파일 디스크립터 설정

    /*
    timeout.tv_sec = 5;         // 타임아웃 초 설정
    timeout.tv_usec = 5000;     // 타임아웃 마이크로초 설정
    */

    while(1) {
        temps = reads;                      // 임시 파일 디스크립터 집합 설정
        timeout.tv_sec = 5;                 // 타임아웃 초 설정
        timeout.tv_usec = 0;                // 타임아웃 마이크로초 설정
        result = select(1, &temps, 0, 0, &timeout);    // select 함수 호출

        if(result == -1) {                  // select 함수 실패 시
            puts("select() error!");        // 에러 메시지 출력
            break;                          // 루프 탈출
        } else if(result == 0) {            // select 함수에서 타임아웃 발생 시
            puts("Time-out!");              // 타임아웃 메시지 출력
        } else {                            // select 함수에서 이벤트 발생 시
            if(FD_ISSET(0, &temps)) {       // 표준 입력(콘솔)에 이벤트가 발생한 경우
                str_len = read(0, buf, BUF_SIZE);     // 표준 입력에서 데이터 읽기
                buf[str_len] = '\0';                    // 문자열 끝을 표시하는 널 문자 추가
                printf("message from console: %s", buf);   // 받은 메시지 출력
            }
        }
    }
    return 0;   // 프로그램 종료
}
