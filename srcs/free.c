#define _GNU_SOURCE

#include "libmalloc.h"

#define LIBM_INTERNAL
#include "libm_internal.h"
#undef LIBM_INTERNAL

static inline void	*_fragmentation_left(void *restrict src_ptr);
static inline void	_fragmentation_right(void *restrict src_ptr);

void	free(void *ptr) {
	if (!ptr) {
		return ;
	}

	void	*__ptr = (void*)((ptrdiff_t)ptr - (ptrdiff_t)__mblkt_size);

	pthread_mutex_lock(&__mmutex);
	__mblk_set_free(__ptr, __mblk_get_size(__ptr), __mblk_free);
	if (!!(__ptr = _fragmentation_left(__ptr))) {
		_fragmentation_right(__ptr);
	}
	pthread_mutex_unlock(&__mmutex);
}

static inline void	*_fragmentation_left(void *restrict src_ptr) {
	if (!__mblk_valid_start(src_ptr, __mblkt_size)) {
		return NULL;
	}

	ptrdiff_t	__baseptr = (ptrdiff_t)src_ptr;
	ptrdiff_t	__iptr = __baseptr - __mblk_piter(
		__mblk_get_size(__baseptr - __mblkt_psize));

	while (((uintptr_t)__iptr >= (uintptr_t)__mstart)
	&& __mblk_get_free(__iptr)) {
		const size_t	__fragsize = __mblk_get_size(__iptr)
								+ __mblk_get_size(__baseptr)
								+ __mblkt_bd_size;
		__mblk_clear(__baseptr);
		__mblk_set_free(__iptr, __fragsize, __mblk_free);
		__mblk_set_size(__iptr, __fragsize, __fragsize);
		if (!__mblk_valid_start(__iptr, __mblkt_size)
		|| !__mblk_valid_start(__iptr, __mblk_get_size(__iptr - __mblkt_psize)))
			break ;
		__baseptr = __iptr;
		__iptr -= __mblk_piter(__mblk_get_size(__iptr - __mblkt_psize));
	}
	return (void*)__baseptr;
}

static inline void	_fragmentation_right(void *restrict src_ptr) {
	if (!__mblk_valid_end(src_ptr, __mblkt_psize)) {
		return ;
	}

	ptrdiff_t	__baseptr = (ptrdiff_t)src_ptr;
	ptrdiff_t	__iptr = __baseptr + __mblk_piter(__mblk_get_size(__baseptr));

	while (((uintptr_t)__iptr < (uintptr_t)__mend) && __mblk_get_free(__iptr)) {
		const size_t	__fragsize = __mblk_get_size(__iptr)
								+ __mblk_get_size(__baseptr)
								+ __mblkt_bd_size;
		__mblk_clear(__iptr);
		__mblk_set_free(__baseptr, __fragsize, __mblk_free);
		__mblk_set_size(__baseptr, __fragsize, __fragsize);
		__baseptr += __mblk_iter(__fragsize);
		__iptr = __baseptr;
	}
}

void	__free_all(void) {
	pthread_mutex_trylock(&__mmutex);
	brk(__mstart);
	__mend = __mstart;
	pthread_mutex_unlock(&__mmutex);
}
