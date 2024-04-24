#include <env.h>
#include <pmap.h>
#include <printk.h>
#include <trap.h>

extern void handle_int(void);
extern void handle_tlb(void);
extern void handle_sys(void);
extern void handle_mod(void);
extern void handle_reserved(void);
extern void handle_ri(void);

void (*exception_handlers[32])(void) = {
    [0 ... 31] = handle_reserved,
    [0] = handle_int,
    [2 ... 3] = handle_tlb,
    [10]=handle_ri,
#if !defined(LAB) || LAB >= 4
    [1] = handle_mod,
    [8] = handle_sys,
#endif
};

/* Overview:
 *   The fallback handler when an unknown exception code is encountered.
 *   'genex.S' wraps this function in 'handle_reserved'.
 */
void do_reserved(struct Trapframe *tf) {

    // int ExcCode = (tf->cp0_cause >> 2) & 0x1f;

	print_tf(tf);
	panic("Unknown ExcCode %2d", (tf->cp0_cause >> 2) & 0x1f);
}

void do_ri(struct Trapframe *tf) {
    u_int *epc = (u_int*) tf->cp0_epc;
    u_int order = *epc;
    if ((order & 0xff) == 0x3f) {
        // pmaxub
		u_int s=(order>>21)&0b11111;
        u_int t=(order>>16)&0b11111;
		u_int d=(order>>11)&0b11111;
        u_int rs = tf->regs[s];
        u_int rt = tf->regs[t];
        u_int rd = 0;
        for (int i = 0; i < 32; i+= 8) {
            u_int rs_i = rs & (0xff << i);
            u_int rt_i = rt & (0xff << i);
            if (rs_i < rt_i) {
                rd = rd | rt_i;
            } else {
                rd = rd | rs_i;
            }
        }
        tf->regs[d]=rd;
        tf->cp0_epc = (u_int)epc+4;
    } else if ((order & 0xff) == 0x3e) {
        // cas
		u_int s=(order>>21)&0b11111;
		u_int t=(order>>16)&0b11111;
		u_int d=(order>>11)&0b11111;
        u_int rs = tf->regs[s];
        u_int rt = tf->regs[t];
        u_int rd = tf->regs[d];
        u_int temp = *((u_int*)rs);
        if (temp == rt) {
            *((u_int*)rs) = rd;
        }
        tf->regs[d] = temp;
        tf->cp0_epc = (u_int)epc+4;
    }else {
        tf->cp0_epc = (u_int)epc+4;
    }
}
