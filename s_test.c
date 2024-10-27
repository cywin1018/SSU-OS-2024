#include "types.h"
#include "stat.h"
#include "user.h"

#define NUM_PROCESSES 30  // 프로세스 개수를 10개로 정의

int main(int argc, char *argv[])
{
  printf(1, "start scheduler_test\n");

  #ifdef DEBUG
  int start_time = uptime();  // 전체 시작 시간 기록
  int created_processes = 0;   // 생성된 프로세스 수 추적

  for(int i = 0; i < NUM_PROCESSES; i++) {
    int pid = fork();
    if(pid < 0) {
      printf(1, "fork failed\n");
      exit();
    }
    if(pid == 0) {
      // 자식 프로세스
    
      sleep(10);  // 프로세스 생성 간격을 두어 출력이 섞이지 않도록 함
      
      if(set_proc_info(1, 0, 0, 0, 500) < 0) {  
       
        exit();
      }
      
      
      while(1);  // CPU 사용
    } else {
      created_processes++;
      sleep(5);  // 부모 프로세스도 약간의 간격을 두고 자식 프로세스 생성
    }
  }

  // 모든 자식 프로세스 종료 대기
  for(int i = 0; i < NUM_PROCESSES; i++) {
    wait();
  }

  // 성능 측정 결과 출력
  int total_runtime = uptime() - start_time;
  printf(1, "\n=== SSU Performance Summary ===\n");
  printf(1, "Number of processes: %d\n", NUM_PROCESSES);
  printf(1, "Total runtime: %d ticks\n", total_runtime);
  printf(1, "Average time per process: %d ticks\n", total_runtime / NUM_PROCESSES);
  printf(1, "========================\n\n");

  printf(1, "end of scheduler_test\n");
  exit();
  #endif
  
  printf(1, "end of scheduler_test\n"); 
  exit();
}