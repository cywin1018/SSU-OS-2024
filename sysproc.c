#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

/*과제2번 set_proc_info함수 추가*/
int
sys_set_proc_info(void)
{
  int q_level, cpu_burst, cpu_wait, io_wait_time, end_time;

  // 인자 받기용
  if(argint(0,&q_level) < 0 || argint(1, &cpu_burst) < 0 ||
    argint(2, &cpu_wait) < 0 || argint(3, &io_wait_time) < 0 ||
    argint(4, &end_time) < 0)
  return -1;

  struct proc *p = myproc(); // 현재 프로세스

  // 값을 아래에서 설정 해줌
  p->q_level = q_level;
  p->cpu_burst = cpu_burst;
  p->cpu_wait = cpu_wait;
  p->io_wait_time = io_wait_time;
  p->end_time = end_time;

  return 0;
}
