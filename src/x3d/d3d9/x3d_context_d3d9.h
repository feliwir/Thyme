#pragma once
#include "x3d_context.h"
#include <d3d9.h>

namespace X3D
{
class X3DContextD3D9 : public X3DContext
{
public:
    X3DContextD3D9();
    int Init_From_Hwnd(HWND hwnd) override;
    int Set_Device(
        int dev, int resx, int resy, int bits, int windowed, bool resize_window, bool reset_device, bool restore_assets)
        override;
    void Clear(bool clear_color, bool clear_depth) override;
    void Set_Clear_Color(float r, float g, float b, float a) override;
    void Present() override;
    void Begin() override;
    void End() override;

    void Set_Viewport(int x, int y, int w, int h);

    X3DVertexBuffer* Create_Vertex_Buffer() override;
protected:
    int Enumerate_Devices();

    IDirect3D9 *m_d3d = nullptr;
    IDirect3DDevice9 *m_device = nullptr;
    HWND m_hwnd = nullptr;
    D3DCOLOR m_clearColor;

    static HMODULE s_library;
    static IDirect3D9 *(__stdcall *s_create_function)(unsigned);
};
} // namespace X3D