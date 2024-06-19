#include <env.h>
#include <lib.h>
// cpu 忙等

#ifdef RETURN_VALUE
int wait(u_int envid)
{
    debugk_user("the envid is %x,  function wait() is called in user/lib/wait.c", syscall_getenvid());
    const volatile struct Env* e;

    e = &envs[ENVX(envid)];
    while (e->env_id == envid && e->env_status != ENV_FREE) {
        syscall_yield();
    }
    int r;
    syscall_get_exit_code(0, &r);
    // debugk_user("IN function wait() in user/lib/wait.c, the return value after env %x wait env %x is %d", syscall_getenvid(), envid, r);
    debugk_user("IN function wait() in user/lib/wait.c, the return value after wait env %x is %d", envid, r);
    return r;
}
#else
void wait(u_int envid)
{
    debugk_user("function wait() is called in user/lib/wait.c");
    const volatile struct Env* e;

    e = &envs[ENVX(envid)];
    while (e->env_id == envid && e->env_status != ENV_FREE) {
        syscall_yield();
    }
}
#endif
