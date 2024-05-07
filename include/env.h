#ifndef _ENV_H_
#define _ENV_H_

#include <debugk.h>
#include <mmu.h>
#include <queue.h>
#include <trap.h>
#include <types.h>

#define LOG2NENV 10
#define NENV (1 << LOG2NENV)
#define ENVX(envid) ((envid) & (NENV - 1))

// All possible values of 'env_status' in 'struct Env'.
#define ENV_FREE 0
#define ENV_RUNNABLE 1
#define ENV_NOT_RUNNABLE 2

// Control block of an environment (process).
struct Env {
    struct Trapframe env_tf; // saved context (registers) before switching
    // 在发生进程调度或者陷入内核时，会将当时的进程上下文环境保存在这里
    LIST_ENTRY(Env) env_link; 
                    // intrusive entry in 'env_free_list'
    u_int env_id; // unique environment identifier
    u_int env_asid; // ASID of this env
    u_int env_parent_id; // env_id of this env's parent
    u_int env_status; // status of this env
    // ENV_FREE: 表明该进程控制块没有被任何进程使用，即改进程控制块处理进程空闲链表中
    // ENV_NOT_RUNNABLE: 进程处于阻塞状态
    // ENV_RUNNABLE :进程处于执行状态或者就绪状态
    Pde* env_pgdir; // page directory
                    // 进程页目录的内核虚拟地址
    TAILQ_ENTRY(Env) env_sched_link; 
                    // intrusive entry in 'env_sched_list'
                    // 用来构造调度队列
    u_int env_pri; // schedule priority

    // Lab 4 IPC
    u_int env_ipc_value; // the value sent to us
    u_int env_ipc_from; // envid of the sender
    u_int env_ipc_recving; // whether this env is blocked receiving
    u_int env_ipc_dstva; // va at which the received page should be mapped
    u_int env_ipc_perm; // perm in which the received page should be mapped

    // Lab 4 fault handling
    u_int env_user_tlb_mod_entry; // userspace TLB Mod handler

    // Lab 6 scheduler counts
    u_int env_runs; // number of times we've been env_run'ed
};

LIST_HEAD(Env_list, Env);
TAILQ_HEAD(Env_sched_list, Env);
extern struct Env* curenv; // the current env
extern struct Env_sched_list env_sched_list; // runnable env list

void env_init(void);
int env_alloc(struct Env** e, u_int parent_id);
void env_free(struct Env*);
struct Env* env_create(const void* binary, size_t size, int priority);
void env_destroy(struct Env* e);

int envid2env(u_int envid, struct Env** penv, int checkperm);
void env_run(struct Env* e) __attribute__((noreturn));

void env_check(void);
void envid2env_check(void);

#define ENV_CREATE_PRIORITY(x, y)                                    \
    ({                                                               \
        extern u_char binary_##x##_start[];                          \
        extern u_int binary_##x##_size;                              \
        env_create(binary_##x##_start, (u_int)binary_##x##_size, y); \
    })

#define ENV_CREATE(x)                                                \
    ({                                                               \
        extern u_char binary_##x##_start[];                          \
        extern u_int binary_##x##_size;                              \
        debugk("begin env_create");                                  \
        env_create(binary_##x##_start, (u_int)binary_##x##_size, 1); \
    })

#endif // !_ENV_H_
