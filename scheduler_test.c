#include "types.h"
#include "stat.h"
#include "user.h"

void run_child_process(int id) {
  printf(1, "PID: %d created\n", getpid());
  if (set_proc_info(1, 0, 0, 0, 500) < 0) {
    printf(1, "set_proc_info failed for PID: %d\n", getpid());
    exit();
  }
  printf(1, "Set process %d's info complete\n", getpid());

  // CPU를 소비하는 무한 루프
  while(1) {
    // CPU 사용 (NOPs)
    for(int i = 0; i < 1000; i++) {
      asm volatile("nop");
    }
  }
}

int main(void) {
  int i;
  int pid;
  int num_children = 1;  // 생성할 자식 프로세스 수 (최대 3개까지 늘릴 수 있음)

  printf(1, "start scheduler_test\n");

  for (i = 0; i < num_children; i++) {
    pid = fork();
    if (pid < 0) {
      printf(1, "Fork failed\n");
      exit();
    }
    if (pid == 0) {  // 자식 프로세스
      run_child_process(i);
    }
  }

  // 모든 자식 프로세스가 종료될 때까지 대기
  for (i = 0; i < num_children; i++) {
    wait();
  }

  printf(1, "end of scheduler_test\n");
  exit();
}