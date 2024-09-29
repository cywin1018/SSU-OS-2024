#include "user.h"    // 사용자 프로그램이 커널과 상호작용할 수 있는 함수들이 선언된 헤더 파일
#include "fcntl.h"   // 파일 제어 옵션과 파일 열기 모드를 정의한 헤더 파일

#define BUFFER_SIZE 50  
#define SEEK_SET 0       // 파일 시작점으로부터의 오프셋을 의미
#define SEEK_CUR 1       // 현재 파일 위치로부터의 오프셋을 의미
#define SEEK_END 2       // 파일 끝에서부터의 오프셋을 의미

char buf[BUFFER_SIZE];

int main(int argc, char *argv[])
{
    // 인자가 4개 미만이면 사용법을 출력하고 종료
    if (argc < 4) {
        printf(1, "usage: lseek_test <filename> <offset> <string>\n");
        exit();
    }

    char *fname = argv[1];   // 파일 이름
    int mv = atoi(argv[2]);  // 문자열로 전달된 offset 값을 정수로 변환
    char *str = argv[3];     // 파일에 쓸 문자열
    int fd;                  // 파일 디스크립터

    // 파일을 읽기/쓰기 모드로 염
    if ((fd = open(fname, O_RDWR)) < 0) {
        printf(1, "open error!\n");
        exit();
    }

    // 파일의 현재 내용을 출력
    int ret;
    printf(1, "Before: ");
    while ((ret = read(fd, buf, sizeof(buf))) > 0) {
        write(1, buf, ret);  // 읽은 내용을 표준 출력으로 출력
    }
    printf(1, "\n");

    // 파일 포인터를 mv(입력받은 offset) 위치로 이동
    if (lseek(fd, mv, SEEK_SET) < 0) {
        // 올바르지 않은 offset에 대한 예외처리
        printf(1, "올바르지 않은 offset, lseek failed\n");
        close(fd);
        exit();
    }

    // 입력받은 문자열을 파일에 씀
    if (write(fd, str, strlen(str)) != strlen(str)) {
        printf(1, "write failed\n");
        close(fd);
        exit();
    }

    // 파일 포인터를 다시 처음으로 이동
    if (lseek(fd, 0, SEEK_SET) < 0) {
        printf(1, "lseek failed\n");
        close(fd);
        exit();
    }

    // 파일의 변경된 내용을 다시 출력
    printf(1, "After: ");
    while ((ret = read(fd, buf, sizeof(buf))) > 0) {
        write(1, buf, ret);  // 읽은 내용을 표준 출력으로 출력
    }
    printf(1, "\n");

    close(fd);  // 파일 디스크립터 닫음
    exit();     // 프로그램 종료
}