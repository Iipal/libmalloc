#define _GNU_SOURCE

#include "libmalloc.h"

#define LIBM_INTERNAL
#include "libm_internal.h"
#undef LIBM_INTERNAL

// shared from stdlib.h
extern int atexit (void (*__func) (void))
	__attribute__ ((__nothrow__ __LEAF))
	__attribute__ ((__nonnull__ (1)));

void	*__mend = NULL;
void	*__mstart = NULL;

static inline void	*find_best_free_block(const size_t __require_size);
static inline void	*new_block(const size_t __size);
static inline bool	malloc_init(void);

void	*malloc(size_t size) {
	const size_t	__align_size = __mblk_align_size(size);
	void	*out = NULL;

	if ((!__mstart || !__mend) && !malloc_init()) {
		return NULL;
	}
	if (!(out = find_best_free_block(__align_size))) {
		if (!(out = new_block(__align_size))) {
			return NULL;
		}
	}
	return (void*)((uintptr_t)out + __mblkt_size);
}

static inline bool	malloc_init(void) {
	if ((void*)-1 == (__mstart = sbrk(0))) {
		__mstart = NULL;
		return false;
	}
	__mend = __mstart;
	atexit(__free_all);
	return true;
}

static inline void	*new_block(const size_t __size) {
	void	*out = sbrk((intptr_t)(__size + __mblkt_bd_size));

	if (((void*)-1) == out) {
		out = NULL;
	} else {
		__mend = (void*)((uintptr_t)__mend
			+ (__size + (uintptr_t)__mblkt_bd_size));
		__mblk_set_size(out, __size, __size);
		__mblk_set_free(out, __size, __mblk_not_free);
	}
	return out;
}

static inline void	frag_free_space(const ptrdiff_t __bestptr,
									const size_t __bestsize,
									const size_t __require_size) {
	if ((__bestsize - __mblkt_bd_size) > __require_size) {
		const size_t	__fragmentation_size
			= __bestsize - __mblkt_bd_size - __require_size;

		__mblk_set_size((uintptr_t)__bestptr + __mblk_iter(__require_size),
			__fragmentation_size, __fragmentation_size);
		__mblk_set_free((uintptr_t)__bestptr + __mblk_iter(__require_size),
			__fragmentation_size, __mblk_free);
	} else {
		__mblk_set_size(__bestptr, __bestsize, __bestsize);
		__mblk_set_free(__bestptr, __bestsize, __mblk_not_free);
	}
}

static inline void	*find_best_free_block(const size_t __require_size) {
	ptrdiff_t	__iptr = (ptrdiff_t)__mstart;
	ptrdiff_t	__bestptr = 0;
	size_t	__isize = 0;
	size_t	__bestsize = mhsize();

	while ((uintptr_t)__iptr < (uintptr_t)__mend) {
		__isize = __mblk_get_size(__iptr);
		if (__mblk_get_free(__iptr)
		&& __isize >= __require_size
		&& __isize <= __bestsize) {
			__bestsize = __isize;
			__bestptr = __iptr;
		}
		__iptr += __mblk_iter(__isize);
	}
	if (!!__bestptr) {
		__mblk_set_size(__bestptr, __require_size, __require_size);
		__mblk_set_free(__bestptr, __require_size, __mblk_not_free);
		if (__bestsize > __require_size) {
			frag_free_space(__bestptr, __bestsize, __require_size);
		}
	}
	return (void*)__bestptr;
}
