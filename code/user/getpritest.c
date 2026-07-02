#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]){
	int pid = getpid();
	int prio = getpriority(pid);

	if(prio < 0){
		printf("Erro: getpriority falhou para %d\n", pid);
		exit(1);
	}
	printf("PID %d tem prioridade %d\n", pid, prio);
	exit(0);
}
