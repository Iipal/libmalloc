#define _GNU_SOURCE

#include "libmalloc.h"

#define LIBM_INTERNAL
#include "libm_internal.h"
#undef LIBM_INTERNAL

inline void	*calloc(size_t nmemb, size_t size) {
	void *restrict	out = malloc(nmemb * size);

	return out ? _mbzero(out, nmemb * size) : out;
}
