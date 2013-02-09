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
#ifndef RB_OGL3_CONTEXT_H
#define RB_OGL3_CONTEXT_H

#include "ogl3/rb_ogl3.h"
#include <sys/ref_count.h>
#include <GL/gl.h>

struct mem_allocator;

struct rb_context {
  struct ref ref;
  struct mem_allocator* allocator;
  struct rb_config config;
  /* Basic state cache. */
  struct state_cache {
    GLuint buffer_binding[RB_OGL3_NB_BUFFER_TARGETS];
    GLuint current_program;
    GLuint framebuffer_binding;
    GLuint sampler_binding[RB_OGL3_MAX_TEXTURE_UNITS];
    GLuint texture_binding_2d[RB_OGL3_MAX_TEXTURE_UNITS];
    GLuint vertex_array_binding;
    GLenum active_texture;
  } state_cache;
};

#endif /* RB_OGL3_CONTEXT_H */

