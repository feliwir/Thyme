#pragma once
#include <vector>

namespace X3D
{
enum X3DLockUsage
{
    X3D_LOCK_READ,
    X3D_LOCK_WRITE
};

class X3DVertexBuffer
{
public:
    X3DVertexBuffer(size_t size) : m_size(size) {}
    virtual void *Lock(X3DLockUsage, size_t offset, size_t size) = 0;
    virtual void Unlock() = 0;

    size_t Get_Size() const { return m_size; }

protected:
    size_t m_size = 0;
};

class X3DIndexBuffer
{
public:
    X3DIndexBuffer(size_t size) : m_size(size) {}
    virtual void *Lock(X3DLockUsage, size_t offset, size_t size) = 0;
    virtual void Unlock() = 0;

    size_t Get_Size() const { return m_size; }

protected:
    size_t m_size = 0;
};
} // namespace X3D