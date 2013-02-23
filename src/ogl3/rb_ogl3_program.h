#ifndef RB_OGL3_PROGRAM_H
#define RB_OGL3_PROGRAM_H

#include <snlsys/list.h>
#include <snlsys/ref_count.h>
#include <GL/gl.h>

struct rb_context;

struct rb_program {
  struct ref ref;
  struct list_node attached_shader_list;
  struct rb_context* ctxt;
  GLuint name;
  int is_linked;
  char* log;
};

#endif /* RB_OGL3_PROGRAM_H */

