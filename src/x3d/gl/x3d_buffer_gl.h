#pragma once
#include "x3d_buffer.h"

#include <flextGL.h>

namespace X3D
{
class X3DContextGL;
class X3DBufferGL
{
public:
    X3DBufferGL(GLenum target) : m_target(target) { glGenBuffers(1, &m_handle); }
    ~X3DBufferGL() { 
        glDeleteBuffers(1, &m_handle); 
    }

protected:
    GLenum m_target = GL_INVALID_ENUM;
    GLuint m_handle = 0;
};

class X3DVertexBufferGL : public X3DBufferGL, public X3DVertexBuffer
{
public:
    X3DVertexBufferGL(X3DContextGL *ctx, size_t size);
    void *Lock(X3DLockUsage usage, size_t offset, size_t size) override;
    void Unlock() override;
    void Bind() override;

protected:
    X3DContextGL *m_context = nullptr;
};

class X3DIndexBufferGL : public X3DBufferGL, public X3DIndexBuffer
{
public:
    X3DIndexBufferGL(X3DContextGL *ctx, size_t size);
    void *Lock(X3DLockUsage usage, size_t offset, size_t size) override;
    void Unlock() override;
    void Bind() override;

protected:
    X3DContextGL *m_context = nullptr;
};
} // namespace X3D