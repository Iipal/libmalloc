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

#  undef MTRACE_NALL

#  undef MTRACE_FNOT_FREED
#  undef MTRACE_FFREED
#  undef MTRACE_FALL
#  undef MTRACE_FTOTAL
#  undef MTRACE_FNONE

// put it at 'mtrace()' first argument to count all available blocks.
#  define MTRACE_NALL       ~((size_t)0)

// print info only about not freed blocks
#  define MTRACE_FNOT_FREED (1 << 0)

// print info only about freed blocks.
#  define MTRACE_FFREED     (1 << 1)

// print both info of the freed and not freed blocks.
#  define MTRACE_FALL       (1 << 2)

// print total allocated size.
#  define MTRACE_FTOTAL     (1 << 3)

// doesn't print anything. just will count allocated blocks.
#  define MTRACE_FNONE      (1 << 4)

/*
	Simple malloc trace log.

	\param n_blocks is a limit to print\counter allocated blocks.

	\param flags is a bits mask,
	 so the next code is allowed and will work correct:
		`mtrace(MTRACE_NALL, MTRACE_FTOTAL | MTRACE_FNOT_FREED);`
	 If `MTRACE_f_none` is set at any way - all other bits will be ignored.

	\return value is a counter of allocated blocks
	 and not greater than \param n_blocks.
*/
extern size_t	mtrace(size_t n_blocks, int flags);

//	\return value how much memory on heap are allocated.
extern size_t	mhsize(void);

# endif

#endif
