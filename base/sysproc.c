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

int sys_shutdown(void)
{
  /* Either of the following will work. Does not harm to put them together. */
  outw(0xB004, 0x0|0x2000); // working for old qemu
  outw(0x604, 0x0|0x2000); // working for newer qemu
  
  return 0;
}

extern int sched_trace_enabled;
extern int sched_trace_counter;
int sys_enable_sched_trace(void)
{
  if (argint(0, &sched_trace_enabled) < 0)
  {
    cprintf("enable_sched_trace() failed!\n");
  }
  
  sched_trace_counter = 0;

  return 0;
}


int childFirst;
int
sys_fork_winner(void){

    
    // passing user arg choice into kernel space via system call
    if (argint(0, &childFirst) < 0){
       return -1; // return -1 if we fail to get arg from userspace wrapper   
    }
    
   
   return 0; // better to return here than to exit to not accidentally kill process.   

}


int schedMode;        // we use externs because there is a global variable declared somewhere else but used here.
		      // was not declared within the scope of this system call but in another file
int 
sys_set_sched(void){
    
    // passing arg choice into kernel space
    if (argint(0, &schedMode) < 0){
        return -1; // return -1 if we fail to get arg from userspace wrapper
    }

    return 0;
}


int
sys_tickets_owned(void){
    
    int pidOfProcess;
    int processTickets;
    int found = 0; //did we find the process with the specified pid?
    if (argint (0, &pidOfProcess) < 0){
         return -1; // return -1 if we fail to get arg from userspace wrapper    
    }
    
    processTickets = GetTickets(pidOfProcess, &found);

    if (!found){
        cprintf("Process (%d) not found. \n", pidOfProcess);
    }

    return processTickets; // returns -1 if process wasn't found
}

int 
sys_transfer_tickets(void){
    int pidReceiver, ticketsToTransfer;

    if (argint(0, &pidReceiver) < 0){
        cprintf("failed to get the pid of the receiving process.\n");
	return -1;
    }

    if (argint(1, &ticketsToTransfer) < 0){
        cprintf("failed to get the number of tickets to transfer. \n");
	return -1;
    
    }

    if (ticketsToTransfer < 0){
        cprintf("must transfer a positive number of tickets.\n");
	return -1;
    }


    int foundReceiver = 0, receiverTickets;
    receiverTickets = GetTickets(pidReceiver, &foundReceiver);

    if (!foundReceiver){
        cprintf("Receiving process not found. \n");
	return -3;
    }

    int senderTickets = DoTransfer(pidReceiver, ticketsToTransfer); // does transfer and returns updated # of tickets in caller

    return senderTickets;
}
