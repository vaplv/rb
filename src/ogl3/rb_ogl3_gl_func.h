/*******************************************************************************
 *
 * Buffer
 *
 ******************************************************************************/
GL_FUNC(void, BindBuffer,
    GLenum target, GLuint buffer)

GL_FUNC(void, BufferData,
  GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage)

GL_FUNC(void, DeleteBuffers,
    GLsizei n, const GLuint *buffers)

GL_FUNC(void, GenBuffers,
    GLsizei n, GLuint *buffers)

GL_FUNC(GLvoid*, MapBuffer,
  GLenum target, GLenum access)

GL_FUNC(GLvoid*, MapBufferRange,
  GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access)

GL_FUNC(GLboolean, UnmapBuffer,
  GLenum target)

/*******************************************************************************
 *
 * Shader
 *
 ******************************************************************************/
GL_FUNC(void, DeleteShader,
  GLuint shader)

GL_FUNC(GLuint, CreateShader,
  GLenum type)

GL_FUNC(void, ShaderSource,
  GLuint shader, GLsizei count, const GLchar** string, const GLint *length)

GL_FUNC(void, CompileShader,
  GLuint shader)

GL_FUNC(void, GetShaderiv,
  GLuint shader, GLenum pname, GLint *params)

GL_FUNC(void, GetShaderInfoLog,
  GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog)

/*******************************************************************************
 *
 * Framebuffer
 *
 ******************************************************************************/
GL_FUNC(void, BindFramebuffer,
  GLenum target, GLuint framebuffer)

GL_FUNC(GLenum, CheckFramebufferStatus,
  GLenum target)

GL_FUNC(void, ClearBufferiv,
  GLenum buffer, GLint drawbuffer, const GLint *value)

GL_FUNC(void, ClearBufferuiv,
  GLenum buffer, GLint drawbuffer, const GLuint *value)

GL_FUNC(void, ClearBufferfv,
  GLenum buffer, GLint drawbuffer, const GLfloat *value)

GL_FUNC(void, ClearBufferfi,
  GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil)

GL_FUNC(void, DeleteFramebuffers,
  GLsizei n, const GLuint *framebuffers)

GL_FUNC(void, FramebufferTexture2D,
  GLenum target, GLenum attachment, GLenum textarget, GLuint texture,
  GLint level)

GL_FUNC(void, FramebufferTexture3D,
  GLenum target, GLenum attachment, GLenum textarget, GLuint texture,
  GLint level, GLint zoffset)

GL_FUNC(void, GenFramebuffers,
  GLsizei n, GLuint *framebuffers)

/*******************************************************************************
 *
 * Raster functions
 *
 ******************************************************************************/
GL_FUNC(void, CullFace,
  GLenum mode)

GL_FUNC(void, FrontFace,
  GLenum mode)

GL_FUNC(void, PixelStorei,
  GLenum pname, GLint param)

GL_FUNC(void, PolygonMode,
  GLenum face, GLenum mode)

GL_FUNC(void, ReadBuffer,
  GLenum mode)

GL_FUNC(void, ReadPixels,
  GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type,
  GLvoid *pixels)

/*******************************************************************************
 *
 * Program
 *
 ******************************************************************************/
GL_FUNC(void, AttachShader,
  GLuint program, GLuint shader)

GL_FUNC(GLuint, CreateProgram,
  void)

GL_FUNC(void, DeleteProgram,
  GLuint program)

GL_FUNC(void, DetachShader,
  GLuint program, GLuint shader)

GL_FUNC(void, GetActiveAttrib,
  GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size,
  GLenum *type, GLchar *name)

GL_FUNC(GLint, GetAttribLocation,
  GLuint program, const GLchar *name)

GL_FUNC(void, GetActiveUniform,
  GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size,
  GLenum *type, GLchar *name)

GL_FUNC(void, GetProgramInfoLog,
  GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog)

GL_FUNC(void, GetUniformIndices, GLuint program, GLsizei uniformCount,
  const GLchar* *uniformNames, GLuint *uniformIndices)

GL_FUNC(GLint, GetUniformLocation,
  GLuint program, const GLchar *name)

GL_FUNC(void, GetProgramiv,
  GLuint program, GLenum pname, GLint *params)

GL_FUNC(void, LinkProgram,
  GLuint program)

GL_FUNC(void, Uniform1fv,
  GLint location, GLsizei count, const GLfloat *value)

GL_FUNC(void, Uniform2fv,
  GLint location, GLsizei count, const GLfloat *value)

GL_FUNC(void, Uniform3fv,
  GLint location, GLsizei count, const GLfloat *value)

GL_FUNC(void, Uniform4fv,
  GLint location, GLsizei count, const GLfloat *value)

GL_FUNC(void, Uniform1iv,
  GLint location, GLsizei count, const GLint *value)

GL_FUNC(void, UniformMatrix2fv,
  GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)

GL_FUNC(void, UniformMatrix3fv,
  GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)

GL_FUNC(void, UniformMatrix4fv,
  GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)

GL_FUNC(void, Uniform1uiv,
  GLint location, GLsizei count, const GLuint *value)

GL_FUNC(void, UseProgram,
  GLuint program)

/*******************************************************************************
 *
 * Vertex array
 *
 ******************************************************************************/
GL_FUNC(void, BindVertexArray,
  GLuint array)

GL_FUNC(void, DeleteVertexArrays,
  GLsizei n, const GLuint *arrays)

GL_FUNC(void, DisableVertexAttribArray,
  GLuint index)

GL_FUNC(void, DrawElements,
  GLenum mode, GLsizei count, GLenum type, const GLvoid *indices)

GL_FUNC(void, DrawArrays,
  GLenum mode, GLint first, GLsizei count)

GL_FUNC(void, EnableVertexAttribArray,
  GLuint index)

GL_FUNC(void, GenVertexArrays,
  GLsizei n, GLuint *arrays)

GL_FUNC(void, VertexAttrib1fv,
  GLuint index, const GLfloat *v)

GL_FUNC(void, VertexAttrib2fv,
  GLuint index, const GLfloat *v)

GL_FUNC(void, VertexAttrib3fv,
  GLuint index, const GLfloat *v)

GL_FUNC(void, VertexAttrib4fv,
  GLuint index, const GLfloat *v)

GL_FUNC(void, VertexAttribPointer,
  GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride,
  const GLvoid *pointer)

/*******************************************************************************
 *
 * Texture
 *
 ******************************************************************************/
GL_FUNC(void, ActiveTexture,
  GLenum texture)

GL_FUNC(void, GenTextures,
  GLsizei n, GLuint *textures)

GL_FUNC(void, DeleteTextures,
  GLsizei n, const GLuint *textures)

GL_FUNC(void, BindTexture,
  GLenum target, GLuint texture)

GL_FUNC(void, TexImage2D,
  GLenum target, GLint level, GLint internalFormat, GLsizei width,
  GLsizei height, GLint border, GLenum format, GLenum type,
  const GLvoid* pixels)

GL_FUNC(void, TexParameteri,
  GLenum target, GLenum pname, GLint param)

/*******************************************************************************
 *
 * Depth buffer
 *
 ******************************************************************************/
GL_FUNC(void, ClearDepth,
  GLclampd depth)

GL_FUNC(void, DepthFunc,
  GLenum func)

GL_FUNC(void, DepthMask,
  GLboolean flag)

GL_FUNC(void, DepthRange,
  GLclampd near_val, GLclampd far_val)

/*******************************************************************************
 *
 * Stencil buffer
 *
 ******************************************************************************/
GL_FUNC(void, ClearStencil,
  GLint s)

GL_FUNC(void, StencilFuncSeparate,
  GLenum face, GLenum func, GLint ref, GLuint mask)

GL_FUNC(void, StencilMaskSeparate,
  GLenum face, GLuint mask)

GL_FUNC(void, StencilOpSeparate,
  GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass)

/*******************************************************************************
 *
 * Samplers
 *
 ******************************************************************************/
GL_FUNC(void, GenSamplers,
  GLsizei count, GLuint *samplers)

GL_FUNC(void, DeleteSamplers,
  GLsizei count, const GLuint *samplers)

GL_FUNC(void, BindSampler,
  GLuint unit, GLuint sampler)

GL_FUNC(void, SamplerParameteri,
  GLuint sampler, GLenum pname, GLint param)

GL_FUNC(void, SamplerParameterf,
  GLuint sampler, GLenum pname, GLfloat param)

/*******************************************************************************
 *
 * Miscellaneous
 *
 ******************************************************************************/
GL_FUNC(void, BlendFuncSeparate,
  GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha,
  GLenum dfactorAlpha)

GL_FUNC(void, BlendEquationSeparate,
  GLenum modeRGB, GLenum modeAlpha)

GL_FUNC(void, Clear,
  GLbitfield mask)

GL_FUNC(void, ClearColor,
  GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)

GL_FUNC(void, Disable,
  GLenum cap)

GL_FUNC(void, Enable,
  GLenum cap)

GL_FUNC(void, Flush,
  void)

GL_FUNC(GLenum, GetError,
  void )

GL_FUNC(void, GetIntegerv,
  GLenum pname, GLint *params)

GL_FUNC(void, Viewport,
  GLint x, GLint y, GLsizei width, GLsizei height)

