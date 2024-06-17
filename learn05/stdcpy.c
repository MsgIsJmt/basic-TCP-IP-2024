#include <stdio.h>
#define BUF_SIZE 3  // 버퍼 크기 정의

int main(int argc, char *argv[])
{
	FILE *fp1;  // 파일 포인터 fp1 선언
	FILE *fp2;  // 파일 포인터 fp2 선언
	char buf[BUF_SIZE];  // 데이터를 저장할 버퍼 배열 선언
	
	// news.txt 파일을 읽기 모드("r")로 열기
	fp1 = fopen("news.txt", "r");
	if (fp1 == NULL) {  // 파일 열기 실패 시 오류 처리
		perror("Error opening news.txt");
		return 1;
	}
	
	// cpy.txt 파일을 쓰기 모드("w")로 열기
	fp2 = fopen("cpy.txt", "w");
	if (fp2 == NULL) {  // 파일 열기 실패 시 오류 처리
		perror("Error opening cpy.txt");
		fclose(fp1);  // 열린 파일 포인터 닫기
		return 1;
	}
	
	// fp1에서 데이터를 한 줄씩 읽어서 buf에 저장하고, fp2에 쓰는 과정을 반복
	while (fgets(buf, BUF_SIZE, fp1) != NULL) {
		fputs(buf, fp2);  // buf의 내용을 fp2에 씁니다.
	}
	
	fclose(fp1);  // 파일 포인터 fp1 닫기
	fclose(fp2);  // 파일 포인터 fp2 닫기
	
	return 0;  // 프로그램 정상 종료
}
