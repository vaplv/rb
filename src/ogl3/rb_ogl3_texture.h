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
#ifndef RB_OGL3_TEXTURE_H
#define RB_OGL3_TEXTURE_H

#include "ogl3/rb_ogl3.h"
#include <sys/ref_count.h>
#include <sys/sys.h>

struct rb_context;
struct rb_buffer;
struct mip_level {
  size_t pixbuf_offset;
  unsigned int width;
  unsigned int height;
};

struct rb_tex2d {
  struct ref ref;
  struct rb_context* ctxt;
  struct rb_buffer* pixbuf;
  struct mip_level* mip_list;
  unsigned int mip_count;
  GLenum format;
  GLenum internal_format;
  GLenum type;
  GLuint name;
};

LOCAL_SYM size_t
rb_ogl3_sizeof_pixel
  (GLenum format, 
   GLenum type);

LOCAL_SYM int
rb_ogl3_is_uint_type
  (GLenum type);

LOCAL_SYM int
rb_ogl3_is_int_type
  (GLenum type);

#endif /* RB_OGL3_TEXTURE_H */
