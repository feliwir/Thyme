#include "x3d_buffer_gl.h"

X3D::X3DVertexBufferGL::X3DVertexBufferGL(size_t size) : X3DVertexBuffer(size)
{
    glBindBuffer(GL_ARRAY_BUFFER, m_handle);
    glBufferData(GL_ARRAY_BUFFER, size, 0, GL_STATIC_DRAW);
}

void *X3D::X3DVertexBufferGL::Lock(X3DLockUsage usage, size_t offset, size_t size)
{
    glBindBuffer(GL_ARRAY_BUFFER, m_handle);
    return glMapBufferRange(GL_ARRAY_BUFFER, offset, size, usage == X3D_LOCK_READ ? GL_MAP_READ_BIT : GL_MAP_WRITE_BIT);
}

void X3D::X3DVertexBufferGL::Unlock()
{
    glBindBuffer(GL_ARRAY_BUFFER, m_handle);
    glUnmapBuffer(GL_ARRAY_BUFFER);
}

X3D::X3DIndexBufferGL::X3DIndexBufferGL(size_t size) : X3DIndexBuffer(size)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, 0, GL_STATIC_DRAW);
}

void *X3D::X3DIndexBufferGL::Lock(X3DLockUsage usage, size_t offset, size_t size)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handle);
    return glMapBufferRange(
        GL_ELEMENT_ARRAY_BUFFER, offset, size, usage == X3D_LOCK_READ ? GL_MAP_READ_BIT : GL_MAP_WRITE_BIT);
}

void X3D::X3DIndexBufferGL::Unlock()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handle);
    glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
}