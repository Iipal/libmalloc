#define _GNU_SOURCE

#include "libmalloc.h"

#define LIBM_INTERNAL
#include "libm_internal.h"
#undef LIBM_INTERNAL

inline size_t	mhsize(void) {
	if (!__mstart || !__mend) {
		return (size_t)0;
	}
	return (size_t)((uintptr_t)__mend - (uintptr_t)__mstart);
}
