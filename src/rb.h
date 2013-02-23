#ifndef RB_H
#define RB_H

#include "rb_types.h"
#include <snlsys/snlsys.h>

#ifndef NDEBUG
  #define RB(func) ASSERT(0 == rb_##func)
#else
  #define RB(func) rb_##func
#endif

#if defined(RB_SHARED_BUILD)
  #define RB_API EXPORT_SYM
#else
  #define RB_API IMPORT_SYM
#endif

/* Define the prototypes of the render backend functions. The generated
 * prototypes are prefixed by rb. The returned type is an integer. */
#define RB_FUNC(func_name, ...) \
  RB_API int rb_##func_name(__VA_ARGS__);

#include "rb_func.h"

#undef RB_FUNC
#endif /* RB_H */

