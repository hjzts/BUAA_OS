#include <env.h>
#include <lib.h>
// cpu 忙等

#ifdef RETURN_VALUE
int wait(u_int envid)
{
    const volatile struct Env* e;

    e = &envs[ENVX(envid)];
    int r = ipc_recv(&envid, 0, 0);
    while (e->env_id == envid && e->env_status != ENV_FREE) {
        syscall_yield();
    }
    debugk_user("the return value after wait is %d", r);
    return r;
}
#else
void wait(u_int envid)
{
    const volatile struct Env* e;

    e = &envs[ENVX(envid)];
    while (e->env_id == envid && e->env_status != ENV_FREE) {
        syscall_yield();
    }

}
#endif
