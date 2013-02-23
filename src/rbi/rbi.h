#ifndef RBI_H
#define RBI_H

#include "rb_types.h"
#include <snlsys/snlsys.h>

#if defined(RBI_SHARED_BUILD)
  #define RBI_API EXPORT_SYM
#else
  #define RBI_API IMPORT_SYM
#endif

/* Define the render backend interface data structure; list of function
 * pointers toward a given implementation of the render backed functions. */
struct rbi {
  #define RB_FUNC(func_name, ...) int (*func_name)(__VA_ARGS__);
  #include "rb_func.h"
  #undef RB_FUNC

  /* For internal use only. */
  void* handle;
};

#ifndef NDEBUG
  #define RBI(rbi, func) ASSERT(0 == (rbi)->func)
#else
  #define RBI(rbi, func) ((rbi)->func)
#endif

#ifdef __cplusplus
extern "C" {
#endif

RBI_API int rbi_init(const char* library, struct rbi*);
RBI_API int rbi_shutdown(struct rbi*);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* RBI_H */

