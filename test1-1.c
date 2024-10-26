#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  printf(1, "start scheduler_test\n");

  int pid = fork();
  if(pid < 0) {
    printf(1, "fork failed\n");
    exit();
  }
  
  if(pid == 0) {
    // 자식 프로세스
    printf(1, "PID: %d created\n", getpid());
    if(set_proc_info(0, 0, 0, 0, 500) < 0) {  // 반드시 레벨 0에서 시작
      printf(1, "set_proc_info failed\n");
      exit();
    }
    printf(1, "Set process %d's info complete\n", getpid());
    
    while(1); // 무한 루프를 돌며 CPU 사용
  } else {
    wait(); // 자식 프로세스가 종료될 때까지 대기
    printf(1, "end of scheduler_test\n");
  }
  
  exit();
}