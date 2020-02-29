#include "libmalloc.h"

#define LIBM_INTERNAL
#include "libm_internal.h"
#undef LIBM_INTERNAL

inline void	*realloc(void *ptr, size_t size) {
	(void)size;
	free(ptr);
	return NULL;
}
