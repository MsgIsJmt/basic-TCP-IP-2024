#include <stdio.h>

int main(int argc, char* argv[])
{
	printf("전달하는 인자갯수:%d\n", argc);
	prinrf("실행파일:%s\n", argv[0]);

	for(int i=1; i<argc; i++) {
		printf("전달인자:%s\n", argv[i]);
	}

	return 0;
}
