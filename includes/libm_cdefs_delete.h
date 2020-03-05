#ifndef LIBM_CDEFS_DELETE_H
# define LIBM_CDEFS_DELETE_H

# ifndef LIBMALLOC_H
#  error "include only libmalloc.h"
# endif

# ifdef __libm_attr_THROW
#  undef __libm_attr_THROW
# endif

# ifdef __libm_attr_alloc_size
#  undef __libm_attr_alloc_size
# endif

# ifdef __libm_attr_malloc
#  undef __libm_attr_malloc
# endif

# ifdef __libm_attr_wur
#  undef __libm_attr_wur
# endif

#endif /* LIBM_CDEFS_DELETE_H */
