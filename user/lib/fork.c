#include <debugk.h>
#include <env.h>
#include <lib.h>
#include <mmu.h>

/* Overview:
 *   Map the faulting page to a private writable copy.
 *
 * Pre-Condition:
 * 	'va' is the address which led to the TLB Mod exception.
 *
 * Post-Condition:
 *  - Launch a 'user_panic' if 'va' is not a copy-on-write page.
 *  - Otherwise, this handler should map a private writable copy of
 *    the faulting page at the same address.
 */
static void __attribute__((noreturn)) cow_entry(struct Trapframe* tf)
{
    u_int va = tf->cp0_badvaddr;
    u_int perm;

    /* Step 1: Find the 'perm' in which the faulting address 'va' is mapped. */
    /* Hint: Use 'vpt' and 'VPN' to find the page table entry. If the 'perm' doesn't have
     * 'PTE_COW', launch a 'user_panic'. */
    /* Exercise 4.13: Your code here. (1/6) */
    debugk_user("static function cow_entry is called");

    perm = vpt[VPN(va)];
    if (!(perm & PTE_COW))
        user_panic("User page falut face a not COW page");

    /* Step 2: Remove 'PTE_COW' from the 'perm', and add 'PTE_D' to it. */
    /* Exercise 4.13: Your code here. (2/6) */
    perm = (perm & ~PTE_COW) | PTE_D;
    debugk_user("perm has remove PTE_COE and add PTE_D in function cow_enty");
    /* Step 3: Allocate a new page at 'UCOW'. */
    /* Exercise 4.13: Your code here. (3/6) */
    try(syscall_mem_alloc(0, (void*)UCOW, perm));
    // try(syscall_mem_alloc(0, (void*)UCOW, PTE_V | PTE_D));

    /* Step 4: Copy the content of the faulting page at 'va' to 'UCOW'. */
    /* Hint: 'va' may not be aligned to a page! */
    /* Exercise 4.13: Your code here. (4/6) */
    va = ROUNDDOWN(va, PAGE_SIZE);
    memcpy((void*)UCOW, (void*)va, PAGE_SIZE);

    // Step 5: Map the page at 'UCOW' to 'va' with the new 'perm'.
    /* Exercise 4.13: Your code here. (5/6) */
    syscall_mem_map(0, (void*)UCOW, 0, (void*)va, perm);

    // Step 6: Unmap the page at 'UCOW'.
    /* Exercise 4.13: Your code here. (6/6) */
    syscall_mem_unmap(0, (void*)UCOW);

    // Step 7: Return to the faulting routine.
    int r = syscall_set_trapframe(0, tf);
    user_panic("syscall_set_trapframe returned %d", r);
}

/* Overview:
 *   Grant授予 our child 'envid' access to the virtual page 'vpn' (with address 'vpn' * 'PAGE_SIZE') in
 * our (current env's) address space. 'PTE_COW' should be used to isolate隔离 the modifications修改 on
 * unshared memory from a parent and its children.
 *
 * Post-Condition:
 *  	可写页面：
 *   If the virtual page 'vpn' has 'PTE_D' and doesn't has 'PTE_LIBRARY', both our original virtual
 *   page and 'envid''s newly-mapped virtual page should be marked 'PTE_COW' and without 'PTE_D',
 *   while the other permission bits are kept.
 *
 *   If not, the newly-mapped virtual page in 'envid' should have the exact same permission as our
 *   original virtual page.
 *
 * Hint:
 *   - 'PTE_LIBRARY'共享页面 indicates that the page should be shared among a parent and its children.
 *   - A page with 'PTE_LIBRARY' may have 'PTE_D' at the same time, you should handle it correctly.
 *   - You can pass '0' as an 'envid' in arguments of 'syscall_*' to indicate指示 current env because
 *     kernel 'envid2env' converts '0' to 'curenv').
 *   - You should use 'syscall_mem_map', the user space wrapper around 'msyscall' to invoke
 *     'sys_mem_map' in kernel.
 */
static void duppage(u_int envid, u_int vpn)
{
    int r;
    u_int addr;
    u_int perm;

    /* Step 1: Get the permission of the page. */
    /* Hint: Use 'vpt' to find the page table entry. */
    /* Exercise 4.10: Your code here. (1/2) */
    perm = vpt[vpn] & 0xfff;
    addr = vpn << PGSHIFT;

    /* Step 2: If the page is writable, and not shared with children, and not marked as COW yet,
     * then map it as copy-on-write, both in the parent (0) and the child (envid). */
    /* Hint: The page should be first mapped to the child before remapped in the parent. (Why?)
     */
    // 可能是子进程不会被再次调度(还是NOT_RUNNABLE)，但是父进程可能会被中断？
    /* Exercise 4.10: Your code here. (2/2) */
    // 这些页面应该是用户空间，也就是父进程的页面
    // 在用户空间，他的id就是0
    if ((perm & PTE_D) && !(perm & PTE_LIBRARY) && !(perm & PTE_COW)) {
        // 可写入	不是共享页面	不是写时复制页面
        perm = (perm & ~PTE_D) | PTE_COW; // 	加入写时复制保护
        syscall_mem_map(0, (void*)addr, envid, (void*)addr, perm); // 	先复制给子进程
        syscall_mem_map(0, (void*)addr, 0, (void*)addr, perm); // 	然后复制给父进程
    } else {
        // 不可写页面	共享页面
        syscall_mem_map(0, (void*)addr, envid, (void*)addr, perm); //	其他的该是什么权限就是什么权限
    }
}

/* Overview:
 *   User-level 'fork'. Create a child and then copy our address space.
 *   Set up ours and its TLB Mod user exception entry to 'cow_entry'.
 *
 * Post-Conditon:
 *   Child's 'env' is properly set.
 *
 * Hint:
 *   Use global symbols 'env', 'vpt' and 'vpd'.
 *   Use 'syscall_set_tlb_mod_entry', 'syscall_getenvid', 'syscall_exofork',  and 'duppage'.
 */
int fork(void)
{
    u_int child;
    u_int i;

    /* Step 1: Set our TLB Mod user exception entry to 'cow_entry' if not done yet. */
    if (env->env_user_tlb_mod_entry != (u_int)cow_entry) {
        try(syscall_set_tlb_mod_entry(0, cow_entry));
    }

    /* Step 2: Create a child env that's not ready to be scheduled. */
    // Hint: 'env' should always point to the current env itself, so we should fix it to the
    // correct value.
    child = syscall_exofork();
    if (child == 0) {
        env = envs + ENVX(syscall_getenvid());
        return 0;
    }

    /* Step 3: Map all mapped pages below 'USTACKTOP' into the child's address space. */
    // Hint: You should use 'duppage'.
    /* Exercise 4.15: Your code here. (1/2) */
    for (int i = 0; i < USTACKTOP; i += PAGE_SIZE) {
        Pde* pde = &vpd[i >> PDSHIFT];
        Pte* pte = &vpt[i >> PGSHIFT];

        if (((*pde) & PTE_V) && ((*pte) & PTE_V)) {
            duppage(child, VPN(i));
        }
    }
    // 另一种写法，一样的，相当于是一页一页来
    /*for (i = 0; i < VPN(USTACKTOP); i++) {
        if ((vpd[i >> 10] & PTE_V) && (vpt[i] & PTE_V)) {
            duppage(child, i);
        }
    }*/
    /* Step 4: Set up the child's tlb mod handler and set child's 'env_status' to
     * 'ENV_RUNNABLE'. */
    /* Hint:
     *   You may use 'syscall_set_tlb_mod_entry' and 'syscall_set_env_status'
     *   Child's TLB Mod user exception entry should handle COW, so set it to 'cow_entry'
     */
    /* Exercise 4.15: Your code here. (2/2) */
    try(syscall_set_tlb_mod_entry(child, cow_entry));
    try(syscall_set_env_status(child, ENV_RUNNABLE));
    return child;
}
