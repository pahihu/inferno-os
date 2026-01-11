#define EMU_ASM 1
#include "lib9.h"
#include "emu.h"

/*
 * umult(ulong m1, ulong m2, ulong *hi)
 */
void umult(u32int m1, u32int m2, u32int *hi)
{
    u64int result = (u64int)m1 * (u64int)m2;
    *hi = (u32int)(result >> 32);
}

void FPsave(FPU *ptr)
{
    fegetenv(ptr);
}

void FPrestore(FPU *ptr)
{
    fesetenv(ptr);
}

int _tas(int *ptr)
{
    return __sync_lock_test_and_set(ptr, (int)1);
}
