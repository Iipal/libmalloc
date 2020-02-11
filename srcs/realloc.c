#include "libmalloc.h"

#define LIBM_INTERNAL
#include "libm_internal.h"
#undef LIBM_INTERNAL

// with optimization, this code will transform into libc-memcpy call.
static inline void	*_mmemcpy(void *restrict dst,
						const void *restrict src,
						size_t n) {
	unsigned char	*__dptr = (unsigned char*)dst;
	const unsigned char	*__sptr = (unsigned char*)src;
	mblk_t			__isize = n;

	while (__isize--)
		__dptr[__isize] = __sptr[__isize];
	return dst;
}

inline void	*realloc(void *ptr, size_t size)
{
	void			*__ptr = __ptr_get_mblk(ptr);
	const mblk_t	__ptrsize = __mblk_get_size(__ptr);
	const mblk_t	__newsize = __mblk_align_size(size);

	if (!ptr) {
		return (malloc(__newsize));
	} else if (!!ptr && !__newsize) {
		free(ptr);
		return NULL;
	} else if (__ptrsize >= __newsize) {
		return ptr;
	} else {
		void	*out = calloc(1UL, __newsize);

		if (out) {
			_mmemcpy(out, ptr, __ptrsize);
			free(ptr);
		}
		return (out);
	}
}
