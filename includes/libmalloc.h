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

#endif
