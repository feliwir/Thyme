#include "x3d_buffer_d3d9.h"
#include <cassert>

void *X3D::X3DVertexBufferD3D9::Lock(X3DLockUsage usage, size_t offset, size_t size)
{
    assert(m_buffer != nullptr);
    void *buffer_data = nullptr;
    m_buffer->Lock(offset, size, &buffer_data, 0);
    return buffer_data;
}

void X3D::X3DVertexBufferD3D9::Unlock()
{
    m_buffer->Unlock();
}

void X3D::X3DVertexBufferD3D9::Bind()
{
    assert(m_device != nullptr);
    X3DContext::Bind_Vertex_Buffer(this);
    m_device->SetStreamSource(0, m_buffer, 0, m_vertexSize);
}

void *X3D::X3DIndexBufferD3D9::Lock(X3DLockUsage usage, size_t offset, size_t size)
{
    assert(m_buffer != nullptr);
    void *buffer_data = nullptr;
    m_buffer->Lock(offset, size, &buffer_data, 0);
    return buffer_data;
}

void X3D::X3DIndexBufferD3D9::Unlock()
{
    m_buffer->Unlock();
}

void X3D::X3DIndexBufferD3D9::Bind()
{
    assert(m_device != nullptr);
    X3DContext::Bind_Index_Buffer(this);
    m_device->SetIndices(m_buffer);
}