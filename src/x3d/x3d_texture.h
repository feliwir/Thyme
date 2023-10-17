#pragma once
#include "x3d_types.h"
#include <cstddef>
#include <cstdint>

namespace X3D
{
enum X3DTextureFormat : uint8_t
{
    X3D_FMT_UNKNOWN,
    X3D_FMT_ARGB8,
    X3D_FMT_RGBA8,
    X3D_FMT_RGB8,
    X3D_FMT_RGBA16F,
    X3D_FMT_RGB16F,
    X3D_FMT_RGBA32F,
    X3D_FMT_RGB32F,
    X3D_FMT_BC1,
    X3D_FMT_BC3,
};

class X3DTexture
{
public:
    X3DTexture(int widht, int height, X3DTextureFormat fmt, int levels) :
        m_width(widht), m_height(height), m_fmt(fmt), m_levels(levels)
    {
    }
    virtual ~X3DTexture() {}

    virtual void Upload(uint8_t *data, size_t size, int level = 0) = 0;
    virtual void *Lock(X3DLockUsage, int level = 0) = 0;
    virtual void Unlock(int level = 0) = 0;

    virtual void Bind(int slot) = 0;
    virtual void Unbind(int slot) = 0;

    int Get_Levels() const { return m_levels; }
    int Get_Width() const { return m_width; }
    int Get_Height() const { return m_height; }
    int Get_Bytesize(int level);

protected:
    int m_width = 0;
    int m_height = 0;
    X3DTextureFormat m_fmt;
    int m_levels = 0;
};
} // namespace X3D