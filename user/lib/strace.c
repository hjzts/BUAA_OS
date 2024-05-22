#include <lib.h>

void strace_barrier(u_int env_id) {
	int straced_bak = straced;
	straced = 0;
	while (envs[ENVX(env_id)].env_status == ENV_RUNNABLE) {
		syscall_yield();
	}
	straced = straced_bak;
}

void strace_send(int sysno) {
	if (!((SYS_putchar <= sysno && sysno <= SYS_set_tlb_mod_entry) ||
	      (SYS_exofork <= sysno && sysno <= SYS_panic)) ||
	    sysno == SYS_set_trapframe) {
		return;
	}
	
	// Your code here. (1/2)
	if (straced == 0) return;
	int straced_bak=straced;
	straced = 0;
	int parent = 0;
	// int child_env_id = strace_recv_child[strace_recv_cnt];
	int child_env_id = env->env_id;
	ipc_send(env->env_parent_id, sysno,0 ,0);
	syscall_set_env_status(child_env_id, ENV_NOT_RUNNABLE);
	straced = straced_bak;
}

void strace_recv() {
	// Your code here. (2/2)
	int child_env_id = strace_recv_child[strace_recv_cnt];
	u_int whom;
	int r = 0;
	do {
		r = ipc_recv(&whom, 0 ,0);
		child_env_id = whom;
		strace_barrier(child_env_id);
		//int child_env_id = strace_recv_child[strace_recv_cnt];
		recv_sysno(child_env_id, r);
		syscall_set_env_status(child_env_id, ENV_RUNNABLE);
		if(r == SYS_env_destroy) return;
	} while(1);
}
