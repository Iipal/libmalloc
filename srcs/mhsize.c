#define _GNU_SOURCE

#include "libmalloc.h"

#define LIBM_INTERNAL
#include "libm_internal.h"
#undef LIBM_INTERNAL

inline size_t	mhsize(void) {
	return (!__mstart || !__mend) ? 0UL : (size_t)(__mend - __mstart);
}
