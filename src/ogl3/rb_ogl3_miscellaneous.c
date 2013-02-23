#include "ogl3/rb_ogl3.h"
#include "ogl3/rb_ogl3_context.h"
#include "rb.h"
#include <snlsys/snlsys.h>
#include <stdlib.h>
#include <string.h>

static const GLenum rb_to_ogl3_primitive_type[] = {
  [RB_LINES] = GL_LINES,
  [RB_LINE_LOOP] = GL_LINE_LOOP,
  [RB_TRIANGLE_LIST] = GL_TRIANGLES,
  [RB_TRIANGLE_STRIP] = GL_TRIANGLE_STRIP
};

static const GLenum rb_to_ogl3_blend_func[] = {
  [RB_BLEND_ZERO] = GL_ZERO,
  [RB_BLEND_ONE] = GL_ONE,
  [RB_BLEND_SRC_COLOR] = GL_SRC_COLOR,
  [RB_BLEND_ONE_MINUS_SRC_COLOR] = GL_ONE_MINUS_SRC_COLOR,
  [RB_BLEND_DST_COLOR] = GL_DST_COLOR,
  [RB_BLEND_ONE_MINUS_DST_COLOR] = GL_ONE_MINUS_DST_COLOR,
  [RB_BLEND_SRC_ALPHA] = GL_SRC_ALPHA,
  [RB_BLEND_ONE_MINUS_SRC_ALPHA] = GL_ONE_MINUS_SRC_ALPHA,
  [RB_BLEND_DST_ALPHA] = GL_DST_ALPHA,
  [RB_BLEND_ONE_MINUS_DST_ALPHA] = GL_ONE_MINUS_DST_ALPHA,
  [RB_BLEND_CONSTANT] = GL_CONSTANT_COLOR
};

static const GLenum rb_to_ogl3_blend_op[] = {
  [RB_BLEND_OP_ADD] = GL_FUNC_ADD,
  [RB_BLEND_OP_SUB] = GL_FUNC_SUBTRACT,
  [RB_BLEND_OP_REVERSE_SUB] = GL_FUNC_REVERSE_SUBTRACT,
  [RB_BLEND_OP_MIN] = GL_MIN,
  [RB_BLEND_OP_MAX] = GL_MAX
};

static const GLenum rb_to_ogl3_comparison[] = {
  [RB_COMPARISON_NEVER] = GL_NEVER,
  [RB_COMPARISON_ALWAYS] = GL_ALWAYS,
  [RB_COMPARISON_LESS] = GL_LESS,
  [RB_COMPARISON_EQUAL] = GL_EQUAL,
  [RB_COMPARISON_NOT_EQUAL] = GL_NOTEQUAL,
  [RB_COMPARISON_LESS_EQUAL] = GL_LEQUAL,
  [RB_COMPARISON_GREATER] = GL_GREATER,
  [RB_COMPARISON_GREATER_EQUAL] = GL_GEQUAL
};

static const GLenum rb_to_ogl3_stencil_op[] = {
  [RB_STENCIL_OP_KEEP] = GL_KEEP,
  [RB_STENCIL_OP_ZERO] = GL_ZERO,
  [RB_STENCIL_OP_REPLACE] = GL_REPLACE,
  [RB_STENCIL_OP_INCR_SAT] = GL_INCR,
  [RB_STENCIL_OP_DECR_SAT] = GL_DECR,
  [RB_STENCIL_OP_INCR] = GL_INCR_WRAP,
  [RB_STENCIL_OP_DECR] = GL_DECR_WRAP,
  [RB_STENCIL_OP_INVERT] = GL_INVERT
};

static const GLenum rb_to_ogl3_fill_mode[] = {
  [RB_FILL_WIREFRAME] = GL_LINE,
  [RB_FILL_SOLID] = GL_FILL
};

static const GLenum rb_to_ogl3_cull_mode[] = {
  [RB_CULL_FRONT] = GL_FRONT,
  [RB_CULL_BACK] = GL_BACK,
  /* Invalid mode. Set a value only to be consistent. May be never used. */
  [RB_CULL_NONE] = GL_NONE
};

static const GLenum rb_to_ogl3_face_orientation[] = {
  [RB_ORIENTATION_CW] = GL_CW,
  [RB_ORIENTATION_CCW] = GL_CCW
};

int
rb_draw_indexed
  (struct rb_context* ctxt,
   enum rb_primitive_type prim_type,
   unsigned int count)
{
  if(!ctxt)
    return -1;
  OGL(DrawElements
    (rb_to_ogl3_primitive_type[prim_type], count, GL_UNSIGNED_INT, NULL));
  return 0;
}

int
rb_draw
  (struct rb_context* ctxt,
   enum rb_primitive_type prim_type,
   unsigned int count)
{
  if(!ctxt)
    return -1;
  OGL(DrawArrays(rb_to_ogl3_primitive_type[prim_type], 0, count));
  return 0;
}

int
rb_clear
  (struct rb_context* ctxt,
   int flag,
   const float color[4],
   float depth,
   char stencil)
{
  GLbitfield clear_flag = 0;

  if(!ctxt)
    return -1;

  if((flag & RB_CLEAR_COLOR_BIT) != 0) {
    if(!color)
      return -1;
    clear_flag |= GL_COLOR_BUFFER_BIT;
    OGL(ClearColor(color[0], color[1], color[2], color[3]));
  }
  if((flag & RB_CLEAR_DEPTH_BIT) != 0) {
    clear_flag |= GL_DEPTH_BUFFER_BIT;
    OGL(ClearDepth(depth));
  }
  if((flag & RB_CLEAR_STENCIL_BIT) != 0) {
    clear_flag |= GL_STENCIL_BUFFER_BIT;
    OGL(ClearStencil(stencil));
  }
  OGL(Clear(clear_flag));

  return 0;
}

int
rb_flush(struct rb_context* ctxt)
{
  if(!ctxt)
    return -1;

  OGL(Flush());
  return 0;
}

int
rb_viewport(struct rb_context* ctxt, const struct rb_viewport_desc* vp)
{
  if(!ctxt || !vp)
    return -1;

  OGL(Viewport(vp->x, vp->y, vp->width, vp->height));
  OGL(DepthRange(vp->min_depth, vp->max_depth));
  return 0;
}

int
rb_blend(struct rb_context* ctxt, const struct rb_blend_desc* blend)
{
  if(!ctxt || !blend)
    return -1;

  if(blend->enable == 0) {
    OGL(Disable(GL_BLEND));
  } else {
    OGL(Enable(GL_BLEND));
    OGL(BlendFuncSeparate
        (rb_to_ogl3_blend_func[blend->src_blend_RGB],
         rb_to_ogl3_blend_func[blend->dst_blend_RGB],
         rb_to_ogl3_blend_func[blend->src_blend_Alpha],
         rb_to_ogl3_blend_func[blend->dst_blend_Alpha]));
    OGL(BlendEquationSeparate
        (rb_to_ogl3_blend_op[blend->blend_op_RGB],
         rb_to_ogl3_blend_op[blend->blend_op_Alpha]));
  }
  return 0;
}

int
rb_depth_stencil
  (struct rb_context* ctxt, const struct rb_depth_stencil_desc* desc)
{
  if(!ctxt || !desc)
    return -1;

  OGL(DepthMask(desc->enable_depth_write ? GL_TRUE : GL_FALSE));
  if(desc->enable_depth_test == 0) {
    OGL(Disable(GL_DEPTH_TEST));
  } else {
    OGL(Enable(GL_DEPTH_TEST));
    OGL(DepthFunc(rb_to_ogl3_comparison[desc->depth_func]));
  }

  OGL(StencilMaskSeparate(GL_FRONT, desc->front_face_op.write_mask));
  OGL(StencilMaskSeparate(GL_BACK, desc->back_face_op.write_mask));
  if(desc->enable_stencil_test == 0) {
    OGL(Disable(GL_STENCIL_TEST));
  } else {
    OGL(Enable(GL_STENCIL_TEST));
    OGL(StencilOpSeparate
        (GL_FRONT,
         rb_to_ogl3_stencil_op[desc->front_face_op.stencil_fail],
         rb_to_ogl3_stencil_op[desc->front_face_op.depth_fail],
         rb_to_ogl3_stencil_op[desc->front_face_op.depth_pass]));
    OGL(StencilOpSeparate
        (GL_BACK,
         rb_to_ogl3_stencil_op[desc->back_face_op.stencil_fail],
         rb_to_ogl3_stencil_op[desc->back_face_op.depth_fail],
         rb_to_ogl3_stencil_op[desc->back_face_op.depth_pass]));
    OGL(StencilFuncSeparate
        (GL_FRONT,
         rb_to_ogl3_comparison[desc->front_face_op.stencil_func],
         desc->stencil_ref,
         ~0));
    OGL(StencilFuncSeparate
        (GL_BACK,
         rb_to_ogl3_comparison[desc->back_face_op.stencil_func],
         desc->stencil_ref,
         ~0));
  }
  return 0;
}

int
rb_rasterizer(struct rb_context* ctxt, const struct rb_rasterizer_desc* desc)
{
  if(!ctxt || !desc)
    return -1;

  if(desc->cull_mode == RB_CULL_NONE) {
    OGL(Disable(GL_CULL_FACE));
  } else {
    OGL(Enable(GL_CULL_FACE));
    OGL(CullFace(rb_to_ogl3_cull_mode[desc->cull_mode]));
  }

  OGL(PolygonMode(GL_FRONT_AND_BACK, rb_to_ogl3_fill_mode[desc->fill_mode]));
  OGL(FrontFace(rb_to_ogl3_face_orientation[desc->front_facing]));
  return 0;
}

int
rb_get_config(struct rb_context* ctxt, struct rb_config* config)
{
  if(!ctxt || !config)
    return -1;
  memcpy(config, &ctxt->config, sizeof(struct rb_config));
  return 0;
}

