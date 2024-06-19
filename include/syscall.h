#include <debugk.h>
#ifndef SYSCALL_H
#define SYSCALL_H

#ifndef __ASSEMBLER__

enum {
    SYS_putchar,
    SYS_print_cons,
    SYS_getenvid,
    SYS_get_parent_envid,
    SYS_yield,
    SYS_env_destroy,
    SYS_set_tlb_mod_entry,
    SYS_mem_alloc,
    SYS_mem_map,
    SYS_mem_unmap,
    SYS_exofork,
    SYS_set_env_status,
    SYS_set_trapframe,
    SYS_panic,
    SYS_ipc_try_send,
    SYS_ipc_recv,
    SYS_cgetc,
    SYS_write_dev,
    SYS_read_dev,
#ifdef RETURN_VALUE
#ifndef IPC
    SYS_set_exit_code,
    SYS_get_exit_code,
#endif
    SYS_get_return_value,
#endif
    MAX_SYSNO,
};

#endif

#endif
