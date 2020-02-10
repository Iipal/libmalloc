#include "libmalloc.h"

#define LIBM_INTERNAL
#include "libm_internal.h"
#undef LIBM_INTERNAL

void	*__mlast = NULL;
void	*__mstart = NULL;

inline void	malloc_init(void) {
	__mstart = sbrk(0);
	if ((void*)-1 == __mstart)
		err(errno, "");
	__mlast = __mstart;
}

inline void	*find_best_free_block(const mblk_t __size) {
	void	*__iptr = __mstart;
	void	*__bestptr = NULL;
	mblk_t	__bestsize = __mlast - __mstart;

	while (__iptr < __mlast) {
		const mblk_t __isize = __mblk_get_size(__iptr);

		if (__mblk_is_free(__iptr)
		&& __isize >= __size
		&& __isize - __size < __bestsize) {
			__bestsize = __isize - __size;
			__bestptr = __iptr;
		}
		__iptr += __isize + __mblkt_bd_size;
	}
	if (__bestptr) {
		__mblk_set_size(__bestptr, __size);
		__mblk_set_free(__bestptr, __size);
	}
	return __bestptr;
}

inline void	*new_block(const mblk_t __size) {
	const mblk_t	__asize = __size + __mblkt_bd_size;
	void			*out = NULL;

	assert((void*)-1 != (out = sbrk(__asize)));
	__mlast += __asize;
	__mblk_set_size(out, __size);
	__mblk_set_free(out, __size);
	return out;
}

void	*malloc(size_t size) {
	mblk_t	__size = size;
	void	*out = NULL;

	if (__size % sizeof(void*))
		__size += (~size & __mblk_align_bits) + 1UL;
	if (!__mstart && !__mlast)
		malloc_init();
	if (__mstart == __mlast)
		out = new_block(__size);
	else
		out = find_best_free_block(__size);
	return out + __mblkt_size;
}
