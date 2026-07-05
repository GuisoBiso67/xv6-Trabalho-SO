// user/agingtest.c
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define NUM_CHILDREN 8
#define BASE_ITERS   20000000

int
itoa(int n, char *buf)
{
  char tmp[16];
  int i = 0, j = 0;
  int neg = n < 0;
  if (neg) n = -n;

  if (n == 0) tmp[i++] = '0';
  while (n > 0) {
    tmp[i++] = '0' + (n % 10);
    n /= 10;
  }
  if (neg) tmp[i++] = '-';

  while (i > 0) buf[j++] = tmp[--i];
  return j;
}

int
strcopy(char *dst, const char *src)
{
  int i = 0;
  while (src[i]) {
    dst[i] = src[i];
    i++;
  }
  return i;
}

void
print_result(int idx, int start, int end, int prio)
{
  char buf[128];
  int len = 0;

  len += strcopy(buf + len, "filho[");
  len += itoa(idx, buf + len);
  len += strcopy(buf + len, "] pid=");
  len += itoa(getpid(), buf + len);
  len += strcopy(buf + len, " start=");
  len += itoa(start, buf + len);
  len += strcopy(buf + len, " end=");
  len += itoa(end, buf + len);
  len += strcopy(buf + len, " duracao=");
  len += itoa(end - start, buf + len);
  len += strcopy(buf + len, " prioridade_final=");
  len += itoa(prio, buf + len);
  len += strcopy(buf + len, "\n");

  write(1, buf, len);
}

void
busy_work(int idx, long iters)
{
	int start = uptime();

	volatile long dummy = 0;
	for(long i = 0; i < iters; i++){
		dummy += 1;
	}

	int end = uptime();
	int prio = getpriority(getpid());
	print_result(idx, start, end, prio);
}

int
main(void)
{
  write(1, "=== iniciando teste de aging com 8 processos ===\n", 50);

  for (int i = 0; i < NUM_CHILDREN; i++) {
    int pid = fork();
    if (pid == 0) {
      long work = (i%2 == 0) ? BASE_ITERS * 2 : BASE_ITERS;
      busy_work(i, work);
      exit(0);
    }
  }

  for (int i = 0; i < NUM_CHILDREN; i++) {
    wait(0);
  }

  write(1, "=== teste finalizado ===\n", 26);
  exit(0);
}
