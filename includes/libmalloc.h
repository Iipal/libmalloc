#ifndef LIBMALLOC_H
# define LIBMALLOC_H

# define _GNU_SOURCE

# include <sys/types.h>

extern void	*malloc(size_t size);
extern void	free(void *restrict ptr);

#endif
