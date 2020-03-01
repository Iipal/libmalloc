#ifndef LIBMALLOC_H
# define LIBMALLOC_H

# include <stddef.h>
# include <features.h>

extern void	*malloc(size_t size)
	__attribute__((nothrow));
extern void	free(void *restrict ptr)
	__attribute__((nothrow));
extern void	*calloc(size_t nmemb, size_t size)
	__attribute__((nothrow));
extern void	*realloc(void *ptr, size_t size)
	__attribute__((nothrow));

# ifdef __USE_MISC

#  define _mtrace_defined 1

/*
	Simple malloc information tracer.

	\param n_blocks is a limit to print\counter allocated blocks.

	\param flags is a bits mask,
	 so next code is allowed and will work correct:
		`mtrace(MTRACE_NALL, MTRACE_FQUITE | MTRACE_FALL);`
	 If `MTRACE_F_NONE` is set in any way - all other flags will be ignored.

	\return a counter of allocated blocks
	 and not greater than \param n_blocks.
*/
extern size_t	mtrace(size_t n_blocks, int flags)
	__attribute__((nothrow));

#  undef MTRACE_NALL

#  undef MTRACE_FNONE
#  undef MTRACE_FNOT_FREED
#  undef MTRACE_FFREED
#  undef MTRACE_FALL
#  undef MTRACE_FTOTAL
#  undef MTRACE_FQUITE

/*
	only(!) for \param n_blocks to count all allocated blocks.
*/
#  define MTRACE_NALL       ~((size_t)0)

/*
	doesn't print anything. just will count allocated blocks.
*/
#  define MTRACE_FNONE      (1 << 0)

/*
	print info only about not freed blocks. (marked as '-')
*/
#  define MTRACE_FNOT_FREED (1 << 1)

/*
	print info only about freed blocks. (marked as 'f')
*/
#  define MTRACE_FFREED     (1 << 2)

// alias for `MTRACE_FFREED | MTRACE_FNOT_FREED`.
#  define MTRACE_FALL       (MTRACE_FFREED | MTRACE_FNOT_FREED)

/*
	print additional("total") info about blocks.

	behavior:
	 only `MTRACE_FTOTAL` - will print only count of allocated blocks
	 `MTRACE_FTOTAL | MTRACE_FFREED` - also print info about freed blocks.
	 `MTRACE_FTOTAL | MTRACE_FNOT_FREED` - also print info about not freed blocks.
	 `MTRACE_FTOTAL | MTRACE_FALL` - also print additional summary allocated memory blocks info.
*/
#  define MTRACE_FTOTAL     (1 << 3)

/*
	alias for `MTRACE_FTOTAL | MTRACE_FNONE`.

	it will not print any information for each block as `MTRACE_FNONE`,
	 but will print total as `MTRACE_FTOTAL`
	 with any corresponding flag is set.
*/
#  define MTRACE_FQUITE     (MTRACE_FTOTAL | MTRACE_FNONE)

#  define _mhsize_defined 1

/*
	\return size of summary allocated memory on heap.
*/
extern size_t	mhsize(void)
	__attribute__((nothrow));

# endif

#endif
