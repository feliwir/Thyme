#include "x3dutil.h"

X3D::X3DTextureFormat WW3DFormat_To_X3DFormat(WW3DFormat format)
{
    switch (format)
    {
    case WW3D_FORMAT_R8G8B8:
        return X3D::X3D_FMT_RGB8;
    case WW3D_FORMAT_A8R8G8B8:
        return X3D::X3D_FMT_RGBA8;
    case WW3D_FORMAT_DXT1:
        return X3D::X3D_FMT_BC1;
    case WW3D_FORMAT_DXT2:
        return X3D::X3D_FMT_BC2;
    case WW3D_FORMAT_DXT3:
        return X3D::X3D_FMT_BC3;
    case WW3D_FORMAT_DXT4:
        return X3D::X3D_FMT_BC4;
    case WW3D_FORMAT_DXT5:
        return X3D::X3D_FMT_BC5;
    
    default:
        break;
    }
}