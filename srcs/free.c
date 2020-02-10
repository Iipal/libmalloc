#include "libmalloc.h"

#define LIBM_INTERNAL
#include "libm_internal.h"
#undef LIBM_INTERNAL

static inline void	fragmentation_free_space_left(void *restrict ptr)
{
	mblk_t	__isize = __mblk_get_size(ptr - __mblkt_size);
	void	*__iptr = ptr - __mblkt_iter(__isize);
	mblk_t	__basesize = 0UL;
	void	*__baseptr = ptr;

	if (__iptr < __mstart)
		return ;
	while (__iptr >= __mstart && __mblk_is_free(__iptr)) {
		__isize = __mblk_get_size(__iptr);
		__basesize = __mblk_get_size(__baseptr);
		__mblk_clear_value(__iptr);
		__baseptr -= __mblkt_iter(__isize);
		__mblk_set_size(__baseptr, __isize + __basesize + __mblkt_bd_size);
		if (__baseptr != __mstart)
			__iptr = __baseptr - __mblkt_iter(
					__mblk_get_size(__baseptr - __mblkt_size));
		else
			__iptr = __baseptr - 1UL;
	}
}

static inline void	fragmentation_free_space_right(void *restrict ptr)
{
	void	*__iptr = ptr + __mblkt_iter(__mblk_get_size(ptr));
	void	*__baseptr = ptr;
	mblk_t	__newsize = 0UL;

	if (__iptr >= __mend)
		return ;
	while (__iptr < __mend && __mblk_is_free(__iptr)) {
		__newsize = __mblk_get_size(__iptr)
				+ __mblk_get_size(__baseptr)
				+ __mblkt_bd_size;
		__mblk_clear_value(__iptr);
		__mblk_set_size(__baseptr, __newsize);
		__baseptr += __mblkt_iter(__newsize);
		__iptr = __baseptr;
	}
}

inline void	free(void *restrict ptr) {
	void *restrict	__ptr = __ptr_get_mblk(ptr);

	__mblk_unset_free(__ptr, __mblk_get_size(__ptr));
	fragmentation_free_space_left(__ptr);
	fragmentation_free_space_right(__ptr);
}
