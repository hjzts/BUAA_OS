#include <asm/asm.h>
#include <stackframe.h>

.macro BUILD_HANDLER exception handler
# 这里的 \ 是参数替换符，用于在宏展开时将宏的参数 exception 
# 插入到这个位置，即替换成实际的异常处理函数的名字。
NESTED(handle_\exception, TF_SIZE + 8, zero)
# 这里也很有意思
# \exception是调用的异常类型，比如sys，实际这里会调用handler_sys
	move    a0, sp
	addiu   sp, sp, -8
	jal     \handler
	# 这就是这里会跳转到handler这个宏所指示的方法这里
	addiu   sp, sp, 8
	j       ret_from_exception
END(handle_\exception)
.endm

.text

FEXPORT(ret_from_exception)
	RESTORE_ALL
	eret

NESTED(handle_int, TF_SIZE, zero)
	mfc0    t0, CP0_CAUSE
	mfc0    t2, CP0_STATUS
	and     t0, t2
	andi    t1, t0, STATUS_IM7
	bnez    t1, timer_irq
timer_irq:
	li      a0, 0
	j       schedule
END(handle_int)

BUILD_HANDLER tlb do_tlb_refill

#if !defined(LAB) || LAB >= 4
BUILD_HANDLER mod do_tlb_mod
BUILD_HANDLER sys do_syscall
#endif

BUILD_HANDLER reserved do_reserved
