#include "x3d_buffer_gl.h"
#include "x3d_context_gl.h"

X3D::X3DVertexBufferGL::X3DVertexBufferGL(X3DContextGL *ctx, size_t size) :
    X3DVertexBuffer(size), X3DBufferGL(GL_ARRAY_BUFFER), m_context(ctx)
{
    Bind();
    glBufferData(m_target, size, 0, GL_STATIC_DRAW);
}

void *X3D::X3DVertexBufferGL::Lock(X3DLockUsage usage, size_t offset, size_t size)
{
    Bind();
    return glMapBufferRange(GL_ARRAY_BUFFER, offset, size, usage == X3D_LOCK_READ ? GL_MAP_READ_BIT : GL_MAP_WRITE_BIT);
}

void X3D::X3DVertexBufferGL::Unlock()
{
    Bind();
    glUnmapBuffer(GL_ARRAY_BUFFER);
}

void X3D::X3DVertexBufferGL::Bind()
{
    m_context->Bind_Vertex_Buffer(this);
    glBindBuffer(m_target, m_handle);
}

X3D::X3DIndexBufferGL::X3DIndexBufferGL(X3DContextGL *ctx, size_t size) :
    X3DIndexBuffer(size), X3DBufferGL(GL_ELEMENT_ARRAY_BUFFER), m_context(ctx)
{
    Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, 0, GL_STATIC_DRAW);
}

void *X3D::X3DIndexBufferGL::Lock(X3DLockUsage usage, size_t offset, size_t size)
{
    Bind();
    return glMapBufferRange(
        GL_ELEMENT_ARRAY_BUFFER, offset, size, usage == X3D_LOCK_READ ? GL_MAP_READ_BIT : GL_MAP_WRITE_BIT);
}

void X3D::X3DIndexBufferGL::Unlock()
{
    Bind();
    glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
}

void X3D::X3DIndexBufferGL::Bind()
{
    m_context->Bind_Index_Buffer(this);
    glBindBuffer(m_target, m_handle);
}