#include "x3d.h"
#ifdef X3D_HAS_D3D9
#include "d3d9/x3d_context_d3d9.h"
#endif
#ifdef X3D_HAS_OPENGL
#include "gl/x3d_context_gl.h"
#endif

#include <cassert>

namespace X3D
{
static X3DContext *s_context = nullptr;

static X3DBackend GetPreferredBackend()
{
#if defined X3D_HAS_D3D9
    return X3D_D3D9;
#elif defined X3D_HAS_OPENGL
    return X3D_OPENGL;
#else
    return X3D_OPENGL;
#endif
}
} // namespace X3D

int X3D::Init(X3DBackend backend)
{
    if (s_context) {
        return X3D_ERR_ALREADY_INITIALIZED;
    }

    if (backend == X3D_AUTO) {
        backend = GetPreferredBackend();
    }

    switch (backend) {
#ifdef X3D_HAS_OPENGL
        case X3D_OPENGL:
            s_context = new X3DContextGL;
            break;
#endif
#ifdef X3D_HAS_D3D9
        case X3D_D3D9:
            s_context = new X3DContextD3D9;
            break;
#endif
        case X3D_D3D11:
            break;

        default:
            return X3D_ERR_UNSUPPORTED;
    }

    return X3D_ERR_OK;
}

#ifdef _WIN32
int X3D::Init_From_Hwnd(X3DBackend backend, HWND hwnd)
{
    int result = Init(backend);
    if (result != X3D_ERR_OK)
        return result;

    result = s_context->Init_From_Hwnd(hwnd);
    return result;
}
#endif

void X3D::Shutdown()
{
    delete s_context;
}

const std::vector<X3D::X3DDevice> &X3D::Get_Device_List()
{
    assert(s_context != nullptr);
    return s_context->Get_Device_List();
}

int X3D::Set_Device(
    int dev, int resx, int resy, int bits, int windowed, bool resize_window, bool reset_device, bool restore_assets)
{
    assert(s_context != nullptr);
    return s_context->Set_Device(dev, resx, resy, bits, windowed, resize_window, reset_device, restore_assets);
}

void X3D::Clear(bool clear_color, bool clear_depth)
{
    assert(s_context != nullptr);
    return s_context->Clear(clear_color, clear_depth);
}

void X3D::Set_Clear_Color(float r, float g, float b, float a)
{
    assert(s_context != nullptr);
    return s_context->Set_Clear_Color(r, g, b, a);
}

void X3D::Present()
{
    assert(s_context != nullptr);
    return s_context->Present();
}