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
#include "ogl3/rb_ogl3_context.h"
#include "rb.h"
#include <sys/math.h>
#include <sys/mem_allocator.h>
#include <sys/ref_count.h>
#include <sys/sys.h>
#include <assert.h>

struct rb_sampler {
  struct ref ref;
  struct rb_context* ctxt;
  GLuint name;
};

/*******************************************************************************
 *
 * Helper functions.
 *
 ******************************************************************************/
struct ogl3_tex_filter{
  GLenum min;
  GLenum mag;
};

static struct ogl3_tex_filter
rb_to_ogl3_tex_filter(enum rb_tex_filter filter)
{
  struct ogl3_tex_filter ogl3_filter = {
    .min = GL_LINEAR_MIPMAP_LINEAR,
    .mag = GL_LINEAR
  };
  switch(filter) {
    case RB_MIN_POINT_MAG_POINT_MIP_POINT:
      ogl3_filter.min = GL_NEAREST_MIPMAP_NEAREST;
      ogl3_filter.mag = GL_NEAREST;
      break;
    case RB_MIN_LINEAR_MAG_POINT_MIP_POINT:
      ogl3_filter.min = GL_LINEAR_MIPMAP_NEAREST;
      ogl3_filter.mag = GL_NEAREST;
      break;
    case RB_MIN_POINT_MAG_LINEAR_MIP_POINT:
      ogl3_filter.min = GL_NEAREST_MIPMAP_NEAREST;
      ogl3_filter.mag = GL_LINEAR;
      break;
    case RB_MIN_LINEAR_MAG_LINEAR_MIP_POINT:
      ogl3_filter.min = GL_LINEAR_MIPMAP_NEAREST;
      ogl3_filter.mag = GL_LINEAR;
      break;
    case RB_MIN_POINT_MAG_POINT_MIP_LINEAR:
      ogl3_filter.min = GL_NEAREST_MIPMAP_LINEAR;
      ogl3_filter.mag = GL_NEAREST;
      break;
    case RB_MIN_LINEAR_MAG_POINT_MIP_LINEAR:
      ogl3_filter.min = GL_LINEAR_MIPMAP_LINEAR;
      ogl3_filter.mag = GL_NEAREST;
      break;
    case RB_MIN_POINT_MAG_LINEAR_MIP_LINEAR:
      ogl3_filter.min = GL_NEAREST_MIPMAP_LINEAR;
      ogl3_filter.mag = GL_LINEAR;
      break;
    case RB_MIN_LINEAR_MAG_LINEAR_MIP_LINEAR:
      ogl3_filter.min = GL_LINEAR_MIPMAP_LINEAR;
      ogl3_filter.mag = GL_LINEAR;
      break;
    default:
      assert(0);
      break;
  }
  return ogl3_filter;
}

static GLenum
rb_to_ogl3_address(enum rb_tex_address address)
{
  GLenum ogl3_address = GL_REPEAT;
  switch(address) {
    case RB_ADDRESS_WRAP:
      ogl3_address = GL_REPEAT;
      break;
    case RB_ADDRESS_CLAMP:
      ogl3_address = GL_CLAMP_TO_EDGE;
      break;
    default:
      assert(0);
      break;
  }
  return ogl3_address;
}

static void
release_sampler(struct ref* ref)
{
  struct rb_context* ctxt = NULL;
  struct rb_sampler* sampler = NULL;
  size_t i = 0;
  assert(ref);

  sampler = CONTAINER_OF(ref, struct rb_sampler, ref);
  ctxt = sampler->ctxt;

  for(i = 0; i < RB_OGL3_MAX_TEXTURE_UNITS; ++i) {
    if(ctxt->state_cache.sampler_binding[i] == sampler->name)
      RB(bind_sampler(ctxt, NULL, i));
  }

  OGL(DeleteSamplers(1, &sampler->name));
  MEM_FREE(ctxt->allocator, sampler);
  RB(context_ref_put(ctxt));
}

/*******************************************************************************
 *
 * Sampler function.
 *
 ******************************************************************************/
int
rb_create_sampler
  (struct rb_context* ctxt,
   const struct rb_sampler_desc* desc,
   struct rb_sampler** out_sampler)
{
  struct rb_sampler* sampler = NULL;
  int err = 0;

  if(!ctxt || !desc || !out_sampler)
    goto error;

  sampler = MEM_CALLOC(ctxt->allocator, 1, sizeof(struct rb_sampler));
  if(!sampler)
    goto error;
  ref_init(&sampler->ref);
  RB(context_ref_get(ctxt));
  sampler->ctxt = ctxt;
  OGL(GenSamplers(1, &sampler->name));

  err = rb_sampler_parameters(sampler, desc);
  if(0 != err)
    goto error;

exit:
  if(out_sampler)
    *out_sampler = sampler;
  return err;
error:
  if(sampler) {
    RB(sampler_ref_put(sampler));
    sampler = NULL;
  }

  err = -1;
  goto exit;
}

int
rb_sampler_ref_get(struct rb_sampler* sampler)
{
  if(!sampler)
    return -1;
  ref_get(&sampler->ref);
  return 0;
}

int
rb_sampler_ref_put(struct rb_sampler* sampler)
{
  if(!sampler)
    return -1;
  ref_put(&sampler->ref, release_sampler);
  return 0;
}

int
rb_sampler_parameters
  (struct rb_sampler* sampler,
   const struct rb_sampler_desc* desc)
{
  struct ogl3_tex_filter filter;
  float min_lod = 0.f;
  float max_lod = 0.f;
  int max_tex_max_aniso = 0;
  int err = 0;

  if(!sampler || !desc || desc->min_lod > desc->max_lod)
    goto error;

  OGL(GetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_tex_max_aniso));
  assert(max_tex_max_aniso >= 0);
  if(desc->max_anisotropy > (size_t)max_tex_max_aniso)
    goto error;

  filter = rb_to_ogl3_tex_filter(desc->filter);
  min_lod = MAX(MIN(min_lod, -1000.f), 1000.f);
  max_lod = MAX(MIN(min_lod, -1000.f), 1000.f);

  OGL(SamplerParameteri(sampler->name, GL_TEXTURE_MIN_FILTER, filter.min));
  OGL(SamplerParameteri(sampler->name, GL_TEXTURE_MAG_FILTER, filter.mag));
  OGL(SamplerParameteri
    (sampler->name, GL_TEXTURE_WRAP_S, rb_to_ogl3_address(desc->address_u)));
  OGL(SamplerParameteri
    (sampler->name, GL_TEXTURE_WRAP_T, rb_to_ogl3_address(desc->address_v)));
  OGL(SamplerParameteri
    (sampler->name, GL_TEXTURE_WRAP_R, rb_to_ogl3_address(desc->address_w)));
  OGL(SamplerParameterf(sampler->name, GL_TEXTURE_LOD_BIAS, desc->lod_bias));
  OGL(SamplerParameterf(sampler->name, GL_TEXTURE_MIN_LOD, min_lod));
  OGL(SamplerParameterf(sampler->name, GL_TEXTURE_MAX_LOD, max_lod));
  OGL(SamplerParameterf
    (sampler->name,
     GL_TEXTURE_MAX_ANISOTROPY_EXT,
     (float)desc->max_anisotropy));

exit:
  return err;
error:
  err = -1;
  goto exit;
}

int
rb_bind_sampler
  (struct rb_context* ctxt,
   struct rb_sampler* sampler,
   unsigned int tex_unit)
{
  int err = 0;

  if(!ctxt || tex_unit > RB_OGL3_MAX_TEXTURE_UNITS)
    goto error;

  ctxt->state_cache.sampler_binding[tex_unit] = sampler ? sampler->name : 0;
  OGL(BindSampler(tex_unit, ctxt->state_cache.sampler_binding[tex_unit]));

exit:
  return err;
error:
  err = -1;
  goto exit;
}

