#define _GNU_SOURCE

#include "libmalloc.h"

#define LIBM_INTERNAL
#include "libm_internal.h"
#undef LIBM_INTERNAL

// with optimization, this code will transform into libc-memset call.
static inline void	*_mbzero(void *ptr, size_t size);

void	*calloc(size_t nmemb, size_t size) {
	size_t	__align_size = __mblk_align_size(nmemb * size);
	void	*out = malloc(__align_size);

	return out ? _mbzero(out, __align_size) : out;
}

static inline void	*_mbzero(void *ptr, size_t size) {
	unsigned char	*__iptr = (unsigned char*)ptr;
	size_t	__isize = size;

	while (__isize--)
		__iptr[__isize] = 0;
	return ptr;
}
