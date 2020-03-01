#include "libmalloc.h"

#define LIBM_INTERNAL
#include "libm_internal.h"
#undef LIBM_INTERNAL

void	*realloc(void *ptr, size_t size) {
	if (!ptr) {
		return malloc(size);
	} else if (!!ptr && !size) {
		free(ptr);
		return NULL;
	}

	const size_t	__ptrsize = __mblk_get_size((ptrdiff_t)ptr - __mblkt_size);

	if (__ptrsize >= __mblk_align_size(size)) {
		return ptr;
	} else {
		void	*out = malloc(size);

		if (out) {
			memcpy(out, ptr, __ptrsize);
			free(ptr);
		}
		return out;
	}
}
