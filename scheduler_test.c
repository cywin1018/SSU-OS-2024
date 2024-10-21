#include "types.h"
#include "stat.h"
#include "user.h"

void run_child_process(int id) {
  printf(1, "PID: %d created\n", getpid());
  if (set_proc_info(0, 0, 0, 0, 500) < 0) {
    printf(1, "set_proc_info failed for PID: %d\n", getpid());
    exit();
  }
  printf(1, "Set process %d's info complete\n", getpid());

  // 프로세스가 종료될 때까지 대기
  while(1) {
    if (set_proc_info(0, 0, 0, 0, -1) < 0) {
      // 프로세스가 종료되었다면 set_proc_info가 실패할 것임
      break;
    }
    sleep(1);  // CPU 시간을 다른 프로세스에게 양보
  }

  exit();
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