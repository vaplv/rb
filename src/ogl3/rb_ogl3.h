#ifndef RB_OGL3_H
#define RB_OGL3_H

#include "rb_types.h"
#include <snlsys/snlsys.h>
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

