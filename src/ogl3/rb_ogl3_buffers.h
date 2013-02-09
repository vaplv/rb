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
#ifndef RB_OGL3_BUFFERS_H
#define RB_OGL3_BUFFERS_H

#include "ogl3/rb_ogl3.h"
#include "rb_types.h"
#include <sys/ref_count.h>
#include <sys/sys.h>
#include <GL/gl.h>

struct rb_context;

struct rb_buffer {
  struct ref ref;
  struct rb_context* ctxt;
  GLuint name;
  GLenum target;
  GLenum usage;
  GLsizei size;
  enum rb_ogl3_buffer_target binding; /* used to indexed the state cache. */
};

struct rb_ogl3_buffer_desc {
  size_t size;
  enum rb_ogl3_buffer_target target;
  enum rb_usage usage;
};

LOCAL_SYM int
rb_ogl3_create_buffer
  (struct rb_context* ctxt, 
   const struct rb_ogl3_buffer_desc* desc, 
   const void* init_data, 
   struct rb_buffer** buffer);

LOCAL_SYM int
rb_ogl3_bind_buffer
  (struct rb_context* ctxt,
   struct rb_buffer* buffer, 
   enum rb_ogl3_buffer_target target);

#endif /* RB_OGL3_BUFFERS_H */

