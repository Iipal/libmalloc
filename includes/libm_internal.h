#ifndef LIBM_INTERNAL_H
# define LIBM_INTERNAL_H

# ifndef LIBM_INTERNAL
#  error "include only libmalloc.h"
# endif

# include <unistd.h>
# include <assert.h>
# include <errno.h>
# include <err.h>

/*
Memory block metadata type - mblk_t:
	contain 8-bytes aligned size and in 1-st bit for indicate is address free.
	2-nd and 3-rd bits are unused.

Using implicit "list" model: Bidirectional Coalescing.
*/
typedef unsigned long mblk_t;

void	*__mlast;
void	*__mstart;

extern void	malloc_init(void);
extern void	*new_block(const mblk_t size);
extern void	*find_best_free_block(const mblk_t size);

# define __mblkt_size    ((mblk_t)(sizeof(mblk_t)))
# define __mblkt_bd_size ((mblk_t)(sizeof(mblk_t) * 2UL))

# define __mblk_mask_free ((mblk_t)(0x0000000000000001UL))
# define __mblk_mask_size ((mblk_t)(0xFFFFFFFFFFFFFFF8UL))

# define __mblk_clear_free(free) ((free) & __mblk_mask_free)
# define __mblk_clear_size(size) ((size) & __mblk_mask_size)

# define __mblk_is_free(mblk) !((*((mblk_t*)(mblk))) & __mblk_mask_free)
# define __mblk_get_size(mblk) ((*((mblk_t*)(mblk))) & __mblk_mask_size)

# define __mblk_set_free(mblk, size) { \
	((*((mblk_t*)(mblk))) |= __mblk_mask_free); \
	((*((mblk_t*)((mblk) + (size) + __mblkt_size))) \
		|= __mblk_mask_free); }
# define __mblk_set_size(mblk, size) { \
	((*((mblk_t*)(mblk))) |= __mblk_clear_size(size)); \
	((*((mblk_t*)((mblk) + (size) + __mblkt_size))) \
		|= __mblk_clear_size(size)); }

# define __mblk_unset_free(mblk, size) { \
	((*((mblk_t*)(mblk))) &= ~__mblk_mask_free); \
	((*((mblk_t*)((mblk) + (size) + __mblkt_size))) \
		&= ~__mblk_mask_free); }
# define __mblk_unset_size(mblk, size) { \
	((*((mblk_t*)(mblk))) &= ~__mblk_clear_size(size)); \
	((*((mblk_t*)((mblk) + (size) + __mblkt_size))) \
		&= ~__mblk_clear_size(size)); }

# define __mblk_align_bits ((mblk_t)(sizeof(unsigned long) - 1UL))

#endif /* LIBM_INTERNAL_H */
