#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[]) {
    printf(1, "start scheduler_test\n");
    
    int pid;
    
    // 첫 번째 프로세스 생성
    pid = fork();
    if(pid < 0) {
        printf(1, "fork failed\n");
        exit();
    }
    if(pid == 0) {  // 첫 번째 자식 프로세스
        printf(1, "PID: %d created\n", getpid());
        sleep(50); // 다른 프로세스들이 생성될 때까지 대기
        sleep(100); // 추가 대기 시간
        
        if(set_proc_info(2, 0, 0, 0, 300) < 0) {
            printf(1, "set_proc_info failed\n");
            exit();
        }
        printf(1, "Set process %d's info complete\n", getpid());
        while(1);
    }

    // 두 번째 프로세스 생성
    pid = fork();
    if(pid < 0) {
        printf(1, "fork failed\n");
        exit();
    }
    if(pid == 0) {  // 두 번째 자식 프로세스
        printf(1, "PID: %d created\n", getpid());
        sleep(50);
        sleep(50); // 중간 대기 시간
        
        if(set_proc_info(2, 0, 0, 0, 300) < 0) {
            printf(1, "set_proc_info failed\n");
            exit();
        }
        printf(1, "Set process %d's info complete\n", getpid());
        while(1);
    }

    // 세 번째 프로세스 생성
    pid = fork();
    if(pid < 0) {
        printf(1, "fork failed\n");
        exit();
    }
    if(pid == 0) {  // 세 번째 자식 프로세스
        printf(1, "PID: %d created\n", getpid());
        sleep(50);
        
        if(set_proc_info(2, 0, 0, 0, 300) < 0) {
            printf(1, "set_proc_info failed\n");
            exit();
        }
        printf(1, "Set process %d's info complete\n", getpid());
        while(1);
    }

    // 부모 프로세스는 모든 자식 프로세스가 종료될 때까지 대기
    for(int i = 0; i < 3; i++) {
        wait();
    }

    printf(1, "end of scheduler_test\n");
    exit();
}