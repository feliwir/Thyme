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
std::function<void(X3DMessageSeverity, const char *)> s_msgCallback = nullptr;

static X3DBackend GetPreferredBackend()
{
#if 0 // defined X3D_HAS_D3D9
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

    return s_context->Init();
}

#ifdef _WIN32
int X3D::Init_From_Hwnd(X3DBackend backend, HWND hwnd)
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

    return s_context->Init_From_Hwnd(hwnd);
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

int X3D::Get_Current_Device_Id()
{
    if (s_context == nullptr)
        return -1;
    return s_context->Get_Device_Id();
}

void X3D::Clear(bool clear_color, bool clear_depth)
{
    assert(s_context != nullptr);
    s_context->Clear(clear_color, clear_depth);
}

void X3D::Set_Clear_Color(float r, float g, float b, float a)
{
    assert(s_context != nullptr);
    s_context->Set_Clear_Color(r, g, b, a);
}

void X3D::Present()
{
    assert(s_context != nullptr);
    s_context->Present();
}

void X3D::Begin()
{
    assert(s_context != nullptr);
    s_context->Begin();
}

void X3D::End()
{
    assert(s_context != nullptr);
    s_context->End();
}

void X3D::Set_Viewport(int x, int y, int w, int h)
{
    assert(s_context != nullptr);
    s_context->Set_Viewport(x, y, w, h);
}

void X3D::Get_Device_Resolution(int dev, int &resx, int &resy)
{
    assert(s_context != nullptr);
    s_context->Get_Resolution(resx, resy);
}

int X3D::Set_Resolution(int resx, int resy, int bits, int windowed, bool resize_window)
{
    assert(s_context != nullptr);
    s_context->Set_Resolution(resx, resy, bits, windowed, resize_window);
    return X3D::X3D_ERR_OK;
}

X3D::X3DShader *X3D::Create_Shader()
{
    assert(s_context != nullptr);
    return s_context->Create_Shader();
}

X3D::X3DVertexBuffer *X3D::Create_Vertex_Buffer(size_t size)
{
    assert(s_context != nullptr);
    assert(size > 0);
    return s_context->Create_Vertex_Buffer(size);
}

X3D::X3DIndexBuffer *X3D::Create_Index_Buffer(size_t size)
{
    assert(s_context != nullptr);
    assert(size > 0);
    return s_context->Create_Index_Buffer(size);
}

X3D::X3DVertexLayout *X3D::Create_Vertex_Layout()
{
    assert(s_context != nullptr);
    return s_context->Create_Vertex_Layout();
}

X3D::X3DTexture *X3D::Create_Texture(int width, int height, X3DTextureFormat format, int mip_count)
{
    assert(s_context != nullptr);
    return s_context->Create_Texture(width, height, format, mip_count);
}

int X3D::Draw_Indexed(X3DPrimitive type, int count, int baseVertex)
{
    assert(s_context != nullptr);
    assert(count > 0);
    return s_context->Draw_Indexed(type, count, baseVertex);
}

void X3D::Set_Message_Callback(std::function<void(X3DMessageSeverity, const char *)> callback)
{
    s_msgCallback = callback;
}
