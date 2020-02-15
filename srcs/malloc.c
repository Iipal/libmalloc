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
	__mstart = sbrk(0);
	__mend = __mstart;
	assert(((void*)-1) != (__mstart = sbrk(0)));
	atexit(__free_all);
}

static inline void	*find_best_free_block(const mblk_t __size) {
	void	*__iptr = __mstart;
	void	*__bestptr = NULL;
	mblk_t	__bestsize = ~0UL;
	mblk_t	__isize = 0UL;

	while (__iptr < __mend) {
		__isize = __mblk_get_size(__iptr);
		if (__mblk_is_free(__iptr)
		&& __isize >= __size
		&& __isize <= __bestsize) {
			__bestsize = __isize;
			__bestptr = __iptr;
		}
		__iptr += __mblkt_iter(__isize);
	}
	if (__bestptr) {
		__mblk_set_size(__bestptr, __size);
		__mblk_set_free(__bestptr, __size);
		if (__bestsize - __mblkt_bd_size > __size) {
			__mblk_set_size(__bestptr + __mblkt_iter(__size),
				__bestsize - __size - __mblkt_bd_size);
			__mblk_unset_free(__bestptr + __mblkt_iter(__size),
				__bestsize - __size - __mblkt_bd_size);
		}
	}
	return __bestptr;
}

static inline void	*new_block(const mblk_t __size) {
	const mblk_t	__alloc_size = __size + __mblkt_bd_size;
	void			*out = NULL;

	assert(((void*)-1) != (out = sbrk(__alloc_size)));
	__mend += __alloc_size;
	__mblk_set_size(out, __size);
	__mblk_set_free(out, __size);
	return out;
}

inline void	*malloc(size_t size) {
	const mblk_t	__align_size = __mblk_align_size(size);
	void			*out = NULL;

	if (!__mstart && !__mend)
		malloc_init();
	if (__mstart != __mend)
		out = find_best_free_block(__align_size);
	if (!out)
		out = new_block(__align_size);
	assert(out);
	return __mblk_get_ptr(out);
}
