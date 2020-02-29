#define _GNU_SOURCE

#include "libmalloc.h"

#define LIBM_INTERNAL
#include "libm_internal.h"
#undef LIBM_INTERNAL

static inline void	fragmentation_free_space_left(void *restrict src_ptr);
static inline void	fragmentation_free_space_right(void *restrict src_ptr);

void	free(void *restrict ptr) {
	if (!ptr)
		return ;

	void *restrict	__ptr = (void *restrict)((ptrdiff_t)ptr - __mblkt_size);

	__mblk_set_free(__ptr, __mblk_get_size(__ptr), __mblk_free);
	fragmentation_free_space_left(__ptr);
	fragmentation_free_space_right(__ptr);
}

static inline void	fragmentation_free_space_left(void *restrict src_ptr) {
	ptrdiff_t	__baseptr = (ptrdiff_t)src_ptr;
	ptrdiff_t	__iptr = 0;
	size_t	__isize = 0;

	if (!__mblk_valid_start(__baseptr, __mblkt_size))
		return ;
	__isize = __mblk_get_size(__baseptr - __mblkt_size);
	__iptr = __baseptr - __mblkt_iter(__isize);
	if ((uintptr_t)__iptr < (uintptr_t)__mstart)
		return ;
	do {
		if (!__mblk_get_free(__iptr))
			break ;
		__isize = __mblk_get_size(__iptr);
		const size_t	__fragsize
			= __isize + __mblk_get_size(__baseptr) + (size_t)__mblkt_bd_size;

		__mblk_clear(__iptr);
		__baseptr -= __mblkt_iter(__isize);
		__mblk_set_free(__baseptr, __fragsize, __mblk_free);
		__mblk_set_size(__baseptr, __fragsize, __fragsize);
		__iptr = __baseptr;
		if (__mblk_valid_start(__baseptr, __mblkt_size))
			__iptr -= __mblkt_iter(__mblk_get_size(__baseptr - __mblkt_size));
	} while ((uintptr_t)__iptr > (uintptr_t)__mstart);
}

static inline void	fragmentation_free_space_right(void *restrict src_ptr) {
	ptrdiff_t	__baseptr = (ptrdiff_t)src_ptr;
	ptrdiff_t	__iptr = 0;

	if (!__mblk_valid_end(__baseptr, __mblkt_iter(__mblk_get_size(__baseptr))))
		return ;
	__iptr = __baseptr + __mblkt_iter(__mblk_get_size(__baseptr));
	if ((uintptr_t)__iptr > (uintptr_t)__mend)
		return ;
	while ((uintptr_t)__iptr < (uintptr_t)__mend && __mblk_get_free(__iptr)) {
		const size_t	__fragsize = __mblk_get_size(__iptr)
								+ __mblk_get_size(__baseptr)
								+ __mblkt_bd_size;
		__mblk_clear(__iptr);
		__mblk_set_free(__baseptr, __fragsize, __mblk_free);
		__mblk_set_size(__baseptr, __fragsize, __fragsize);
		__baseptr += __mblkt_iter(__fragsize);
		__iptr = __baseptr;
	}
}

#include <err.h>

// using via atexit()
void	__free_all(void) {
	if (-1 == brk((void*)__mstart))
		err(1, "brk:");
	__mend = __mstart;
}
