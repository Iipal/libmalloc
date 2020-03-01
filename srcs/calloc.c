#define _GNU_SOURCE

#include "libmalloc.h"

#define LIBM_INTERNAL
#include "libm_internal.h"
#undef LIBM_INTERNAL

void	*calloc(size_t nmemb, size_t size) {
	size_t	__align_size = __mblk_align_size(nmemb * size);
	void	*out = malloc(__align_size);

	if (out) {
		bzero(out, __align_size);
	}
	return out;
}
