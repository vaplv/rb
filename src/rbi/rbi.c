#include "rbi.h"
#include <snlsys/snlsys.h>
#include <stdio.h>
#include <string.h>
#include <dlfcn.h>
#include <stdint.h>

static
void set_rbi_to_null(struct rbi* driver)
{
  if(driver)
    memset(driver, 0, sizeof(struct rbi));
}

int
rbi_init(const char* library, struct rbi* driver)
{
  int err = 0;
  char* err_msg = NULL;

  if(!library || !driver)
    goto error;

  set_rbi_to_null(driver);

  driver->handle = dlopen(library, RTLD_LAZY | RTLD_LOCAL);

  if(!driver->handle) {
    err_msg = dlerror();
    goto error;
  }

  #define RB_FUNC(func_name, ...)\
    driver->func_name = (int (*)(__VA_ARGS__))(intptr_t) \
      dlsym(driver->handle, "rb_"#func_name); \
    if((err_msg=dlerror())) \
      goto error;

  #include "rb_func.h"

  #undef RB_FUNC

exit:
  return err;

error:
  if(err_msg)
    fprintf(stderr, "%s\n", err_msg);

  rbi_shutdown(driver);

  err = -1;
  goto exit;
}

int
rbi_shutdown(struct rbi* driver)
{
  if(!driver)
    return -1;

  if(driver->handle)
    dlclose(driver->handle);

  set_rbi_to_null(driver);
  return 0;
}

