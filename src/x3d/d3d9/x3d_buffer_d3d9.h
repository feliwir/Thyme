#pragma once
#include "x3d_buffer.h"
#include <cassert>
#include <d3d9.h>

namespace X3D
{
class X3DVertexBufferD3D9 : public X3DVertexBuffer
{
    friend class X3DContextD3D9;

public:
    X3DVertexBufferD3D9(IDirect3DDevice9 *device, size_t size) : X3DVertexBuffer(size)
    {
        assert(SUCCEEDED(device->CreateVertexBuffer(size, D3DUSAGE_DYNAMIC, 0, D3DPOOL_DEFAULT, &m_buffer, NULL)));
    }

    void *Lock(X3DLockUsage usage, size_t offset, size_t size) override;
    void Unlock() override;

private:
    IDirect3DVertexBuffer9 *m_buffer = nullptr;
};

class X3DIndexBufferD3D9 : public X3DIndexBuffer
{
    friend class X3DContextD3D9;

public:
    X3DIndexBufferD3D9(IDirect3DDevice9 *device, size_t size) : X3DIndexBuffer(size)
    {
        assert(
            SUCCEEDED(device->CreateIndexBuffer(size, D3DUSAGE_DYNAMIC, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_buffer, NULL)));
    }

    void *Lock(X3DLockUsage usage, size_t offset, size_t size) override;
    void Unlock() override;

private:
    IDirect3DIndexBuffer9 *m_buffer = nullptr;
};
} // namespace X3D