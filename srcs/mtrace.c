#define _GNU_SOURCE

#include "libmalloc.h"

#define LIBM_INTERNAL
#include "libm_internal.h"
#undef LIBM_INTERNAL

# include <stdio.h>

# define __is_bit(f, b) (((f) & (b)) == (b))

static inline size_t	__fast_mtrace(const size_t n_blocks);
static inline void	__prepare_fmt_str(size_t n_blocks, char *restrict __fmt);

size_t	mtrace(size_t n_blocks, int flags) {
	if ((__is_bit(flags, MTRACE_FNONE) && !__is_bit(flags, MTRACE_FQUITE))
	|| !flags)
		return __fast_mtrace(n_blocks);

	if (!mhsize()) {
		puts("mtrace: no memory allocated yet.\n");
		return 0;
	}

	char	__fmt_str[128] = { 0 };
	bool	__is_only_total = false;
	if ((MTRACE_FTOTAL == flags) || __is_bit(flags, MTRACE_FQUITE)) {
		__is_only_total = true;
	}

	if (!__is_only_total) {
		puts("\tmtrace:\n"
			"-------------------------\n"
			"[N]:Address - Size:Status\n");
		__prepare_fmt_str(n_blocks, __fmt_str);
	}

	size_t	__isize = 0;
	size_t	__ifree = 0;
	size_t	__iblocks = 0;
	size_t	__freed_size = 0;
	size_t	__not_freed_size = 0;

	pthread_mutex_lock(&__mmutex);
	ptrdiff_t	__iptr = (ptrdiff_t)__mstart;
	while (n_blocks > __iblocks && (uintptr_t)__iptr < (uintptr_t)__mend) {
		__isize = __mblk_get_size(__iptr);
		__ifree = __mblk_get_free(__iptr);

		if (__ifree) {
			__freed_size += __isize;
		} else {
			__not_freed_size += __isize;
		}

		if (__is_only_total) {
			goto continue_iterate;
		}

		if ((__ifree && __is_bit(flags, MTRACE_FFREED))
		|| (!__ifree && __is_bit(flags, MTRACE_FNOT_FREED))) {
			printf(__fmt_str, __iblocks,
				(void*)(__iptr + (ptrdiff_t)__mblkt_size),
				__isize, __ifree ? 'f' : '-');
		}
	continue_iterate:
		__iptr += __mblk_iter(__isize);
		++__iblocks;
	}
	pthread_mutex_unlock(&__mmutex);

	if (__is_bit(flags, MTRACE_FTOTAL)) {
		printf("-------------------------\nTotal:\n"
			"   blocks: %zu\n", __iblocks);
		if (__is_bit(flags, MTRACE_FALL)) {
			printf("   to use: %zub\n", __freed_size + __not_freed_size);
			printf("allocated: %zub\n", mhsize());
		}
		if (__is_bit(flags, MTRACE_FNOT_FREED)) {
			printf("not freed: %zu-\n", __not_freed_size);
		}
		if ( __is_bit(flags, MTRACE_FFREED)) {
			printf("    freed: %zuf\n", __freed_size);
		}
	}

	printf("\n");
	return __iblocks;
}

static inline size_t	__fast_mtrace(const size_t n_blocks) {
	size_t	__iblocks = 0UL;

	pthread_mutex_lock(&__mmutex);
	if (mhsize()) {
		ptrdiff_t	__iptr = (ptrdiff_t)__mstart;

		while ((n_blocks > __iblocks)
		&& ((uintptr_t)__iptr < (uintptr_t)__mend)) {
			__iptr += __mblk_iter(__mblk_get_size(__iptr));
			++__iblocks;
		}
	}
	pthread_mutex_unlock(&__mmutex);
	return __iblocks;
}

static inline void	__prepare_fmt_str(size_t n_blocks, char *restrict __fmt) {
	char	__tmp[24] = { 0 };
	ptrdiff_t	__iptr = (ptrdiff_t)__mstart;
	size_t	__iblocks = 0;
	size_t	__addr_len = 0;
	size_t	__size_len = 0;

	while (n_blocks-- && ((uintptr_t)__iptr < (uintptr_t)__mend)) {
		snprintf(__tmp, 24, "%zu", __mblk_get_size(__iptr));
		const size_t	__isize_len = strlen(__tmp);
		if (__isize_len > __size_len) {
			__size_len = __isize_len;
		}

		snprintf(__tmp, 24, "%p", (void*)__iptr);
		const size_t	__iaddr_len = strlen(__tmp);
		if (__iaddr_len > __addr_len) {
			__addr_len = __iaddr_len;
		}

		__iptr += __mblk_iter(__mblk_get_size(__iptr));
		++__iblocks;
	}

	snprintf(__tmp, 24, "%zu", __iblocks);
	snprintf(__fmt, 128, "[%%%zulu]:%%-%zup - %%-%zuzu:%%c\n",
		strlen(__tmp), __addr_len, __size_len);
}
