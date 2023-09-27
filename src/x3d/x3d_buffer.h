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
    virtual void *Lock(X3DLockUsage, size_t offset, size_t size) = 0;
    virtual void Unlock() = 0;
};

class X3DIndexBuffer
{
public:
    virtual void *Lock(X3DLockUsage, size_t offset, size_t size) = 0;
    virtual void Unlock() = 0;
};
} // namespace X3D