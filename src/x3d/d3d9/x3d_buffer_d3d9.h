#pragma once
#include "x3d_buffer.h"
#include <cassert>
#include <d3d9.h>

namespace X3D
{
class X3DContext;
class X3DVertexBufferD3D9 : public X3DVertexBuffer
{
public:
    X3DVertexBufferD3D9(X3DContext *ctx, IDirect3DDevice9 *device, size_t size) :
        X3DVertexBuffer(size), m_device(device), m_context(ctx)
    {
        assert(SUCCEEDED(device->CreateVertexBuffer(size, D3DUSAGE_DYNAMIC, 0, D3DPOOL_DEFAULT, &m_buffer, NULL)));
    }

    void *Lock(X3DLockUsage usage, size_t offset, size_t size) override;
    void Unlock() override;

private:
    IDirect3DVertexBuffer9 *m_buffer = nullptr;
    IDirect3DDevice9 *m_device = nullptr;
    X3DContext *m_context = nullptr;

    // Inherited via X3DVertexBuffer
    void Bind() override;
};

class X3DIndexBufferD3D9 : public X3DIndexBuffer
{
    friend class X3DContextD3D9;

public:
    X3DIndexBufferD3D9(X3DContext *ctx, IDirect3DDevice9 *device, size_t size) :
        X3DIndexBuffer(size), m_device(device), m_context(ctx)
    {
        assert(
            SUCCEEDED(device->CreateIndexBuffer(size, D3DUSAGE_DYNAMIC, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_buffer, NULL)));
    }

    void *Lock(X3DLockUsage usage, size_t offset, size_t size) override;
    void Unlock() override;
    void Bind() override;

private:
    IDirect3DIndexBuffer9 *m_buffer = nullptr;
    IDirect3DDevice9 *m_device = nullptr;
    X3DContext *m_context = nullptr;
};
} // namespace X3D