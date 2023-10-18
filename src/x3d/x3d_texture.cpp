#include "x3d_texture.h"
#include <cmath>

int X3D::X3DTexture::Get_Level_Bytesize(int level)
{
    int width = Get_Width(level);
    int height = Get_Height(level);
    switch (m_fmt) {
        case X3D_FMT_RGBA8:
        case X3D_FMT_ARGB8:
            return width * height * 4;

        case X3D_FMT_RGB8:
            return width * height * 3;

        case X3D_FMT_BC1:
            return 8 * ceilf(width / 4.0f) * ceilf(height / 4.0f);
        case X3D_FMT_BC3:
            return 16 * ceilf(width / 4.0f) * ceilf(height / 4.0f);
        default:
            return -1;
    }
}

int X3D::X3DTexture::Get_Total_Bytesize()
{
    int size = 0;
    for (int i = 0; i < m_levels; i++)
        size += Get_Level_Bytesize(i);
    return size;
}

intptr_t X3D::X3DTexture::Get_Level_Offset(int level)
{
    intptr_t offset = 0;
    for (int i = 0; i < level; i++)
        offset += Get_Level_Bytesize(i);
    return offset;
}