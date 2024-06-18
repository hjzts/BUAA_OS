#include <debugk.h>
#include <env.h>
#include <lib.h>
#include <mmu.h>
#include <syscall.h>
#include <syscall_my.h>
#include <trap.h>

void syscall_putchar(int ch)
{
    msyscall(SYS_putchar, ch);
}

int syscall_print_cons(const void* str, u_int num)
{
    return msyscall(SYS_print_cons, str, num);
}

u_int syscall_getenvid(void)
{
    return msyscall(SYS_getenvid);
}

void syscall_yield(void)
{
    msyscall(SYS_yield);
}

int syscall_env_destroy(u_int envid)
{
    return msyscall(SYS_env_destroy, envid);
}

int syscall_env_destroy_with_exit_code(u_int envid)
{
    return msyscall(SYS_env_destroy_with_exit_code, envid);
}

int syscall_set_tlb_mod_entry(u_int envid, void (*func)(struct Trapframe*))
{
    return msyscall(SYS_set_tlb_mod_entry, envid, func);
}

int syscall_mem_alloc(u_int envid, void* va, u_int perm)
{
    // debugk_user("function syscall_mem_alloc is called in user/lib//syscall_lib.c");
    return msyscall(SYS_mem_alloc, envid, va, perm);
}

int syscall_mem_map(u_int srcid, void* srcva, u_int dstid, void* dstva, u_int perm)
{
    return msyscall(SYS_mem_map, srcid, srcva, dstid, dstva, perm);
}

int syscall_mem_unmap(u_int envid, void* va)
{
    return msyscall(SYS_mem_unmap, envid, va);
}

int syscall_set_env_status(u_int envid, u_int status)
{
    return msyscall(SYS_set_env_status, envid, status);
}

int syscall_set_trapframe(u_int envid, struct Trapframe* tf)
{
    return msyscall(SYS_set_trapframe, envid, tf);
}

void syscall_panic(const char* msg)
{
    int r = msyscall(SYS_panic, msg);
    user_panic("SYS_panic returned %d", r);
}

int syscall_ipc_try_send(u_int envid, u_int value, const void* srcva, u_int perm)
{
    return msyscall(SYS_ipc_try_send, envid, value, srcva, perm);
}

int syscall_ipc_recv(void* dstva)
{
    return msyscall(SYS_ipc_recv, dstva);
}

int syscall_cgetc()
{
    return msyscall(SYS_cgetc);
}

int syscall_write_dev(void* va, u_int dev, u_int size)
{
    /* Exercise 5.2: Your code here. (1/2) */
    // debugk_user("function syscall_write_dev in called in user/lib/syscall_lib.c");
    // if (!(va == 0x7f3fdfcc && dev == 0x180003fd && size == 1)) {
    //     debugk_user("syscall_write_dev va is %x, pa is %x, len is %x", va, dev, size);
    // }
    return msyscall(SYS_write_dev, va, dev, size);
}

int syscall_read_dev(void* va, u_int dev, u_int size)
{
    /* Exercise 5.2: Your code here. (2/2) */
    // debugk_user("function syscall_read_dev in called in user/lib/syscall_lib.c");
    // if (!(va == 0x7f3fdfcc && dev == 0x180003fd && size == 1)) {
    //     debugk_user("syscall_read_dev va is %x, pa is %x, len is %x", va, dev, size);
    // }
    // 这里虽然参数va是void*,但是传入msyscall的参数是u_int,类型自动转换了
    return msyscall(SYS_read_dev, va, dev, size);
}

int syscall_get_return_value(u_int envid)
{
    return msyscall(SYS_get_return_value, envid);
}
