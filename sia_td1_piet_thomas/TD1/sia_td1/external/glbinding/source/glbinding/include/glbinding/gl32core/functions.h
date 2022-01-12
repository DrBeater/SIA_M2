
#pragma once


#include <glbinding/nogl.h>
#include <glbinding/gl/functions.h>


namespace gl32core
{


using gl::glActiveTexture;
using gl::glAttachShader;
using gl::glBeginConditionalRender;
using gl::glBeginQuery;
using gl::glBeginTransformFeedback;
using gl::glBindAttribLocation;
using gl::glBindBuffer;
using gl::glBindBufferBase;
using gl::glBindBufferRange;
using gl::glBindFragDataLocation;
using gl::glBindFramebuffer;
using gl::glBindRenderbuffer;
using gl::glBindTexture;
using gl::glBindVertexArray;
using gl::glBlendColor;
using gl::glBlendEquation;
using gl::glBlendEquationSeparate;
using gl::glBlendFunc;
using gl::glBlendFuncSeparate;
using gl::glBlitFramebuffer;
using gl::glBufferData;
using gl::glBufferSubData;
using gl::glCheckFramebufferStatus;
using gl::glClampColor;
using gl::glClear;
using gl::glClearBufferfi;
using gl::glClearBufferfv;
using gl::glClearBufferiv;
using gl::glClearBufferuiv;
using gl::glClearColor;
using gl::glClearDepth;
using gl::glClearStencil;
using gl::glClientWaitSync;
using gl::glColorMask;
using gl::glColorMaski;
using gl::glCompileShader;
using gl::glCompressedTexImage1D;
using gl::glCompressedTexImage2D;
using gl::glCompressedTexImage3D;
using gl::glCompressedTexSubImage1D;
using gl::glCompressedTexSubImage2D;
using gl::glCompressedTexSubImage3D;
using gl::glCopyBufferSubData;
using gl::glCopyTexImage1D;
using gl::glCopyTexImage2D;
using gl::glCopyTexSubImage1D;
using gl::glCopyTexSubImage2D;
using gl::glCopyTexSubImage3D;
using gl::glCreateProgram;
using gl::glCreateShader;
using gl::glCullFace;
using gl::glDeleteBuffers;
using gl::glDeleteFramebuffers;
using gl::glDeleteProgram;
using gl::glDeleteQueries;
using gl::glDeleteRenderbuffers;
using gl::glDeleteShader;
using gl::glDeleteSync;
using gl::glDeleteTextures;
using gl::glDeleteVertexArrays;
using gl::glDepthFunc;
using gl::glDepthMask;
using gl::glDepthRange;
using gl::glDetachShader;
using gl::glDisable;
using gl::glDisableVertexAttribArray;
using gl::glDisablei;
using gl::glDrawArrays;
using gl::glDrawArraysInstanced;
using gl::glDrawBuffer;
using gl::glDrawBuffers;
using gl::glDrawElements;
using gl::glDrawElementsBaseVertex;
using gl::glDrawElementsInstanced;
using gl::glDrawElementsInstancedBaseVertex;
using gl::glDrawRangeElements;
using gl::glDrawRangeElementsBaseVertex;
using gl::glEnable;
using gl::glEnableVertexAttribArray;
using gl::glEnablei;
using gl::glEndConditionalRender;
using gl::glEndQuery;
using gl::glEndTransformFeedback;
using gl::glFenceSync;
using gl::glFinish;
using gl::glFlush;
using gl::glFlushMappedBufferRange;
using gl::glFramebufferRenderbuffer;
using gl::glFramebufferTexture;
using gl::glFramebufferTexture1D;
using gl::glFramebufferTexture2D;
using gl::glFramebufferTexture3D;
using gl::glFramebufferTextureLayer;
using gl::glFrontFace;
using gl::glGenBuffers;
using gl::glGenFramebuffers;
using gl::glGenQueries;
using gl::glGenRenderbuffers;
using gl::glGenTextures;
using gl::glGenVertexArrays;
using gl::glGenerateMipmap;
using gl::glGetActiveAttrib;
using gl::glGetActiveUniform;
using gl::glGetActiveUniformBlockName;
using gl::glGetActiveUniformBlockiv;
using gl::glGetActiveUniformName;
using gl::glGetActiveUniformsiv;
using gl::glGetAttachedShaders;
using gl::glGetAttribLocation;
using gl::glGetBooleani_v;
using gl::glGetBooleanv;
using gl::glGetBufferParameteri64v;
using gl::glGetBufferParameteriv;
using gl::glGetBufferPointerv;
using gl::glGetBufferSubData;
using gl::glGetCompressedTexImage;
using gl::glGetDoublev;
using gl::glGetError;
using gl::glGetFloatv;
using gl::glGetFragDataLocation;
using gl::glGetFramebufferAttachmentParameteriv;
using gl::glGetInteger64i_v;
using gl::glGetInteger64v;
using gl::glGetIntegeri_v;
using gl::glGetIntegerv;
using gl::glGetMultisamplefv;
using gl::glGetProgramInfoLog;
using gl::glGetProgramiv;
using gl::glGetQueryObjectiv;
using gl::glGetQueryObjectuiv;
using gl::glGetQueryiv;
using gl::glGetRenderbufferParameteriv;
using gl::glGetShaderInfoLog;
using gl::glGetShaderSource;
using gl::glGetShaderiv;
using gl::glGetString;
using gl::glGetStringi;
using gl::glGetSynciv;
using gl::glGetTexImage;
using gl::glGetTexLevelParameterfv;
using gl::glGetTexLevelParameteriv;
using gl::glGetTexParameterIiv;
using gl::glGetTexParameterIuiv;
using gl::glGetTexParameterfv;
using gl::glGetTexParameteriv;
using gl::glGetTransformFeedbackVarying;
using gl::glGetUniformBlockIndex;
using gl::glGetUniformIndices;
using gl::glGetUniformLocation;
using gl::glGetUniformfv;
using gl::glGetUniformiv;
using gl::glGetUniformuiv;
using gl::glGetVertexAttribIiv;
using gl::glGetVertexAttribIuiv;
using gl::glGetVertexAttribPointerv;
using gl::glGetVertexAttribdv;
using gl::glGetVertexAttribfv;
using gl::glGetVertexAttribiv;
using gl::glHint;
using gl::glIsBuffer;
using gl::glIsEnabled;
using gl::glIsEnabledi;
using gl::glIsFramebuffer;
using gl::glIsProgram;
using gl::glIsQuery;
using gl::glIsRenderbuffer;
using gl::glIsShader;
using gl::glIsSync;
using gl::glIsTexture;
using gl::glIsVertexArray;
using gl::glLineWidth;
using gl::glLinkProgram;
using gl::glLogicOp;
using gl::glMapBuffer;
using gl::glMapBufferRange;
using gl::glMultiDrawArrays;
using gl::glMultiDrawElements;
using gl::glMultiDrawElementsBaseVertex;
using gl::glPixelStoref;
using gl::glPixelStorei;
using gl::glPointParameterf;
using gl::glPointParameterfv;
using gl::glPointParameteri;
using gl::glPointParameteriv;
using gl::glPointSize;
using gl::glPolygonMode;
using gl::glPolygonOffset;
using gl::glPrimitiveRestartIndex;
using gl::glProvokingVertex;
using gl::glReadBuffer;
using gl::glReadPixels;
using gl::glRenderbufferStorage;
using gl::glRenderbufferStorageMultisample;
using gl::glSampleCoverage;
using gl::glSampleMaski;
using gl::glScissor;
using gl::glShaderSource;
using gl::glStencilFunc;
using gl::glStencilFuncSeparate;
using gl::glStencilMask;
using gl::glStencilMaskSeparate;
using gl::glStencilOp;
using gl::glStencilOpSeparate;
using gl::glTexBuffer;
using gl::glTexImage1D;
using gl::glTexImage2D;
using gl::glTexImage2DMultisample;
using gl::glTexImage3D;
using gl::glTexImage3DMultisample;
using gl::glTexParameterIiv;
using gl::glTexParameterIuiv;
using gl::glTexParameterf;
using gl::glTexParameterfv;
using gl::glTexParameteri;
using gl::glTexParameteriv;
using gl::glTexSubImage1D;
using gl::glTexSubImage2D;
using gl::glTexSubImage3D;
using gl::glTransformFeedbackVaryings;
using gl::glUniform1f;
using gl::glUniform1fv;
using gl::glUniform1i;
using gl::glUniform1iv;
using gl::glUniform1ui;
using gl::glUniform1uiv;
using gl::glUniform2f;
using gl::glUniform2fv;
using gl::glUniform2i;
using gl::glUniform2iv;
using gl::glUniform2ui;
using gl::glUniform2uiv;
using gl::glUniform3f;
using gl::glUniform3fv;
using gl::glUniform3i;
using gl::glUniform3iv;
using gl::glUniform3ui;
using gl::glUniform3uiv;
using gl::glUniform4f;
using gl::glUniform4fv;
using gl::glUniform4i;
using gl::glUniform4iv;
using gl::glUniform4ui;
using gl::glUniform4uiv;
using gl::glUniformBlockBinding;
using gl::glUniformMatrix2fv;
using gl::glUniformMatrix2x3fv;
using gl::glUniformMatrix2x4fv;
using gl::glUniformMatrix3fv;
using gl::glUniformMatrix3x2fv;
using gl::glUniformMatrix3x4fv;
using gl::glUniformMatrix4fv;
using gl::glUniformMatrix4x2fv;
using gl::glUniformMatrix4x3fv;
using gl::glUnmapBuffer;
using gl::glUseProgram;
using gl::glValidateProgram;
using gl::glVertexAttrib1d;
using gl::glVertexAttrib1dv;
using gl::glVertexAttrib1f;
using gl::glVertexAttrib1fv;
using gl::glVertexAttrib1s;
using gl::glVertexAttrib1sv;
using gl::glVertexAttrib2d;
using gl::glVertexAttrib2dv;
using gl::glVertexAttrib2f;
using gl::glVertexAttrib2fv;
using gl::glVertexAttrib2s;
using gl::glVertexAttrib2sv;
using gl::glVertexAttrib3d;
using gl::glVertexAttrib3dv;
using gl::glVertexAttrib3f;
using gl::glVertexAttrib3fv;
using gl::glVertexAttrib3s;
using gl::glVertexAttrib3sv;
using gl::glVertexAttrib4Nbv;
using gl::glVertexAttrib4Niv;
using gl::glVertexAttrib4Nsv;
using gl::glVertexAttrib4Nub;
using gl::glVertexAttrib4Nubv;
using gl::glVertexAttrib4Nuiv;
using gl::glVertexAttrib4Nusv;
using gl::glVertexAttrib4bv;
using gl::glVertexAttrib4d;
using gl::glVertexAttrib4dv;
using gl::glVertexAttrib4f;
using gl::glVertexAttrib4fv;
using gl::glVertexAttrib4iv;
using gl::glVertexAttrib4s;
using gl::glVertexAttrib4sv;
using gl::glVertexAttrib4ubv;
using gl::glVertexAttrib4uiv;
using gl::glVertexAttrib4usv;
using gl::glVertexAttribI1i;
using gl::glVertexAttribI1iv;
using gl::glVertexAttribI1ui;
using gl::glVertexAttribI1uiv;
using gl::glVertexAttribI2i;
using gl::glVertexAttribI2iv;
using gl::glVertexAttribI2ui;
using gl::glVertexAttribI2uiv;
using gl::glVertexAttribI3i;
using gl::glVertexAttribI3iv;
using gl::glVertexAttribI3ui;
using gl::glVertexAttribI3uiv;
using gl::glVertexAttribI4bv;
using gl::glVertexAttribI4i;
using gl::glVertexAttribI4iv;
using gl::glVertexAttribI4sv;
using gl::glVertexAttribI4ubv;
using gl::glVertexAttribI4ui;
using gl::glVertexAttribI4uiv;
using gl::glVertexAttribI4usv;
using gl::glVertexAttribIPointer;
using gl::glVertexAttribPointer;
using gl::glViewport;
using gl::glWaitSync;


} // namespace gl32core
