#include <env.h>
#include <lib.h>
#include <mmu.h>

int return_value;
void exit_and_send(void)
{
    // After fs is ready (lab5), all our open files should be closed before dying.
#if !defined(LAB) || LAB >= 5
    close_all();
#endif
    int parent = env->env_parent_id;
    if (!(parent == env->env_id || parent == 0)) {
        ipc_send(parent, return_value, 0, 0);
    }
    syscall_env_destroy(0);
    user_panic("unreachable code");
}

int exit_with_exit_code(void)
{
    // After fs is ready (lab5), all our open files should be closed before dying.
#if !defined(LAB) || LAB >= 5
    close_all();
#endif
    return syscall_env_destroy_with_exit_code(0);
    user_panic("unreachable code");
}

void exit(void)
{
    // After fs is ready (lab5), all our open files should be closed before dying.
#if !defined(LAB) || LAB >= 5
    close_all();
#endif
    syscall_env_destroy(0);
    user_panic("unreachable code");
}

const volatile struct Env* env;
extern int main(int, char**);

void libmain(int argc, char** argv)
{
    // set env to point at our env structure in envs[].
    env = &envs[ENVX(syscall_getenvid())];

    // call user main routine

    // main(argc, argv);

    return_value = main(argc, argv);
    // int parent = env->env_parent_id;
    // ipc_send(parent, return_value, 0, 0);

    // debugk_user("the return value is %d", return_value);
    // exit gracefully 优雅？？？
    exit();
}
