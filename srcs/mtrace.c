#define _GNU_SOURCE

#include "libmalloc.h"

#define LIBM_INTERNAL
#include "libm_internal.h"
#undef LIBM_INTERNAL

static inline size_t	__fast_mtrace(const size_t n_blocks) {
	size_t	__m_blocks = 0UL;

	if (!mhsize())
		return __m_blocks;
	for (void *__iptr = __mstart;
		n_blocks > __m_blocks && __iptr < __mend;
		__iptr += __mblkt_iter(__mblk_get_size(__iptr)), __m_blocks++)
			;
	return __m_blocks;
}

size_t	mtrace(size_t n_blocks, int flags)
{
	if (libm_mtrace_f_none == flags)
		return __fast_mtrace(n_blocks);

	ssize_t	__write_ret = 0;
	switch (flags) {
		case libm_mtrace_f_not_freed:
			__write_ret = write(1, "Memory not freed:\n", 18);
			break ;
		case libm_mtrace_f_freed:
			__write_ret = write(1, "Memory freed:\n", 14);
			break ;
		case libm_mtrace_f_all:
			__write_ret = write(1, "Memory all:\n", 12); break ;
		default: break ;
	}
	__write_ret = write(1, "-----------------\n   N:   Address     Size\n", 43);
	if (!mhsize()) {
		__write_ret = write(1, "\tno memory allocated yet.\n\n", 27);
		return 0;
	}
	(void)__write_ret; // gcc compatibility;

	mblk_t	__isize = 0UL;
	size_t	__m_blocks = 0;

	for (void *__iptr = __mstart;
		n_blocks > __m_blocks && __iptr < __mend;
		__iptr += __mblkt_iter(__isize), __m_blocks++) {
		const bool __ifree = __mblk_is_free(__iptr);

		__isize = __mblk_get_size(__iptr);
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
	}
	printf("\n");
	return __m_blocks;
}
