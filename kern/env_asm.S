#include <asm/asm.h>
#include <mmu.h>
#include <trap.h>
#include <kclock.h>

.text
LEAF(env_pop_tf)
.set reorder
.set at
	mtc0    a1, CP0_ENTRYHI
	li 		t0, 0
	mtc0	t0, CP0_COUNT
	move    sp, a0
	RESET_KCLOCK
	j       ret_from_exception
END(env_pop_tf)
