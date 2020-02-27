#define _GNU_SOURCE

#include "libmalloc.h"

#define LIBM_INTERNAL
#include "libm_internal.h"
#undef LIBM_INTERNAL

static inline size_t	__fast_mtrace(const size_t n_blocks) {
	size_t	__m_blocks = 0UL;

	if (mhsize())
		for (void *__iptr = __mstart;
			n_blocks > __m_blocks && __iptr < __mend;
			__iptr += __mblkt_iter(__mblk_get_size(__iptr)), __m_blocks++)
				;
	return __m_blocks;
}

size_t	mtrace(size_t n_blocks, int flags)
{
	if (libm_mtrace_f_none & flags)
		return __fast_mtrace(n_blocks);

	switch (flags) {
		case libm_mtrace_f_not_freed:
			write(1, "Memory not freed:\n", 18);
			break ;
		case libm_mtrace_f_freed:
			write(1, "Memory freed:\n", 14);
			break ;
		case libm_mtrace_f_all:
			write(1, "Memory all:\n", 12); break ;
		default: break ;
	}
	write(1, "-----------------\n   N:   Address     Size\n", 43);
	if (!mhsize()) {
		write(1, "\tno memory allocated yet.\n\n", 27);
		return 0;
	}

	size_t	__m_blocks = 0;
	mblk_value_t	__isize = 0UL;
	mblk_value_t	__free_size = 0UL;
	mblk_value_t	__not_free_size = 0UL;

	for (void *__iptr = __mstart;
		n_blocks > __m_blocks && __iptr < __mend;
		__iptr += __mblkt_iter(__isize)) {

		const bool __ifree = __mblk_get_free(__iptr);
		const char __iblk_status = (libm_mtrace_f_all & flags)
								? (__ifree ? 'f' : '-') : ' ';

		__isize = __mblk_get_size(__iptr);

		if ((libm_mtrace_f_not_freed & flags || libm_mtrace_f_all & flags)
		&& !__ifree) {
			printf("%4lu: %-12p%7lu%c\n", __m_blocks++ + 1UL,
				__iptr + __mblkt_size, __isize, __iblk_status);
			__not_free_size += __isize;
		}
		else if ((libm_mtrace_f_freed & flags || libm_mtrace_f_all & flags)
		&& __ifree) {
			printf("%4lu: %-12p%7lu%c\n", __m_blocks++ + 1UL,
				__iptr + __mblkt_size, __isize, __iblk_status);
			__free_size += __isize;
		}
	}

	if (libm_mtrace_f_total & flags) {
		printf("----------------\nTotal:\n"
			"   blocks: %lu\n", __m_blocks);
		if (libm_mtrace_f_all & flags) {
			printf("   to use: %lu\n", __free_size + __not_free_size);
			printf("allocated: %zu\n", mhsize());
		}
		if (libm_mtrace_f_not_freed & flags || libm_mtrace_f_all & flags)
			printf("not freed: %lu-\n", __not_free_size);
		if (libm_mtrace_f_freed & flags || libm_mtrace_f_all & flags)
			printf("    freed: %luf\n", __free_size);
	}

	printf("\n");
	return __m_blocks;
}
