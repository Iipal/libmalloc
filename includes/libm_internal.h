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

// Function using with atexit() for return the heap pointer back to the start.
void	__free_all(void);

/*
Memory block metadata type - mblk_t:
	size - exactly the size of a memory block
	free - 1 if block is a free to use, 0 if not

Using implicit "list" model: Bidirectional Coalescing.
*/
# ifndef S_MEM_BLOCK
#  define S_MEM_BLOCK

typedef unsigned long mblk_value_t;
typedef struct s_mem_block mblk_t;

struct s_mem_block {
	mblk_value_t	size:((sizeof(mblk_value_t) * __CHAR_BIT__) - 3);
	mblk_value_t	free:3;
};

# endif /* S_MEM_BLOCK */

extern void	*__mend;
extern void	*__mstart;

# define __mblkt_size        ((mblk_value_t)(sizeof(mblk_t)))
# define __mblkt_bd_size     ((mblk_value_t)(sizeof(mblk_t) * 2UL))
# define __mblkt_iter(_size) ((mblk_value_t)((_size) + __mblkt_bd_size))

# define __mblk_get_free(_ptr) (((mblk_t*)(_ptr))->free)
# define __mblk_get_size(_ptr) (((mblk_t*)(_ptr))->size)

# define __mblk_clear(_ptr) do { \
	__mblk_get_free(_ptr) = 0; \
	__mblk_get_size(_ptr) = 0; \
} while (0)

# define __mblk_set_free(_ptr, _offset, _free_value) do { \
	__mblk_get_free(_ptr) = (_free_value); \
	__mblk_get_free((_ptr) + (_offset) + __mblkt_size) = (_free_value); \
} while (0)

# define __mblk_set_size(_ptr, _offset, _size_value) do { \
	__mblk_get_size(_ptr) = (_size_value); \
	__mblk_get_size((_ptr) + (_offset) + __mblkt_size) = (_size_value); \
} while (0)

# define __mblk_align_size(_size) \
	((mblk_value_t)(((_size) % sizeof(void*)) \
		? ((_size) + ((~(_size) & 0x7) + 1UL)) : (_size)))

# define __mblk_valid_start(_ptr, _offset) ((_ptr) - (_offset) >= __mstart)
# define __mblk_valid_end(_ptr, _offset)   ((_ptr) + (_offset) <= __mend)

#endif /* LIBM_INTERNAL_H */
