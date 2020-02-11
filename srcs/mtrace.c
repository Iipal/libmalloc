#define _GNU_SOURCE

#include "libmalloc.h"

#define LIBM_INTERNAL
#include "libm_internal.h"
#undef LIBM_INTERNAL

static inline size_t	__fast_mtrace(size_t n_blocks) {
	mblk_t	__m_blocks = 0UL;

	for (void *__iptr = __mstart;
		n_blocks > __m_blocks && __iptr < __mend;
		__iptr += __mblkt_iter(__mblk_get_size(__iptr)), __m_blocks++)
			;
	return (size_t)__m_blocks;
}

size_t	mtrace(size_t n_blocks, int flags)
{
	if (libm_mtrace_f_none == flags)
		return __fast_mtrace(n_blocks);

	void	*__iptr = __mstart;
	mblk_t	__isize = 0UL;
	mblk_t	__m_blocks = 0UL;
	bool	__ifree = false;

	switch (flags) {
		case libm_mtrace_f_not_freed: printf("Memory not freed:\n"); break ;
		case libm_mtrace_f_freed: printf("Memory freed:\n"); break ;
		case libm_mtrace_f_all: printf("Memory all:\n"); break ;
		default: break ;
	}
	printf("-----------------\n   N:   Address     Size\n");
	while ((mblk_t)n_blocks > __m_blocks && __iptr < __mend) {
		__isize = __mblk_get_size(__iptr);
		__ifree = __mblk_is_free(__iptr);
		if ((libm_mtrace_f_not_freed == flags || libm_mtrace_f_all == flags)
		&& !__ifree)
			printf("%4lu: %-12p%7lu%c\n", __m_blocks + 1UL,
				__iptr + __mblkt_size, __isize,
				(libm_mtrace_f_all == flags)
					? (__ifree ? 'f' : '-') : ' ');
		else if ((libm_mtrace_f_freed == flags || libm_mtrace_f_all == flags)
		&& __ifree)
			printf("%4lu: %-12p%7lu%c\n", __m_blocks + 1UL,
				__iptr + __mblkt_size, __isize,
				(libm_mtrace_f_all == flags)
					? (__ifree ? 'f' : '-') : ' ');
		__iptr += __mblkt_iter(__isize);
		++__m_blocks;
	}
	printf("\n");
	return (size_t)__m_blocks;
}
