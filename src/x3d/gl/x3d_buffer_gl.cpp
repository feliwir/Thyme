#include "x3d_buffer_gl.h"

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