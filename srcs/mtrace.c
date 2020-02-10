#define _GNU_SOURCE

#include "libmalloc.h"

#define LIBM_INTERNAL
#include "libm_internal.h"
#undef LIBM_INTERNAL

size_t	mtrace(size_t n_blocks, int flags)
{
	void	*__iptr = __mstart;
	mblk_t	__isize = 0UL;
	bool	__ifree = false;
	size_t	__m_blocks = 0;

	if (libm_mtrace_none != flags) {
		switch (flags) {
			case libm_mtrace_not_freed: printf("Memory not freed:\n"); break ;
			case libm_mtrace_freed: printf("Memory freed:\n"); break ;
			case libm_mtrace_all: printf("Memory all:\n"); break ;
			default: break ;
		}
		printf("-----------------\n"
			"   Address     Size\n");
	}
	while (n_blocks > __m_blocks && __iptr < __mend) {
		__isize = __mblk_get_size(__iptr);
		__ifree = __mblk_is_free(__iptr);
		if ((libm_mtrace_not_freed == flags || libm_mtrace_all == flags)
		&& !__ifree)
			printf("%-12p%7lu%c\n", __iptr + __mblkt_size, __isize,
				(libm_mtrace_all == flags)
					? (__ifree ? 'f' : '-') : ' ');
		else if ((libm_mtrace_freed == flags || libm_mtrace_all == flags)
		&& __ifree)
			printf("%-12p%7lu%c\n", __iptr + __mblkt_size, __isize,
				(libm_mtrace_all == flags)
					? (__ifree ? 'f' : '-') : ' ');
		__iptr += __mblkt_iter(__isize);
		++__m_blocks;
	}
	return __m_blocks;
}

# undef _GNU_SOURCE
