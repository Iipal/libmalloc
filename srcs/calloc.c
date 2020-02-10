#define _GNU_SOURCE

#include "libmalloc.h"

#define LIBM_INTERNAL
#include "libm_internal.h"
#undef LIBM_INTERNAL

// with optimization, this code will transform into libc-memset call.
static inline void	*_mbzero(void *ptr, const mblk_t size)
{
	unsigned char	*__iptr = (unsigned char*)ptr;
	mblk_t			__isize = size;

	while (__isize--)
		__iptr[__isize] = 0;
	return ptr;
}

inline void	*calloc(size_t nmemb, size_t size) {
	const mblk_t	__size = ((size * nmemb) % sizeof(void*))
						? __mblk_align_size(size * nmemb)
						: (size * nmemb);

	return _mbzero(malloc(__size), __size);
}
