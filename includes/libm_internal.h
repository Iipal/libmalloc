#ifndef LIBM_INTERNAL_H
# define LIBM_INTERNAL_H

# ifndef LIBM_INTERNAL
#  error "include only libmalloc.h"
# endif

# include <stdbool.h>
# include <stdio.h>
# include <unistd.h>
# include <assert.h>

/*
Memory block metadata type - mblk_t:
	contain 8-bytes aligned block size
		and in the lowest bit - boolean for indicating are address is free.

	x86_64: 2-nd and 3-rd bits are unused.
	x86: only 2-nd bit are unused.

Using implicit "list" model: Bidirectional Coalescing.
*/
typedef unsigned long mblk_t;

extern void	*__mend;
extern void	*__mstart;

# define __mblkt_size       ((mblk_t)(sizeof(mblk_t)))
# define __mblkt_bd_size    ((mblk_t)(sizeof(mblk_t) * 2UL))
# define __mblkt_iter(size) ((mblk_t)((size) + __mblkt_bd_size))

# define __ptr_get_mblk(ptr) ((ptr) - __mblkt_size)
# define __mblk_get_ptr(mblk) ((mblk) + __mblkt_size)

# define __mblk_mask_free ((mblk_t)(0x0000000000000001UL))
# define __mblk_mask_size ((mblk_t)(0xFFFFFFFFFFFFFFF8UL))

# define __mblk_get_value(mblk) (*((mblk_t*)(mblk)))

# define __mblk_clear_free(mblk) (__mblk_get_value(mblk) &= ~__mblk_mask_free)
# define __mblk_clear_size(mblk) (__mblk_get_value(mblk) &= ~__mblk_mask_size)
# define __mblk_clear_value(mblk) \
	(__mblk_get_value(mblk) ^= __mblk_get_value(mblk))

# define __mblk_is_free(mblk) !(__mblk_get_value(mblk) & __mblk_mask_free)
# define __mblk_get_size(mblk) (__mblk_get_value(mblk) & __mblk_mask_size)

# define __mblk_set_free(mblk, size) { \
	__mblk_clear_free(mblk); \
	__mblk_get_value(mblk) |= __mblk_mask_free; \
	__mblk_clear_free((mblk) + (size) + __mblkt_size); \
	__mblk_get_value((mblk) + (size) + __mblkt_size) |= __mblk_mask_free; }
# define __mblk_set_size(mblk, size) { \
	__mblk_clear_size(mblk); \
	__mblk_get_value(mblk) |= size; \
	__mblk_clear_size((mblk) + (size) + __mblkt_size); \
	__mblk_get_value((mblk) + (size) + __mblkt_size) |= size; }

# define __mblk_unset_free(mblk, size) { \
	__mblk_clear_free(mblk); \
	__mblk_clear_free((mblk) + (size) + __mblkt_size); }
# define __mblk_unset_size(mblk, size) { \
	__mblk_clear_size(mblk); \
	__mblk_clear_size((mblk) + (size) + __mblkt_size); }

# define __mblk_align_bits       ((mblk_t)(sizeof(void*) - 1UL))
# define __mblk_align_size(size) \
	((mblk_t)((size % sizeof(void*)) \
			? ((size) + ((~(size) & __mblk_align_bits) + 1UL)) : (size)))

#endif /* LIBM_INTERNAL_H */
