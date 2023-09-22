#pragma once
#include "x3d_context.h"

namespace X3D
{
enum X3DError
{
    X3D_OK = 0,
    X3D_UNSUPPORTED,
    X3D_ALREADY_INITIALIZED,
};

enum X3DBackend
{
    X3D_AUTO,
    X3D_D3D9,
    X3D_D3D11,
    X3D_OPENGL,
    X3D_VULKAN,
};

// This function assumes the context underlying context is already created (E.g. by SDL2)
int Init(X3DBackend backend = X3D_AUTO);
#ifdef _WIN32
// This function creates the context from the HWND
int Init_From_Hwnd(X3DBackend backend, HWND hwnd);
#endif

void Shutdown();
} // namespace X3D