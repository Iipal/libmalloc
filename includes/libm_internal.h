#ifndef LIBM_INTERNAL_H
# define LIBM_INTERNAL_H

# ifndef LIBM_INTERNAL
#  error "include only libmalloc.h"
# endif

# include <sys/mman.h>
# include <stdbool.h>
# include <stdio.h>
# include <unistd.h>
# include <assert.h>

/*
	Function using with atexit() for return the heap pointer back to the start.
*/
void	__free_all(void);

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
# define __mblkt_iter(_size) ((mblk_t)((_size) + __mblkt_bd_size))

# define __ptr_get_mblk(_ptr) ((_ptr) - __mblkt_size)
# define __mblk_get_ptr(_mblk) ((_mblk) + __mblkt_size)

# define __mblk_mask_free ((mblk_t)(0x0000000000000001UL))
# define __mblk_mask_size ((mblk_t)(0xFFFFFFFFFFFFFFF8UL))

# define __mblk_get_value(_mblk) (*((mblk_t*)(_mblk)))

# define __mblk_clear_free(_mblk) (__mblk_get_value(_mblk) &= ~__mblk_mask_free)
# define __mblk_clear_size(_mblk) (__mblk_get_value(_mblk) &= ~__mblk_mask_size)
# define __mblk_clear_value(_mblk) \
	(__mblk_get_value(_mblk) ^= __mblk_get_value(_mblk))

# define __mblk_is_free(_mblk) !(__mblk_get_value(_mblk) & __mblk_mask_free)
# define __mblk_get_size(_mblk) (__mblk_get_value(_mblk) & __mblk_mask_size)

# define __mblk_set_free(_mblk, _size) { \
	__mblk_clear_free(_mblk); \
	__mblk_get_value(_mblk) |= __mblk_mask_free; \
	__mblk_clear_free((_mblk) + (_size) + __mblkt_size); \
	__mblk_get_value((_mblk) + (_size) + __mblkt_size) |= __mblk_mask_free; }
# define __mblk_set_size(_mblk, _size) { \
	__mblk_clear_size(_mblk); \
	__mblk_get_value(_mblk) |= _size; \
	__mblk_clear_size((_mblk) + (_size) + __mblkt_size); \
	__mblk_get_value((_mblk) + (_size) + __mblkt_size) |= _size; }

# define __mblk_unset_free(_mblk, _size) { \
	__mblk_clear_free(_mblk); \
	__mblk_clear_free((_mblk) + (_size) + __mblkt_size); }
# define __mblk_unset_size(_mblk, _size) { \
	__mblk_clear_size(_mblk); \
	__mblk_clear_size((_mblk) + (_size) + __mblkt_size); }

# define __mblk_align_bits       ((mblk_t)(sizeof(void*) - 1UL))
# define __mblk_align_size(_size) \
	((mblk_t)((_size % sizeof(void*)) \
		? ((_size) + ((~(_size) & __mblk_align_bits) + 1UL)) : (_size)))

# define __mblk_valid_start(_mblk, _offset) ((_mblk) - (_offset) >= __mstart)
# define __mblk_valid_end(_mblk, _offset)   ((_mblk) + (_offset) <= __mend)

#endif /* LIBM_INTERNAL_H */
