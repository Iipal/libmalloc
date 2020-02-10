#include "libmalloc.h"

#define LIBM_INTERNAL
#include "libm_internal.h"
#undef LIBM_INTERNAL

static inline void	fragmentation_free_space_left(void *restrict ptr)
{
	mblk_t	__isize = __mblk_get_size(ptr);
	void	*__iptr = ptr - __mblkt_iter(__isize);
	mblk_t	__basesize = 0UL;
	void	*__baseptr = ptr;

	if (__iptr <= __mstart)
		return ;
	while ((__iptr -= __mblkt_size) >= __mstart)
		if (__mblk_is_free(__iptr)) {
			__isize = __mblk_get_size(__iptr);
			__basesize = __mblk_get_size(__baseptr);
			__mblk_clear_value(__iptr);
			__baseptr -= __mblkt_iter(__isize);
			__mblk_set_size(__baseptr, __isize + __basesize);
			__iptr = __baseptr;
		} else {
			break ;
		}
}

static inline void	fragmentation_free_space_right(void *restrict ptr)
{
	void	*__iptr = ptr + __mblkt_iter(__mblk_get_size(ptr));
	mblk_t	__newsize = 0UL;
	void	*__baseptr = ptr;

	if (__iptr >= __mend)
		return ;
	while (__iptr < __mend)
		if (__mblk_is_free(__iptr)) {
			__newsize = __mblk_get_size(__iptr) + __mblk_get_size(__baseptr);
			__mblk_clear_value(__iptr);
			__mblk_set_size(__baseptr, __newsize);
			__baseptr += __mblkt_iter(__newsize);
			__iptr = __baseptr;
		} else {
			break ;
		}
}

inline void	free(void *restrict ptr) {
	void *restrict	__ptr = __ptr_get_mblk(ptr);

	__mblk_unset_free(__ptr, __mblk_get_size(__ptr));
	fragmentation_free_space_left(__ptr);
	fragmentation_free_space_right(__ptr);
}
