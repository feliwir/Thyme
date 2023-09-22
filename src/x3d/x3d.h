#pragma once
#ifdef _WIN32
#include <windows.h>
#endif

namespace x3d
{
enum X3DBackend
{
    X3D_D3D9,
    X3D_D3D11,
    X3D_OpenGL,
    X3D_Vulkan,
};

#ifdef _WIN32
int Init_From_Hwnd(HWND hwnd);

X3DBackend s_backend = X3D_D3D9;
#endif
} // namespace x3d