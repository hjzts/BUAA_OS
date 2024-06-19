#include <env.h>
#include <lib.h>
#include <mmu.h>

#ifdef RETURN_VALUE
#define IPC
int exit_code;
#ifdef IPC
// use ipc
void exit(void)
{
    // After fs is ready (lab5), all our open files should be closed before dying.
#if !defined(LAB) || LAB >= 5
    close_all();
#endif
    int parent = env->env_parent_id;
    if (!(parent == env->env_id || parent == 0)) {
        ipc_send(parent, exit_code, 0, 0);
    }
    syscall_env_destroy(0);
    user_panic("unreachable code");
}
#else
// use struct Env
int exit_with_exit_code(void)
{
    // After fs is ready (lab5), all our open files should be closed before dying.
#if !defined(LAB) || LAB >= 5
    close_all();
#endif
    return syscall_env_destroy_with_exit_code(0);
    user_panic("unreachable code");
}
#endif
#else
void exit(void)
{
    // After fs is ready (lab5), all our open files should be closed before dying.
#if !defined(LAB) || LAB >= 5
    close_all();
#endif
    syscall_env_destroy(0);
    user_panic("unreachable code");
}
#endif

const volatile struct Env* env;
extern int main(int, char**);

void libmain(int argc, char** argv)
{
    // set env to point at our env structure in envs[].
    env = &envs[ENVX(syscall_getenvid())];

    // call user main routine

#ifdef RETURN_VALUE
    exit_code = main(argc, argv);
#else
    main(argc, argv);
#endif
    // int parent = env->env_parent_id;
    // ipc_send(parent, exit_code, 0, 0);

    // debugk_user("the return value is %d", exit_code);
    // exit gracefully 优雅？？？
    exit();
}
