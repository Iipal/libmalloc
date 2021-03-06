#define _GNU_SOURCE

#include "libmalloc.h"

#define LIBM_INTERNAL
#include "libm_internal.h"
#undef LIBM_INTERNAL

inline size_t	mhsize(void) {
	size_t	malloc_heap_size = 0;

	pthread_mutex_trylock(&__mmutex);
	if (__mstart || __mend) {
		malloc_heap_size = (size_t)((uintptr_t)__mend - (uintptr_t)__mstart);
	}
	pthread_mutex_unlock(&__mmutex);
	return malloc_heap_size;
}
