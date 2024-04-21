#include <asm/asm.h>

LEAF(tlb_out)								# 叶子函数
.set noreorder								# 不雅重新排列指令顺序
	mfc0    t0, CP0_ENTRYHI					# 从CP0寄存器CP0_ENTRYHI中的值读取到t0种
	mtc0    a0, CP0_ENTRYHI					# 将a0的值保存到CP0寄存器CP0_ENTRYHI中
	nop
	/* Step 1: Use 'tlbp' to probe TLB entry */
	/* Exercise 2.8: Your code here. (1/2) */
	tlbp									# 查询虚拟地址的物理地址映射
	nop
	/* Step 2: Fetch the probe result from CP0.Index */
	mfc0    t1, CP0_INDEX					# 将CP0寄存器CP0_INDEX寄存器中的值读取到t1中
.set reorder								# 指示汇编器重新排列指令顺序
	bltz    t1, NO_SUCH_ENTRY				# 跳转，如果t1小于0,则跳转到NO_SUCH_ENTRY处
.set noreorder
	mtc0    zero, CP0_ENTRYHI				# 将CP0寄存器CP0_ENTRYHI清空
	mtc0    zero, CP0_ENTRYLO0				# 将CP0寄存器CP0_ENTRYLO0清空
	mtc0    zero, CP0_ENTRYLO1				# 将CP0寄存器CP0_ENTRYLO1清空
	nop
	/* Step 3: Use 'tlbwi' to write CP0.EntryHi/Lo into TLB at CP0.Index  */
	/* Exercise 2.8: Your code here. (2/2) */
	tlbwi									# 将CP0.ENTRYHI/LO 写入tlb的CP0。INDEX处
.set reorder

NO_SUCH_ENTRY:
	mtc0    t0, CP0_ENTRYHI					# 将t0重新赋值到CP0寄存器CP0_ENTRYHI中
	j       ra
END(tlb_out)

NESTED(do_tlb_refill, 24, zero)
	mfc0    a1, CP0_BADVADDR
	mfc0    a2, CP0_ENTRYHI
	andi    a2, a2, 0xff /* ASID is stored in the lower 8 bits of CP0_ENTRYHI */
.globl do_tlb_refill_call;
do_tlb_refill_call:
	addi    sp, sp, -24 /* Allocate stack for arguments(3), return value(2), and return address(1) */
	sw      ra, 20(sp) /* [sp + 20] - [sp + 23] store the return address */
	addi    a0, sp, 12 /* [sp + 12] - [sp + 19] store the return value */
	jal     _do_tlb_refill /* (Pte *, u_int, u_int) [sp + 0] - [sp + 11] reserved for 3 args */
	lw      a0, 12(sp) /* Return value 0 - Even page table entry */
	lw      a1, 16(sp) /* Return value 1 - Odd page table entry */
	lw      ra, 20(sp) /* Return address */
	addi    sp, sp, 24 /* Deallocate stack */
	mtc0    a0, CP0_ENTRYLO0 /* Even page table entry */
	mtc0    a1, CP0_ENTRYLO1 /* Odd page table entry */
	nop
	/* Hint: use 'tlbwr' to write CP0.EntryHi/Lo into a random tlb entry. */
	/* Exercise 2.10: Your code here. */
	tlbwr
	
	jr      ra
END(do_tlb_refill)
