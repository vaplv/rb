/*
 * Copyright (c) 2013 Vincent Forest
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO
 * EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "rbi.h"
#include <sys/sys.h>
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

