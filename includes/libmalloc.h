#ifndef LIBMALLOC_H
# define LIBMALLOC_H

# include <stddef.h>
# include "libm_cdefs.h"

extern void	*malloc(size_t size)
	__libm_attr_THROW
	__libm_attr_alloc_size((1))
	__libm_attr_malloc
	__libm_attr_wur;
extern void	*calloc(size_t nmemb, size_t size)
	__libm_attr_THROW
	__libm_attr_alloc_size((1, 2))
	__libm_attr_malloc
	__libm_attr_wur;
extern void	*realloc(void *ptr, size_t size)
	__libm_attr_THROW
	__libm_attr_alloc_size((2))
	__libm_attr_wur;
extern void	free(void *ptr)
	__libm_attr_THROW;

# ifdef __USE_MISC

#  define _mtrace_defined 1
#  define _mhsize_defined 1
#  define _mptrsize_defined 1

/*
	Simple malloc information tracer.

	\param n_blocks is a limit to print\counter allocated blocks.

	\param flags is a bits mask,
	 so next code is allowed and will work correct:
		`mtrace(MTRACE_NALL, MTRACE_QUITE | MTRACE_ALL);`
	 If `MTRACE_NONE` is set in any way - all other flags will be ignored.

	\return a counter of allocated blocks
	 and not greater than \param n_blocks.
*/
extern size_t	mtrace(size_t n_blocks, int flags)
	__libm_attr_THROW
	__libm_attr_wur;

#  undef MTRACE_N_ALL

#  undef MTRACE_NONE
#  undef MTRACE_NOT_FREED
#  undef MTRACE_FREED
#  undef MTRACE_ALL
#  undef MTRACE_TOTAL
#  undef MTRACE_TOTAL_ALL
#  undef MTRACE_QUITE
#  undef MTRACE_QUITE_ALL

/*
	only(!) for \param n_blocks to count all allocated blocks.
*/
#  define MTRACE_N_ALL       ~((size_t)0)

/*
	doesn't print anything. just will count allocated blocks.
*/
#  define MTRACE_NONE      (1 << 0)

/*
	print info only about not freed blocks. (marked as '-')
*/
#  define MTRACE_NOT_FREED (1 << 1)

/*
	print info only about freed blocks. (marked as 'f')
*/
#  define MTRACE_FREED     (1 << 2)

// alias for `MTRACE_FREED | MTRACE_NOT_FREED`.
#  define MTRACE_ALL       (MTRACE_FREED | MTRACE_NOT_FREED)

/*
	print additional("total") info about blocks.

	behavior:
	 only `MTRACE_TOTAL` - will print only count of allocated blocks
	 `MTRACE_TOTAL | MTRACE_FREED` - also print info about freed blocks.
	 `MTRACE_TOTAL | MTRACE_NOT_FREED` - also print info about not freed blocks.
	 `MTRACE_TOTAL | MTRACE_ALL` - also print additional summary allocated memory blocks info.
*/
#  define MTRACE_TOTAL     (1 << 3)

/*
	alias for `MTRACE_TOTAL | MTRACE_ALL`.
*/
#  define MTRACE_TOTAL_ALL  (MTRACE_TOTAL | MTRACE_ALL)

/*
	alias for `MTRACE_TOTAL | MTRACE_NONE`.

	it will not print any information for each block as `MTRACE_NONE`,
	 but will print total as `MTRACE_TOTAL`
	 with any corresponding flag is set.
*/
#  define MTRACE_QUITE     (MTRACE_TOTAL | MTRACE_NONE)

/*
	alias for `MTRACE_QUITE | MTRACE_ALL`.
*/
#  define MTRACE_QUITE_ALL (MTRACE_QUITE | MTRACE_ALL)

/*
	\return size of summary allocated memory on heap.
*/
extern size_t	mhsize(void)
	__libm_attr_THROW
	__libm_attr_wur;

/*
	\return a size of allocated memory able to use for \param ptr.
*/
extern size_t	mptrsize(void *ptr)
	__libm_attr_THROW
	__libm_attr_wur;

# endif

#endif
