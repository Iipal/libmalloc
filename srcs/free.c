#include "libmalloc.h"

#define LIBM_INTERNAL
#include "libm_internal.h"
#undef LIBM_INTERNAL

inline void	free(void *restrict ptr) {
	__mblk_unset_free(ptr - __mblkt_size, __mblk_get_size(ptr - __mblkt_size));
}
