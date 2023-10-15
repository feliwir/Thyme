#pragma once
#include <cstdint>

namespace X3D
{
enum X3DTextureFormat : uint8_t
{

    X3D_FMT_RGBA8,
    X3D_FMT_RGB8,
    X3D_FMT_RGBA16F,
    X3D_FMT_RGB16F,
    X3D_FMT_RGBA32F,
    X3D_FMT_RGB32F,
    X3D_FMT_BC1,
    X3D_FMT_BC2,
    X3D_FMT_BC3,
    X3D_FMT_BC4,
    X3D_FMT_BC5,
};

class X3DTexture
{
public:
    X3DTexture(int widht, int height, X3DTextureFormat fmt) : m_width(widht), m_height(height) {}

    virtual void Upload(uint8_t *data, size_t size, int layer = 0) = 0;

    virtual void Bind(int slot) = 0;
    virtual void Unbind(int slot) = 0;

protected:
    int m_width = 0;
    int m_height = 0;
    X3DTextureFormat m_fmt;
};
} // namespace X3D