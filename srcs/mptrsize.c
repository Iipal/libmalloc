#define _GNU_SOURCE

#include "libmalloc.h"

#define LIBM_INTERNAL
#include "libm_internal.h"
#undef LIBM_INTERNAL

inline size_t	mptrsize(void *ptr) {
	size_t	__mptrsize_out = 0;

	if (!!ptr) {
		pthread_mutex_trylock(&__mmutex);
		__mptrsize_out = __mblk_get_size(
			(ptrdiff_t)ptr - (ptrdiff_t)__mblkt_size);
		pthread_mutex_unlock(&__mmutex);
	}
	return __mptrsize_out;
}
