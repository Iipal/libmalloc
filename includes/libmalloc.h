#ifndef LIBMALLOC_H
# define LIBMALLOC_H

# include <stddef.h>
# include <features.h>

extern void	*malloc(size_t size);
extern void	free(void *restrict ptr);
extern void	*calloc(size_t nmemb, size_t size);
extern void	*realloc(void *ptr, size_t size);

# ifdef __USE_MISC

#  define libm_mtrace_n_all       ((size_t)~0UL)

#  define libm_mtrace_f_not_freed (1 << 0)
#  define libm_mtrace_f_freed     (1 << 1)
#  define libm_mtrace_f_all       (1 << 2)
#  define libm_mtrace_f_total     (1 << 3)
#  define libm_mtrace_f_none      (1 << 4)

/*
	\param n_blocks is a limit to print\counter allocated blocks.
		\param flags specification:
			libm_mtrace_n_all(size_t max value) -
				put it in to count all available blocks.

	If \param flags are non-zero value - debug info will be printed.
		\param flags specification:
			libm_mtrace_f_not_freed - print info only about not freed blocks.
			libm_mtrace_f_freed     - print info only about freed blocks.
			libm_mtrace_f_all       - print both of the above info,
				and block status: 'f' - freed and '-' - not freed.
			libm_mtrace_f_total     - print total allocated size.
			libm_mtrace_f_none      - doesn't print anything.
				Just will count allocated blocks.

	\return value is a counter of allocated blocks
	 and not greater than \param n_blocks.
*/
extern size_t	mtrace(size_t n_blocks, int flags);

/*
	\return value how much memory on heap are allocated.
*/
extern size_t	mhsize(void);

#endif

#endif
