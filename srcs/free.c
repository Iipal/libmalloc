#include "libmalloc.h"

#define LIBM_INTERNAL
#include "libm_internal.h"
#undef LIBM_INTERNAL

inline void	free(void *restrict ptr)
{
	__mblk_set_free(ptr - __mblkt_size, __mblkt_size, __mblk_free);
}
