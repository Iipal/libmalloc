#define _GNU_SOURCE

#include "libmalloc.h"

#define LIBM_INTERNAL
#include "libm_internal.h"
#undef LIBM_INTERNAL

#include <err.h>

void	__free_all(void) {
	if (-1 == brk(__mstart))
		err(1, "brk:");
	__mend = __mstart;
}

static inline void	fragmentation_free_space_left(void *restrict ptr)
{
	void	*__baseptr = ptr;
	void	*__iptr = NULL;
	mblk_value_t	__basesize = 0UL;
	mblk_value_t	__isize = 0UL;
	mblk_value_t	__fragsize = 0UL;

	if (!__mblk_valid_start(ptr, __mblkt_size))
		return ;
	__isize = __mblk_get_size(ptr - __mblkt_size);
	__iptr = ptr - __mblkt_iter(__isize);
	if (__iptr < __mstart)
		return ;
	do {
		if (!__mblk_get_free(__iptr))
			break ;
		__isize = __mblk_get_size(__iptr);
		__basesize = __mblk_get_size(__baseptr);
		__fragsize = __isize + __basesize + __mblkt_bd_size;
		__mblk_clear(__iptr);
		__baseptr -= __mblkt_iter(__isize);
		__mblk_set_free(__baseptr, __fragsize, 1);
		__mblk_set_size(__baseptr, __fragsize, __fragsize);
		__iptr = __baseptr;
		if (__mblk_valid_start(__baseptr, __mblkt_size))
			__iptr -= __mblkt_iter(__mblk_get_size(__baseptr - __mblkt_size));
	} while (__iptr > __mstart);
}

static inline void	fragmentation_free_space_right(void *restrict ptr)
{
	void	*__baseptr = ptr;
	void	*__iptr = NULL;
	mblk_value_t	__fragsize = 0UL;

	if (!__mblk_valid_end(ptr, __mblkt_iter(__mblk_get_size(ptr))))
		return ;
	__iptr = ptr + __mblkt_iter(__mblk_get_size(ptr));
	if (__iptr > __mend)
		return ;
	while (__iptr < __mend && __mblk_get_free(__iptr)) {
		__fragsize = __mblk_get_size(__iptr)
				+ __mblk_get_size(__baseptr)
				+ __mblkt_bd_size;
		__mblk_clear(__iptr);
		__mblk_set_free(__baseptr, __fragsize, 1);
		__mblk_set_size(__baseptr, __fragsize, __fragsize);
		__baseptr += __mblkt_iter(__fragsize);
		__iptr = __baseptr;
	}
}

inline void	free(void *restrict ptr) {
	if (!ptr)
		return ;

	void	*__ptr = ptr - __mblkt_size;

	__mblk_set_free(__ptr, __mblk_get_size(__ptr), 1);
	fragmentation_free_space_left(__ptr);
	fragmentation_free_space_right(__ptr);
}
