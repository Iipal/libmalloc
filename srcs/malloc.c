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

static inline void	malloc_init(void) {
	assert(((void*)-1) != (__mstart = sbrk(0)));
	__mend = __mstart;
	atexit(__free_all);
}

static inline void	*find_best_free_block(const mblk_value_t __require_size) {
	void	*__iptr = __mstart;
	void	*__bestptr = NULL;
	mblk_value_t	__isize = 0UL;
	mblk_value_t	__bestsize = mhsize();

	while (__iptr < __mend) {
		__isize = __mblk_get_size(__iptr);
		if (__mblk_get_free(__iptr)
		&& __isize >= __require_size
		&& __isize <= __bestsize) {
			__bestsize = __isize;
			__bestptr = __iptr;
		}
		__iptr += __mblkt_iter(__isize);
	}
	if (!__bestptr)
		return NULL;
	__mblk_set_size(__bestptr, __require_size, __require_size);
	__mblk_set_free(__bestptr, __require_size, 0);
	if (__bestsize > __require_size) {
		if ((__bestsize - __mblkt_bd_size) > __require_size) {
			const mblk_value_t	__fragmentation_size
				= __bestsize - __require_size - __mblkt_bd_size;
			__mblk_set_size(__bestptr + __mblkt_iter(__require_size),
				__fragmentation_size, __fragmentation_size);
			__mblk_set_free(__bestptr + __mblkt_iter(__require_size),
				__fragmentation_size, 1);
		} else {
			__mblk_set_size(__bestptr, __bestsize, __bestsize);
			__mblk_set_free(__bestptr, __bestsize, 0);
		}
	}
	return __bestptr;
}

static inline void	*new_block(const mblk_value_t __size) {
	const mblk_value_t	__alloc_size = __size + __mblkt_bd_size;
	void	*out = sbrk(__alloc_size);

	if (((void*)-1) == out) {
		out = NULL;
	} else {
		__mend += __alloc_size;
		__mblk_set_size(out, __size, __size);
		__mblk_set_free(out, __size, 0);
	}
	return out;
}

inline void	*malloc(size_t size) {
	const mblk_value_t	__align_size = __mblk_align_size(size);
	void	*out = NULL;

	if (!__mstart && !__mend)
		malloc_init();
	if (__mstart != __mend)
		out = find_best_free_block(__align_size);
	if (!out)
		out = new_block(__align_size);
	return out ? (out + __mblkt_size) : out;
}
