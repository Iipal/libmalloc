#ifndef LIBM_CDEFS_H
# define LIBM_CDEFS_H

# ifndef LIBMALLOC_H
#  error "include only libmalloc.h"
# endif

# include <features.h>

# if defined (__THROW)
#  define __libm_attr_THROW __THROW
# else
#  define __libm_attr_THROW /* ignored */
# endif

# if defined (__attribute_alloc_size__)
#  define __libm_attr_alloc_size(param) __attribute_alloc_size__(param)
# else
#  define __libm_attr_alloc_size(param) /* ignored */
# endif

# if defined (__attribute_malloc__)
#  define __libm_attr_malloc __attribute_malloc__
# else
#  define __libm_attr_malloc /* ignored */
# endif

# if defined (__attribute_warn_unused_result__)
#  define __libm_attr_wur __attribute_warn_unused_result__
# elif defined (__wur)
#  define __libm_attr_wur _wur
# else
#  define __libm_attr_wur /* ignored */
# endif

#endif /* LIBM_CDEFS_H */
