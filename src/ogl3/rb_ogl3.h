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
#ifndef RB_OGL3_H
#define RB_OGL3_H

#include "rb_types.h"
#include <sys/sys.h>
#include <GL/gl.h>
#include <GL/glext.h>

#ifndef NDEBUG
  #include <GL/glu.h>
  #include <stdio.h>
  #define OGL(func)\
    rbgl##func;                                                                \
    {                                                                          \
      GLenum gl_error = rbglGetError();                                        \
      if(gl_error != GL_NO_ERROR) {                                            \
        fprintf(stderr, "error:opengl: %s\n", gluErrorString(gl_error));       \
        ASSERT(gl_error == GL_NO_ERROR);                                       \
      }                                                                        \
    } (void) 0
#else
  #define OGL(func) rbgl##func
#endif

#ifdef PLATFORM_UNIX
  #include <GL/glx.h>
  #define RB_OGL3_GET_PROC_ADDRESS(name) \
    glXGetProcAddress((const GLubyte*)(name))
#endif

/* Define rb_ogl3 function pointers */
#define GL_FUNC(type, func, ...) type (*rbgl##func)(__VA_ARGS__);
#include "ogl3/rb_ogl3_gl_func.h"
#undef GL_FUNC

/* OpenGL 3.3 spec */
#define RB_OGL3_MAX_TEXTURE_UNITS 16 
#define RB_OGL3_MAX_COLOR_ATTACHMENTS 8

enum rb_ogl3_buffer_target {
  RB_OGL3_BIND_VERTEX_BUFFER,
  RB_OGL3_BIND_INDEX_BUFFER,
  RB_OGL3_BIND_PIXEL_READBACK_BUFFER,
  RB_OGL3_BIND_PIXEL_DOWNLOAD_BUFFER,
  RB_OGL3_NB_BUFFER_TARGETS,
};

static inline enum rb_type
ogl3_to_rb_type(GLenum attrib_type)
{
  switch(attrib_type) {
    case GL_FLOAT: return RB_FLOAT;
    case GL_FLOAT_VEC2: return RB_FLOAT2;
    case GL_FLOAT_VEC3: return RB_FLOAT3;
    case GL_FLOAT_VEC4: return RB_FLOAT4;
    case GL_FLOAT_MAT4: return RB_FLOAT4x4;
    default: return RB_UNKNOWN_TYPE;
  }
}

#endif /* RB_OGL3_H */

