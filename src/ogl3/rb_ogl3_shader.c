#include "ogl3/rb_ogl3.h"
#include "ogl3/rb_ogl3_context.h"
#include "ogl3/rb_ogl3_shader.h"
#include "rb.h"
#include <snlsys/mem_allocator.h>
#include <snlsys/snlsys.h>
#include <stdlib.h>

/*******************************************************************************
 *
 * Helper functions.
 *
 ******************************************************************************/
static FINLINE GLenum 
rb_to_ogl3_shader_type(enum rb_shader_type type)
{
  GLenum ogl3_type = GL_NONE;
  switch(type) {
    case RB_VERTEX_SHADER:
      ogl3_type = GL_VERTEX_SHADER;
      break;
    case RB_GEOMETRY_SHADER:
      ogl3_type = GL_GEOMETRY_SHADER;
      break;
    case RB_FRAGMENT_SHADER:
      ogl3_type = GL_FRAGMENT_SHADER;
      break;
    default:
      ASSERT(0);
      break;
  }
  return ogl3_type;
}

static void
release_shader(struct ref* ref)
{
  struct rb_context* ctxt = NULL;
  struct rb_shader* shader = NULL;
  ASSERT(ref);

  shader = CONTAINER_OF(ref, struct rb_shader, ref);
  ctxt = shader->ctxt;

  if(shader->name != 0)
    OGL(DeleteShader(shader->name));
  if(shader->log)
    MEM_FREE(ctxt->allocator, shader->log);
  MEM_FREE(ctxt->allocator, shader);
  RB(context_ref_put(ctxt));
}

/*******************************************************************************
 *
 * Shader functions.
 *
 ******************************************************************************/
int
rb_create_shader
  (struct rb_context* ctxt,
   enum rb_shader_type type,
   const char* source,
   size_t length,
   struct rb_shader** out_shader)
{
  int err = 0;
  struct rb_shader* shader = NULL;

  if(!ctxt || !out_shader)
    goto error;

  shader = MEM_ALLOC(ctxt->allocator, sizeof(struct rb_shader));
  if(!shader)
    goto error;
  ref_init(&shader->ref);
  list_init(&shader->attachment);
  RB(context_ref_get(ctxt));
  shader->ctxt = ctxt;

  shader->log = NULL;
  shader->type = rb_to_ogl3_shader_type(type);
  shader->name = OGL(CreateShader(shader->type));
  if(shader->name == 0)
    goto error;

  err = rb_shader_source(shader, source, length);

exit:
  if(out_shader)
    *out_shader = shader;
  return err;

error:
  if(shader) {
    RB(shader_ref_put(shader));
    shader = NULL;
  }
  err = -1;
  goto exit;
}

int
rb_shader_source(struct rb_shader* shader, const char* source, size_t length)
{
  int err = 0;
  GLint status = GL_TRUE;
  GLint gl_length = 0;

  if(!shader || (length > 0 && !source))
    goto error;

  gl_length = (GLint)length;
  OGL(ShaderSource(shader->name, 1, (const char**)&source, &gl_length));
  OGL(CompileShader(shader->name));
  OGL(GetShaderiv(shader->name, GL_COMPILE_STATUS, &status));
  length = (size_t)gl_length;

  if(status == GL_FALSE) {
    GLint log_length = 0;
    OGL(GetShaderiv(shader->name, GL_INFO_LOG_LENGTH, &log_length));

    shader->log = MEM_REALLOC
      (shader->ctxt->allocator, shader->log, (size_t)log_length*sizeof(char));
    if(!shader->log)
      goto error;

    OGL(GetShaderInfoLog(shader->name, log_length, NULL, shader->log));
    err = -1;
#ifndef NDEBUG
   fprintf(stderr, "%s\n", shader->log);
#endif
  } else {
    MEM_FREE(shader->ctxt->allocator, shader->log);
    shader->log = NULL;
  }

exit:
  return err;

error:
  err = -1;
  goto exit;
}

int
rb_shader_ref_get(struct rb_shader* shader)
{
  if(!shader)
    return -1;
  ref_get(&shader->ref);
  return 0;
}

int
rb_shader_ref_put(struct rb_shader* shader)
{
  if(!shader)
    return -1;
  ref_put(&shader->ref, release_shader);
  return 0;
}

int
rb_get_shader_log(struct rb_shader* shader, const char** out_log)
{
  if(!shader || !out_log)
    return -1;
  *out_log = shader->log;
  return 0;
}

int
rb_is_shader_attached(struct rb_shader* shader, int* out_is_attached)
{
  if(!shader || !out_is_attached)
    return -1;
  *out_is_attached = !is_list_empty(&shader->attachment);
  return 0;
}

