#define _GNU_SOURCE

#include "libmalloc.h"

#define LIBM_INTERNAL
#include "libm_internal.h"
#undef LIBM_INTERNAL

inline size_t	mptrsize(void *ptr) {
	if (!ptr) {
		return 0;
	}
	return __mblk_get_size((ptrdiff_t)ptr - __mblkt_size);
}
