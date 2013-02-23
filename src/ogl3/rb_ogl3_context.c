#include "ogl3/rb_ogl3_context.h"
#include "rb.h"
#include <snlsys/mem_allocator.h>
#include <snlsys/snlsys.h>
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

