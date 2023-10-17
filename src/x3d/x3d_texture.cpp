#include "x3d_texture.h"

int X3D::X3DTexture::Get_Bytesize(int level)
{
    int width = m_width >> level;
    int height = m_height >> level;
    switch (m_fmt) {
        case X3D_FMT_RGBA8:
        case X3D_FMT_ARGB8:
            return width * height * 4;

        case X3D_FMT_RGB8:
            return width * height * 3;

        case X3D_FMT_BC1:
            return width * height / 2;
        case X3D_FMT_BC3:
            return width * height;
        default:
            return -1;
    }
}