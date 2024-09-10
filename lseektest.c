#include "user.h"
#include "fcntl.h"
#define BUFFER_SIZE 50
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

char buf[BUFFER_SIZE];

int main(int argc, char *argv[])
{
    if (argc < 4) {
        printf(1, "usage: lseek_test <filename> <offset> <string>\n");
        exit();
    }

    char *fname = argv[1];
    int mv = atoi(argv[2]);
    char *str = argv[3];
    int fd;
    
    // 파일을 읽고 쓰기 모드로 엶
    if ((fd = open(fname, O_RDWR)) < 0) {
        printf(1, "open error!\n");
        exit();
    }

    // 파일 처음 내용을 출력
    int ret;
    printf(1, "Before: ");
    while ((ret = read(fd, buf, sizeof(buf))) > 0) {
        write(1, buf, ret);
    }
    printf(1, "\n");

    // 파일 포인터를 mv 위치로 이동
    if (lseek(fd, mv, SEEK_SET) < 0) {
        printf(1, "lseek failed\n");
        close(fd);
        exit();
    }

    // 문자열을 파일에 작성
    if (write(fd, str, strlen(str)) != strlen(str)) {
        printf(1, "write failed\n");
        close(fd);
        exit();
    }

    // 파일 포인터를 처음으로 다시 이동
    if (lseek(fd, 0, SEEK_SET) < 0) {
        printf(1, "lseek failed\n");
        close(fd);
        exit();
    }

    // 파일 내용을 다시 출력
    printf(1, "After: ");
    while ((ret = read(fd, buf, sizeof(buf))) > 0) {
        write(1, buf, ret);
    }
    printf(1, "\n");

    close(fd);
    exit();
}
