#include <lib9.h>

ulong
getcallerpc(void *x)
{
	return (ulong)__builtin_return_address(0);
}

