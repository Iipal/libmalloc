#define _GNU_SOURCE

#include "libmalloc.h"

#define LIBM_INTERNAL
#include "libm_internal.h"
#undef LIBM_INTERNAL

inline void	*malloc(size_t size) {
	return NULL;
}
