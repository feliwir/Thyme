#pragma once
#include "x3d_types.h"
#include <cstddef>
#include <vector>

namespace X3D
{
class X3DVertexBuffer
{
public:
    X3DVertexBuffer(size_t size) : m_size(size) {}
    virtual ~X3DVertexBuffer() {}
    virtual void *Lock(X3DLockUsage, size_t offset, size_t size) = 0;
    virtual void Unlock() = 0;
    virtual void Bind() = 0;

    size_t Get_Size() const { return m_size; }

protected:
    size_t m_size = 0;
};

class X3DIndexBuffer
{
public:
    X3DIndexBuffer(size_t size) : m_size(size) {}
    virtual ~X3DIndexBuffer() {}
    virtual void *Lock(X3DLockUsage, size_t offset, size_t size) = 0;
    virtual void Unlock() = 0;
    virtual void Bind() = 0;

    size_t Get_Size() const { return m_size; }

protected:
    size_t m_size = 0;
};
} // namespace X3D