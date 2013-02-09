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
#include "rb.h"
#include <sys/sys.h>
#include <string.h>

#ifdef __GNUC__
  #pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
 
/* Do not use the X macro to define the rb_get_config function body. */
static int
rb_get_config__(struct rb_context* ctxt, struct rb_config* cfg);
#define get_config get_config__

/* Define NULL function body. */
#define RB_FUNC(func_name, ...) \
  int \
  CONCAT(rb_, func_name)(__VA_ARGS__) \
  { \
    return 0; \
  }

#include "rb_func.h"

#undef RB_FUNC

/* Define the specific body of the rb_get_config config function. */
int
rb_get_config(struct rb_context* ctxt, struct rb_config* cfg)
{
  rb_get_config__(NULL, NULL); /* Avoid the `unused static function' warning. */
  cfg->max_tex_max_anisotropy = SIZE_MAX;
  cfg->max_tex_size = SIZE_MAX;
  return 0;
}

