#ifndef LIBMALLOC_H
# define LIBMALLOC_H

# include <stddef.h>
# include <features.h>

extern void	*malloc(size_t size);
extern void	free(void *restrict ptr);
extern void	*calloc(size_t nmemb, size_t size);
extern void	*realloc(void *ptr, size_t size);

# ifdef __USE_MISC

# define libm_mtrace_n_all      ((size_t)~0UL)

# define libm_mtrace_f_not_freed 1
# define libm_mtrace_f_freed     2
# define libm_mtrace_f_all       4
# define libm_mtrace_f_none      0

/*
	\param n_blocks is a limit to print\counter allocated blocks.
		\param flags specification:
			libm_mtrace_nall(size_t max value) -
				put it in for count all available blocks.
			*your_value* -

	If \param flags are non-zero value - debug info will be printed.
		\param flags specification:
			libm_mtrace_not_freed(1) - print info only about not freed blocks.
			libm_mtrace_freed(2)     - print info only about freed blocks.
			libm_mtrace_all(4)       - print both of the above info.
			libm_mtrace_none(0)      - doesn't print anything.
				Just will count allocated blocks.

	\return value is a counter of allocated blocks
	 and not greater than \param n_blocks.
*/
extern size_t	mtrace(size_t n_blocks, int flags);

#endif

#endif
