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
#include "ogl3/rb_ogl3_program.h"
#include "rb.h"
#include <sys/mem_allocator.h>
#include <sys/ref_count.h>
#include <sys/sys.h>
#include <stdlib.h>
#include <string.h>

struct rb_attrib {
  struct ref ref;
  struct rb_context* ctxt;
  GLuint index;
  GLenum type;
  struct rb_program* program;
  char* name;
  void (*set)(GLuint, const void* data);
};

/*******************************************************************************
 *
 * Helper functions.
 *
 ******************************************************************************/
#define ATTRIB_VALUE(suffix)\
  static void\
  attrib_##suffix(GLuint index, const void* data)\
  {\
    OGL(VertexAttrib##suffix(index, data));\
  }\

ATTRIB_VALUE(1fv)
ATTRIB_VALUE(2fv)
ATTRIB_VALUE(3fv)
ATTRIB_VALUE(4fv)

static void
(*get_attrib_setter(GLenum attrib_type))(GLuint, const void*)
{
  switch(attrib_type) {
    case GL_FLOAT: return &attrib_1fv;
    case GL_FLOAT_VEC2: return &attrib_2fv;
    case GL_FLOAT_VEC3: return &attrib_3fv;
    case GL_FLOAT_VEC4: return &attrib_4fv;
    default: return NULL;
  }
}

static int
get_active_attrib
  (struct rb_context* ctxt,
   struct rb_program* program,
   GLuint index,
   GLsizei bufsize,
   GLchar* buffer,
   struct rb_attrib** out_attrib)
{
  struct rb_attrib* attr = NULL;
  GLsizei attr_namelen = 0;
  GLint attr_size = 0;
  GLenum attr_type;
  int err = 0;

  if(!ctxt
  || !program
  || !out_attrib
  || bufsize < 0
  || (bufsize > 0 && !buffer))
    goto error;

  OGL(GetActiveAttrib
    (program->name,
     index,
     bufsize,
     &attr_namelen,
     &attr_size,
     &attr_type,
     buffer));

  attr = MEM_CALLOC(ctxt->allocator, 1, sizeof(struct rb_attrib));
  if(!attr)
    goto error;
  ref_init(&attr->ref);
  RB(context_ref_get(ctxt));
  attr->ctxt = ctxt;
  RB(program_ref_get(program));
  attr->program = program;
  attr->type = attr_type;
  attr->set = get_attrib_setter(attr_type);

  if(buffer) {
    /* Add 1 to namelen <=> include the null character. */
    ++attr_namelen;

    attr->name = MEM_ALLOC(ctxt->allocator, sizeof(char) * attr_namelen);
    if(!attr->name)
      goto error;
    attr->name = strncpy(attr->name, buffer, attr_namelen);
    attr->index = OGL(GetAttribLocation(program->name, attr->name));
  }

exit:
  if(out_attrib)
    *out_attrib = attr;
  return err;

error:
  if(attr) {
    RB(attrib_ref_put(attr));
    attr = NULL;
  }
  err = -1;
  goto exit;
}

static void
release_attrib(struct ref* ref)
{
  struct rb_attrib* attr = NULL;
  struct rb_context* ctxt = NULL;
  ASSERT(ref);

  attr = CONTAINER_OF(ref, struct rb_attrib, ref);
  ctxt = attr->ctxt;

  if(attr->program)
    RB(program_ref_put(attr->program));
  if(attr->name)
    MEM_FREE(ctxt->allocator, attr->name);
  MEM_FREE(ctxt->allocator, attr);
  RB(context_ref_put(ctxt));
}

/*******************************************************************************
 *
 * Attrib implementation.
 *
 ******************************************************************************/
int
rb_get_attribs
  (struct rb_context* ctxt,
   struct rb_program* prog,
   size_t* out_nb_attribs,
   struct rb_attrib* dst_attrib_list[])
{
  GLchar* attr_buffer = NULL;
  int attr_buflen = 0;
  int nb_attribs = 0;
  int attr_id = 0;
  int err = 0;

  if(!ctxt || !prog || !out_nb_attribs)
    goto error;

  if(!prog->is_linked)
    goto error;

  OGL(GetProgramiv(prog->name, GL_ACTIVE_ATTRIBUTES, &nb_attribs));
  ASSERT(nb_attribs >= 0);

  if(dst_attrib_list) {
    OGL(GetProgramiv(prog->name, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &attr_buflen));
    attr_buffer = MEM_ALLOC(ctxt->allocator, sizeof(GLchar) * attr_buflen);
    if(!attr_buffer)
      goto error;

    for(attr_id = 0; attr_id < nb_attribs; ++attr_id) {
      struct rb_attrib* attr = NULL;

      err = get_active_attrib
        (ctxt, prog, attr_id, attr_buflen, attr_buffer, &attr);
      if(err != 0)
        goto error;

      dst_attrib_list[attr_id] = attr;
    }
  }

exit:
  if(attr_buffer)
    MEM_FREE(ctxt->allocator, attr_buffer);
  if(out_nb_attribs)
    *out_nb_attribs = nb_attribs;
  return err;

error:
  if(dst_attrib_list) {
    int i = 0;
    /* NOTE: attr_id <=> nb attribs in dst_attrib_list; */
    for(i = 0; i < attr_id; ++i) {
      RB(attrib_ref_put(dst_attrib_list[i]));
      dst_attrib_list[i] = NULL;
    }
  }
  nb_attribs = 0;
  err = -1;
  goto exit;
}

int
rb_get_named_attrib
  (struct rb_context* ctxt,
   struct rb_program* prog,
   const char* name,
   struct rb_attrib** out_attrib)
{
  struct rb_attrib* attr = NULL;
  GLint attr_id = 0;
  size_t name_len = 0;
  int err = 0;

  if(!ctxt || !prog || !name || !out_attrib)
    goto error;

  if(!prog->is_linked)
    goto error;

  attr_id = OGL(GetAttribLocation(prog->name, name));
  if(attr_id == -1)
    goto error;

  err = get_active_attrib(ctxt, prog, attr_id, 0, NULL, &attr);
  if(err != 0)
    goto error;

  name_len = strlen(name) + 1;
  attr->name = MEM_ALLOC(ctxt->allocator, sizeof(char) * name_len);
  if(!attr->name)
    goto error;

  attr->name = strncpy(attr->name, name, name_len);
  attr->index = OGL(GetAttribLocation(prog->name, attr->name));

exit:
  *out_attrib = attr;
  return err;

error:
  if(attr) {
    RB(attrib_ref_put(attr));
    attr = NULL;
  }
  err = -1;
  goto exit;
}


int
rb_attrib_ref_get(struct rb_attrib* attr)
{
  if(!attr)
    return -1;
  ref_get(&attr->ref);
  return 0;
}

int
rb_attrib_ref_put(struct rb_attrib* attr)
{
  if(!attr)
    return -1;
  ref_put(&attr->ref, release_attrib);
  return 0;
}

int
rb_attrib_data(struct rb_attrib* attr, const void* data)
{
  if(!attr || !data)
    return -1;

  ASSERT(attr->set != NULL);
  OGL(UseProgram(attr->program->name));
  attr->set(attr->index, data);
  OGL(UseProgram(attr->ctxt->state_cache.current_program));
  return 0;
}

int
rb_get_attrib_desc(const struct rb_attrib* attr, struct rb_attrib_desc* desc)
{
  if(!attr || !desc)
    return -1;

  desc->name = attr->name;
  desc->index = attr->index;
  desc->type = ogl3_to_rb_type(attr->type);
  return 0;
}

