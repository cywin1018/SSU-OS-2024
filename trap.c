#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "x86.h"
#include "traps.h"
#include "spinlock.h"

// Interrupt descriptor table (shared by all CPUs).
struct gatedesc idt[256];
extern uint vectors[];  // in vectors.S: array of 256 entry pointers
struct spinlock tickslock;
uint ticks;
extern void print_all_queue(void); // 큐를 출력하는 함수
extern int queue_size[NQUEUE];             // 각 큐의 크기
extern struct proc* queue[NQUEUE][NPROC];  // 다단계 피드백 큐
extern void remove_from_queue(int level, struct proc *p); // 큐에서부터 삭제하는 함수

void
tvinit(void)
{
  int i;

  for(i = 0; i < 256; i++)
    SETGATE(idt[i], 0, SEG_KCODE<<3, vectors[i], 0);
  SETGATE(idt[T_SYSCALL], 1, SEG_KCODE<<3, vectors[T_SYSCALL], DPL_USER);

  initlock(&tickslock, "time");
}

void
idtinit(void)
{
  lidt(idt, sizeof(idt));
}


//PAGEBREAK: 41
void
trap(struct trapframe *tf)
{
  if(tf->trapno == T_SYSCALL){
    if(myproc()->killed)
      exit();
    myproc()->tf = tf;
    syscall();
    if(myproc()->killed)
      exit();
    return;
  }
  struct proc *other;
  switch(tf->trapno){
  case T_IRQ0 + IRQ_TIMER:
    if(cpuid() == 0){
      acquire(&tickslock);
      ticks++;
      for(int level = 0; level < NQUEUE; level++) {
          for(int i = 0; i < queue_size[level]; i++) {
              other = queue[level][i];
              if(other!=0&&other->state==SLEEPING){
                other->io_wait_time++; 
              }else if(other!=0&&other->state==RUNNABLE){
                other->cpu_wait++;
                if (other->cpu_wait >= AGING_THRESHOLD) {
                    if (other->q_level > 0) {

                      // Move the process up one queue level
                      remove_from_queue(level, other);
                      other->q_level--; // low qeueue 
                      other->cpu_wait = 0; // Reset cpu_wait
                      queue[other->q_level][queue_size[other->q_level]++] = other;
                      #ifdef DEBUG
                        cprintf("PID: %d Aging\n", other->pid); // Aging 출력함
                      #endif
                    }
                }
              }
              
          }
      }
      wakeup(&ticks);
      release(&tickslock);
    }
    lapiceoi();
    break;
  case T_IRQ0 + IRQ_IDE:
    ideintr();
    lapiceoi();
    break;
  case T_IRQ0 + IRQ_IDE+1:
    // Bochs generates spurious IDE1 interrupts.
    break;
  case T_IRQ0 + IRQ_KBD:
    kbdintr();
    lapiceoi();
    break;
  case T_IRQ0 + IRQ_COM1:
    uartintr();
    lapiceoi();
    break;
  case T_IRQ0 + 7:
  case T_IRQ0 + IRQ_SPURIOUS:
    cprintf("cpu%d: spurious interrupt at %x:%x\n",
            cpuid(), tf->cs, tf->eip);
    lapiceoi();
    break;

  //PAGEBREAK: 13
  default:
    if(myproc() == 0 || (tf->cs&3) == 0){
      // In kernel, it must be our mistake.
      cprintf("unexpected trap %d from cpu %d eip %x (cr2=0x%x)\n",
              tf->trapno, cpuid(), tf->eip, rcr2());
      panic("trap");
    }
    // In user space, assume process misbehaved.
    cprintf("pid %d %s: trap %d err %d on cpu %d "
            "eip 0x%x addr 0x%x--kill proc\n",
            myproc()->pid, myproc()->name, tf->trapno,
            tf->err, cpuid(), tf->eip, rcr2());
    myproc()->killed = 1;
  }

  // Force process exit if it has been killed and is in user space.
  // (If it is still executing in the kernel, let it keep running
  // until it gets to the regular system call return.)
  if(myproc() && myproc()->killed && (tf->cs&3) == DPL_USER)
    exit();

  // Force process to give up CPU on clock tick.
  // If interrupts were on while locks held, would need to check nlock.
// 타이머 인터럽트 처리
if(myproc() && myproc()->state == RUNNING &&
   tf->trapno == T_IRQ0+IRQ_TIMER) {
  struct proc *p = myproc();


  if(p->pid > 2 && p->end_time > 0) {
    p->cpu_burst++;
    p->remaining_time--;

    int quantum = get_time_quantum(p->q_level);

    // // Reset cpu_wait since the process is running
    //  p->cpu_wait = 0;
    //  p->io_wait_time = 0;  // 실행 중인 프로세스의 io_wait_time 리셋
    if(p->remaining_time <= 0) {
      // 프로세스 종료 전에 정보 출력
       
       #ifdef DEBUG
              int total_used = p->end_time - p->remaining_time;
              cprintf("PID: %d uses %d ticks in mlfq[%d], total(%d/%d)\n",
                      p->pid, p->cpu_burst, p->q_level, total_used, p->end_time);
       #endif

      p->killed = 1;  // 프로세스 종료 설정
      // print_all_queue();
    } else if(p->cpu_burst >= quantum) {
      // Time Quantum 만료 시 정보 출력
       
       #ifdef DEBUG
              int total_used = p->end_time - p->remaining_time;
              cprintf("PID: %d uses %d ticks in mlfq[%d], total(%d/%d)\n",
                      p->pid, p->cpu_burst, p->q_level, total_used, p->end_time);
       #endif
    
      // p->cpu_burst = 0;  // CPU burst 초기화
     
      yield();
    }
    // p->cpu_wait = 0;
  }
    
}


  // Check if the process has been killed since we yielded
  if(myproc() && myproc()->killed && (tf->cs&3) == DPL_USER)
    exit();
}
