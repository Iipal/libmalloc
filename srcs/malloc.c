#include "libmalloc.h"

#define LIBM_INTERNAL
#include "libm_internal.h"
#undef LIBM_INTERNAL

void	*__mlast = NULL;
void	*__mstart = NULL;

inline void	malloc_init(void)
{
	if ((void*)-1 == (__mstart = sbrk(0)))
		err(errno, "");
	__mlast = __mstart;
}

void	*malloc(size_t size)
{
	void	*out = NULL;
	size_t	alloc_size = size + __mblkt_bd_size;

	if (!__mstart && !__mlast)
		malloc_init();
	if (__mstart == __mlast) {
		out = sbrk(alloc_size);
		__mblk_set_size(out, size);
		__mblk_set_free(out, size, __mblk_not_free);
		__mlast += alloc_size;
	}
	return out + __mblkt_size;
}
