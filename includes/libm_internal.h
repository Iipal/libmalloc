#ifndef LIBM_INTERNAL_H
# define LIBM_INTERNAL_H

# ifndef LIBM_INTERNAL
#  error "include only libmalloc.h"
# endif

# include <unistd.h>
# include <errno.h>
# include <err.h>

void	*__mlast;
void	*__mstart;

extern void	malloc_init(void);

/*
mblk_t contain 8-bytes aligned size and in 1-st byte
 for indicate is address free.
2-st and 3-rd bytes are unused.

Using implicit list model: Bidirectional Coalescing.
*/
typedef unsigned long mblk_t;

# define __mblkt_size    ((mblk_t)(sizeof(mblk_t)))
# define __mblkt_bd_size ((mblk_t)(sizeof(mblk_t) * 2))

# define __mblk_free     ((mblk_t)0x0000000000000000)
# define __mblk_not_free ((mblk_t)0x0000000000000001)

# define __mblk_mask_free ((mblk_t)0x0000000000000001)
# define __mblk_mask_size ((mblk_t)0xFFFFFFFFFFFFFFF8)

# define __mblk_clear_free(free) ((free) & __mblk_mask_free)
# define __mblk_clear_size(size) ((size) & __mblk_mask_size)

# define __mblk_get_free(mblk) ((*((mblk_t*)(mblk))) & __mblk_mask_free)
# define __mblk_get_size(mblk) ((*((mblk_t*)(mblk))) & __mblk_mask_size)

# define __mblk_set_free(mblk, size, free) { \
	((*((mblk_t*)(mblk))) |= __mblk_clear_free(free)); \
	((*((mblk_t*)((mblk) + size))) |= __mblk_clear_free(free)); }
# define __mblk_set_size(mblk, size) { \
	((*((mblk_t*)(mblk))) |= __mblk_clear_size(size)); \
	((*((mblk_t*)((mblk) + size))) |= __mblk_clear_size(size)); }

#endif /* LIBM_INTERNAL_H */
