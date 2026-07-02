#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define NUM_CHILDREN 4
#define BUSY_ITERS   50000000   // ajuste conforme velocidade do seu QEMU
#define PRINT_EVERY  5000000    // a cada quantas iterações imprime status

void
busy_work(int initial_priority)
{
  int last_uptime = -1;

  for (int i = 0; i < BUSY_ITERS; i++) {
    if (i % PRINT_EVERY == 0) {
      int now = uptime();
      if (now != last_uptime) {
        int prio = getpriority(getpid());
        printf("pid=%d prioridade_inicial=%d prioridade_atual=%d tick=%d iter=%d\n",
               getpid(), initial_priority, prio, now, i);
        last_uptime = now;
      }
    }
  }
}

int
main(void)
{
  //int priorities[NUM_CHILDREN] = {0, 3, 6, 9}; // menor = mais prioritário
  int pids[NUM_CHILDREN];

  printf("=== iniciando teste de aging com %d processos ===\n", NUM_CHILDREN);

  for (int i = 0; i < NUM_CHILDREN; i++) {
    int pid = fork();

    if (pid == 0) {
      // processo filho
      busy_work(10);
      exit(0);
    } else {
      pids[i] = pid;
    }
  }

  // pai espera todos os filhos terminarem
  for (int i = 0; i < NUM_CHILDREN; i++) {
    wait(0);
  }

  printf("=== teste finalizado. PIDs criados:  ===\n");
  for(int i=0; i< NUM_CHILDREN; i++){
  	printf("%d ", pids[i]);
  }
  printf("===\n");
  exit(0);
}
