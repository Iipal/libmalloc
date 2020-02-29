#define _GNU_SOURCE

#include "libmalloc.h"

#define LIBM_INTERNAL
#include "libm_internal.h"
#undef LIBM_INTERNAL

static inline size_t	__fast_mtrace(const size_t n_blocks) {
	size_t	__m_blocks = 0UL;

	if (mhsize()) {
		for (ptrdiff_t __iptr = (ptrdiff_t)__mstart;
			n_blocks > __m_blocks && (uintptr_t)__iptr < (uintptr_t)__mend;
			__iptr += __mblkt_iter(__mblk_get_size(__iptr)))
				++__m_blocks;
	}
	return __m_blocks;
}

size_t	mtrace(size_t n_blocks, int flags) {
	if (!flags || (MTRACE_FNONE & flags))
		return __fast_mtrace(n_blocks);

	const bool	__is_only_total = (MTRACE_FTOTAL == flags);
	size_t	__m_blocks = 0;
	size_t	__isize = 0;
	size_t	__ifree = 0;
	size_t	__freed_size = 0;
	size_t	__not_freed_size = 0;

	if (!__is_only_total)
		puts("-----------------\n   N:   Address     Size\n");
	if (!mhsize()) {
		puts("mtrace: no memory allocated yet.\n\n");
		return 0;
	}
	for (ptrdiff_t __iptr = (ptrdiff_t)__mstart;
		n_blocks > __m_blocks && (uintptr_t)__iptr < (uintptr_t)__mend;
		__iptr += __mblkt_iter(__isize), __m_blocks++) {

		__isize = __mblk_get_size(__iptr);
		__ifree = __mblk_get_free(__iptr);

		if (!__is_only_total) {
			printf("%4lu: %-12p%7zu%c\n",
				__m_blocks + 1,
				(void*)(__iptr + (ptrdiff_t)__mblkt_size),
				__isize,
				((MTRACE_FALL & flags) ? (__ifree ? 'f' : '-') : ' '));
		}

		if (__ifree) {
			__freed_size += __isize;
		} else {
			__not_freed_size += __isize;
		}
	}

	if (MTRACE_FTOTAL & flags) {
		printf("----------------\nTotal (in bytes):\n"
			"   blocks: %zu\n", __m_blocks);
		if (MTRACE_FALL & flags) {
			printf("   to use: %zub\n", __freed_size + __not_freed_size);
			printf("allocated: %zub\n", mhsize());
		}
		if ((MTRACE_FNOT_FREED & flags) || (MTRACE_FALL & flags))
			printf("not freed: %zu-\n", __not_freed_size);
		if ((MTRACE_FFREED & flags) || (MTRACE_FALL & flags))
			printf("    freed: %zuf\n", __freed_size);
	}

	printf("\n");
	return __m_blocks;
}
