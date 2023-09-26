#pragma once
#include "x3d_vertexbuffer.h"

#include "flextGL.h"

namespace X3D
{
class X3DBufferGL
{
public:
    X3DBufferGL() { glGenBuffers(1, &m_handle); }
    ~X3DBufferGL() { glDeleteBuffers(1, &m_handle); }

protected:
    GLuint m_handle = 0;
};

class X3DVertexBufferGL : public X3DBufferGL, public X3DVertexBuffer
{
public:
    X3DVertexBufferGL(size_t size);
    void *Lock(X3DLockUsage usage, size_t offset, size_t size) override;
    void Unlock() override;
};
} // namespace X3D