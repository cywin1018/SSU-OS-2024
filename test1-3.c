#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[])
{
  // 스케줄러 테스트 시작 메시지 출력
  printf(1, "start scheduler_test\n");

  #ifdef DEBUG
  // 프로세스를 3개 생성하기 위한 루프
  for(int i = 0; i < 3; i++) {
    int pid = fork();  // 자식 프로세스를 생성
    if(pid != 0) {     // 부모 프로세스에서 자식 PID 출력
      printf(1, "PID: %d created\n", pid);
    }
    if(pid == 0) {     // 자식 프로세스의 경우
      // 자식 프로세스의 스케줄러 관련 정보를 설정
      if(set_proc_info(2, 0, 0, 0, 300) < 0) {  
        printf(1, "set_proc_info failed\n");  // 설정 실패 시 오류 메시지 출력 후 종료
        exit();
      }
      printf(1, "Set process %d's info complete\n", getpid());  // 설정 완료 메시지 출력

      // 자식 프로세스를 무한 루프 상태로 대기
      while(1);
    }
  }

  // 생성된 자식 프로세스 3개가 모두 종료될 때까지 부모 프로세스에서 대기
  for(int i = 0; i < 3; i++) {
    wait();
  }

  // 스케줄러 테스트 종료 메시지 출력
  printf(1, "end of scheduler_test\n");
  exit();
  #endif

  // DEBUG가 정의되지 않은 경우 바로 종료 메시지 출력
  printf(1, "end of scheduler_test\n"); 
  exit();
}
