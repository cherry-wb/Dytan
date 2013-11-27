#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <signal.h>
#include <syscall.h>
#include <sys/ptrace.h>
#include <asm/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <sys/user.h>
#include <getopt.h>

#include "itrace.h"

pid_t pid;

uint32_t peek_register(uint32_t reg) {
	return (uint32_t) ptrace(PTRACE_PEEKUSER, pid, sizeof(long int) * reg, NULL);
}

void peek_memory(uint8_t* buf, size_t cnt, off_t addr) {
    uint8_t *p;
    int i, j;
    union {
            long val;
            uint8_t chars[sizeof(long)];
    } data;

    p = buf;

    j = cnt / sizeof(long);
    for(i = 0; i < j; i++, p += sizeof(long)) {
        data.val = ptrace(PTRACE_PEEKDATA, pid, addr + i * sizeof(long), NULL);
        memcpy(p, data.chars, sizeof(long));
    }

    j = cnt % sizeof(long);
    if(j != 0) {
        data.val = ptrace(PTRACE_PEEKDATA, pid, addr + i * sizeof(long), NULL);
        memcpy(p, data.chars, j);
    }    
}

int main(int argc, char *argv[])
{
        int status, c;

	while((c = getopt (argc, argv, "abc:")) != -1) {
		switch(c) {
		}	
	}

	int cnt = argc -(optind);
	int i;
	char *args[cnt + 1];
	for(i = 0; i < cnt; i++) {
		args[i] = argv[optind+i];
	}
	args[cnt] = NULL;

	itrace_t *trace = itrace_init();
	itrace_set_peek_mem_func(trace, peek_memory);
	itrace_set_peek_reg_func(trace, peek_register);

        switch(pid = fork()) {
	        case -1:
	                perror("fork");
	                break;

	        case 0:
	                ptrace(PTRACE_TRACEME, 0, 0, 0);
	                execv(args[0], args);
			printf("error\n");
	                break;

	        default:
	                wait(&status); 
	                while(SIGTRAP == WSTOPSIG(status)) {
				struct user_regs_struct regs;

				ptrace(PTRACE_GETREGS, pid, NULL, &regs);
				itrace_trace(trace, regs.eip);
	                        if (ptrace(PTRACE_SINGLESTEP, pid, 0, 0) != 0) perror("ptrace");
	                        wait(&status);
	               }
        }
        return EXIT_SUCCESS;
}

