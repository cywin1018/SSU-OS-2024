#include "types.h"   // 기본 자료형을 정의하는 헤더 파일
#include "stat.h"    // 파일 상태를 위한 헤더 파일
#include "user.h"    // 사용자 프로그램이 커널과 상호작용할 수 있게 하는 함수들이 선언된 헤더 파일

int main(int argc, char **argv) {
    // "Hello xv6 World!" 메시지를 출력 (첫 번째 인수로 파일 디스크립터 1 = 표준 출력)
    printf(1, "Hello xv6 World!\n");

    exit();
}