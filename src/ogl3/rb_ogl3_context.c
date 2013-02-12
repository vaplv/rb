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
#include "ogl3/rb_ogl3_context.h"
#include "rb.h"
#include <sys/mem_allocator.h>
#include <sys/sys.h>
#include <stdlib.h>
#include <string.h>

/*******************************************************************************
 *
 * Helper functions.
 *
 ******************************************************************************/
static void
setup_config(struct rb_config* cfg)
{
  int i = 0;
  OGL(GetIntegerv(GL_MAX_TEXTURE_SIZE, &i));
  ASSERT(i > 0);
  cfg->max_tex_size = (size_t)i;
  OGL(GetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &i));
  ASSERT(i > 0);
  cfg->max_tex_max_anisotropy = (size_t)i;
}

static void
release_context(struct ref* ref)
{
  struct rb_context* ctxt = NULL;
  ASSERT(ref);

  ctxt = CONTAINER_OF(ref, struct rb_context, ref);
  MEM_FREE(ctxt->allocator, ctxt);
}

/*******************************************************************************
 *
 * Render backend context functions.
 *
 ******************************************************************************/
int
rb_create_context
  (struct mem_allocator* specific_allocator,
   struct rb_context** out_ctxt)
{
  struct mem_allocator* allocator = NULL;
  struct rb_context* ctxt = NULL;
  int err = 0;

  if(!out_ctxt)
    goto error;

  allocator = specific_allocator ? specific_allocator : &mem_default_allocator;
  ctxt = MEM_CALLOC(allocator, 1, sizeof(struct rb_context));
  if(!ctxt)
    goto error;
  ctxt->allocator = allocator;
  ref_init(&ctxt->ref);

  #define GL_FUNC(type, func, ...)                                             \
    rbgl##func = (type (*)(__VA_ARGS__))                                       \
      RB_OGL3_GET_PROC_ADDRESS(STR(gl##func));                                 \
    if(!rbgl##func)                                                            \
      goto error;
  #include "ogl3/rb_ogl3_gl_func.h"
  #undef GL_FUNC

  setup_config(&ctxt->config);

exit:
  if(ctxt)
    *out_ctxt = ctxt;
  return err;

error:
  if(ctxt) {
    RB(context_ref_put(ctxt));
    ctxt = NULL;
  }
  err = -1;
  goto exit;
}

int
rb_context_ref_get(struct rb_context* ctxt)
{
  if(!ctxt)
    return -1;
  ref_get(&ctxt->ref);
  return 0;
}

int
rb_context_ref_put(struct rb_context* ctxt)
{
  if(!ctxt)
    return -1;
  ref_put(&ctxt->ref, release_context);
  return 0;
}

