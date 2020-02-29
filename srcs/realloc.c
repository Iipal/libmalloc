#include "libmalloc.h"

#define LIBM_INTERNAL
#include "libm_internal.h"
#undef LIBM_INTERNAL

// with optimization, this code will transform into libc-memcpy call.
static inline void	*_mmemcpy(void *restrict dst,
						const void *restrict src, size_t n);

void	*realloc(void *ptr, size_t size) {
	if (!ptr) {
		return malloc(size);
	} else if (!!ptr && !size) {
		free(ptr);
		return NULL;
	}

	const size_t	__ptrsize = __mblk_get_size((ptrdiff_t)ptr - __mblkt_size);

	if (__ptrsize >= __mblk_align_size(size)) {
		return ptr;
	} else {
		void	*out = malloc(size);

		if (out) {
			_mmemcpy(out, ptr, __ptrsize);
			free(ptr);
		}
		return out;
	}
}

static inline void	*_mmemcpy(void *restrict dst,
						const void *restrict src, size_t n) {
	uint8_t	*__dptr = (uint8_t*)dst;
	const uint8_t	*__sptr = (uint8_t*)src;
	size_t	__isize = n;

	while (__isize--)
		__dptr[__isize] = __sptr[__isize];
	return dst;
}
