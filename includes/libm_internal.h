#ifndef LIBM_INTERNAL_H
# define LIBM_INTERNAL_H

# ifndef LIBM_INTERNAL
#  error "include only libmalloc.h"
# endif

# include <err.h>     // errx
# include <unistd.h>  // brk & sbrk
# include <string.h>  // memcpy
# include <strings.h> // bzero
# include <stdbool.h>
# include <stdint.h>

// Function using with atexit() for return the heap pointer back to the start.
void	__free_all(void);

# ifndef S_MEM_BLOCK
#  define S_MEM_BLOCK
/*
	Memory block metadata type - mblk_t:
		size - exactly the size of a memory block
		free - 1 if block is a free to use, 0 if not

	Using implicit free list model: Bidirectional Coalescing.
*/
typedef struct s_mem_block mblk_t;

struct s_mem_block {
	size_t	size:((sizeof(size_t) * __CHAR_BIT__) - 3);
	size_t	free:3;
} __attribute__((aligned(8)));

# endif /* S_MEM_BLOCK */

extern void	*__mend;
extern void	*__mstart;

# define __mblkt_size    (sizeof(mblk_t))
# define __mblkt_bd_size (__mblkt_size * 2)

# define __mblk_free     (size_t)1
# define __mblk_not_free (size_t)0

# define __mblk_iter(_size) ((_size) + __mblkt_bd_size)

# define __mblk_get_free(_ptr) (((mblk_t*)(_ptr))->free)
# define __mblk_get_size(_ptr) (((mblk_t*)(_ptr))->size)

# define __mblk_clear(_ptr) __extension__({ \
	__mblk_get_free(_ptr) = 0; \
	__mblk_get_size(_ptr) = 0; \
})

# define __mblk_set_free(_ptr, _offset, _free_value) __extension__({ \
	__mblk_get_free(_ptr) = (_free_value); \
	__mblk_get_free((uintptr_t)(_ptr) + (_offset) + __mblkt_size) \
		= (_free_value); \
})

# define __mblk_set_size(_ptr, _offset, _size_value) __extension__({ \
	__mblk_get_size(_ptr) = (_size_value); \
	__mblk_get_size((uintptr_t)(_ptr) \
		+ (uintptr_t)(_offset) \
		+ (uintptr_t)__mblkt_size) \
		= (_size_value); \
})

# define __mblk_align_size(_size) ((size_t)(((_size) % sizeof(void*)) \
					? ((_size) + ((~(_size) & 0x7) + 1UL)) : (_size)))

# define __mblk_valid_start(_ptr, _offset) \
	(((uintptr_t)(_ptr) - (_offset)) >= (uintptr_t)__mstart)
# define __mblk_valid_end(_ptr, _offset) \
	(((uintptr_t)(_ptr) + (_offset)) <= (uintptr_t)__mend)

#endif /* LIBM_INTERNAL_H */
