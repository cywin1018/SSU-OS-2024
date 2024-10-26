#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[])
{
  printf(1, "start scheduler_test\n");

  for(int i = 0; i < 3; i++) {
    int pid = fork();
    if(pid < 0) {
      printf(1, "fork failed\n");
      exit();
    }
    if(pid == 0) {
      // Child process
      printf(1, "PID: %d created\n", getpid());
      sleep(10);
      if(set_proc_info(1, 0, 0, 0, 500) < 0) {  
        printf(1, "set_proc_info failed\n");
        exit();
      }
      printf(1, "Set process %d's info complete\n", getpid());
      while(1);
    }
  }

  for(int i = 0; i < 3; i++) {
    wait();
  }

  printf(1, "end of scheduler_test\n");
  exit();
}
