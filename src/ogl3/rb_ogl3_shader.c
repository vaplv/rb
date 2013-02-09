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
#include "ogl3/rb_ogl3.h"
#include "ogl3/rb_ogl3_context.h"
#include "ogl3/rb_ogl3_shader.h"
#include "rb.h"
#include <sys/mem_allocator.h>
#include <sys/sys.h>
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
      assert(0);
      break;
  }
  return ogl3_type;
}

static void
release_shader(struct ref* ref)
{
  struct rb_context* ctxt = NULL;
  struct rb_shader* shader = NULL;
  assert(ref);

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
   int length,
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
rb_shader_source(struct rb_shader* shader, const char* source, int length)
{
  int err = 0;
  GLint status = GL_TRUE;

  if(!shader || (length > 0 && !source))
    goto error;

  OGL(ShaderSource(shader->name, 1, (const char**)&source, &length));
  OGL(CompileShader(shader->name));
  OGL(GetShaderiv(shader->name, GL_COMPILE_STATUS, &status));

  if(status == GL_FALSE) {
    int log_length = 0;
    OGL(GetShaderiv(shader->name, GL_INFO_LOG_LENGTH, &log_length));

    shader->log = MEM_REALLOC
      (shader->ctxt->allocator, shader->log, log_length*sizeof(char));
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

