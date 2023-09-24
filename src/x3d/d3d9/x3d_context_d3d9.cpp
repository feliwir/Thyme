#include "x3d_context_d3d9.h"
#include "x3d.h"
#include <comdef.h>

HMODULE X3D::X3DContextD3D9::s_library = nullptr;
IDirect3D9 *(*X3D::X3DContextD3D9::s_create_function)(unsigned) = nullptr;

X3D::X3DContextD3D9::X3DContextD3D9() : m_clearColor(0) {}

int X3D::X3DContextD3D9::Init_From_Hwnd(HWND hwnd)
{
    if (s_library == nullptr) {
        s_library = LoadLibraryA("d3d9.dll");
        if (s_library == nullptr) {
            return X3D_ERR_FAILED_LOAD;
        }
    }

    if (s_create_function == nullptr) {
        s_create_function =
            reinterpret_cast<IDirect3D9 *(__stdcall *)(unsigned)>(GetProcAddress(s_library, "Direct3DCreate9"));
        if (s_create_function == nullptr) {
            return X3D_ERR_FAILED_LOAD;
        }
    }

    m_hwnd = hwnd;
    m_d3d = s_create_function(D3D9b_SDK_VERSION);
    Enumerate_Devices();

    return X3D_ERR_OK;
}

int X3D::X3DContextD3D9::Enumerate_Devices()
{
    assert(m_d3d != nullptr);
    UINT adapter_count = m_d3d->GetAdapterCount();
    for (int i = 0; i < adapter_count; i++) {
        D3DADAPTER_IDENTIFIER9 id;
        memset(&id, 0, sizeof(id));
        HRESULT res = m_d3d->GetAdapterIdentifier(i, 0, &id);

        if (SUCCEEDED(res)) {

            D3DCAPS9 caps;
            int mode_count = m_d3d->GetAdapterModeCount(i, D3DFMT_X8R8G8B8);

            for (int j = 0; j < mode_count; j++) {
                D3DDISPLAYMODE d3dmode;
                memset(&d3dmode, 0, sizeof(d3dmode));
                res = m_d3d->EnumAdapterModes(i, D3DFMT_X8R8G8B8, j, &d3dmode);

                if (SUCCEEDED(res)) {
                    int bits = 0;

                    switch (d3dmode.Format) {
                        case D3DFMT_R8G8B8:
                        case D3DFMT_A8R8G8B8:
                        case D3DFMT_X8R8G8B8:
                            bits = 32;
                            break;

                        case D3DFMT_R5G6B5:
                        case D3DFMT_X1R5G5B5:
                            bits = 16;
                            break;
                        default:
                            break;
                    }
                }
            }

            X3DDevice &dev = m_devices.emplace_back();
            strcpy(dev.Name, id.Description);
        }
    }

    return X3D_ERR_OK;
}

int X3D::X3DContextD3D9::Set_Device(
    int dev, int resx, int resy, int bits, int windowed, bool resize_window, bool reset_device, bool restore_assets)
{
    D3DPRESENT_PARAMETERS present_params;
    ZeroMemory(&present_params, sizeof(present_params));
    present_params.BackBufferWidth = resx;
    present_params.BackBufferHeight = resy;
    present_params.MultiSampleType = D3DMULTISAMPLE_NONE;
    present_params.SwapEffect = D3DSWAPEFFECT_COPY;
    present_params.EnableAutoDepthStencil = true;
    present_params.BackBufferCount = (windowed == 0) + 1;
    present_params.hDeviceWindow = m_hwnd;
    present_params.Flags = 0;
    present_params.FullScreen_RefreshRateInHz = 0;
    present_params.Windowed = windowed;
    present_params.AutoDepthStencilFormat = D3DFMT_D16;

    D3DDISPLAYMODE mode;
    m_d3d->GetAdapterDisplayMode(dev, &mode);
    present_params.BackBufferFormat = mode.Format;

    HRESULT hr =
        m_d3d->CreateDevice(dev, D3DDEVTYPE_HAL, m_hwnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &present_params, &m_device);
    if (FAILED(hr)) {
        _com_error err(hr);
        LPCTSTR errMsg = err.ErrorMessage();

        return X3D_ERR_FAILED_DEVICE_INIT;
    }

    return X3D_ERR_OK;
}

void X3D::X3DContextD3D9::Clear(bool clear_color, bool clear_depth)
{
    assert(m_device != nullptr);
    unsigned long flags = 0;

    if (clear_color)
        flags |= D3DCLEAR_TARGET;

    if (clear_depth)
        flags |= D3DCLEAR_ZBUFFER;

    m_device->Clear(0, nullptr, flags, m_clearColor, 1.0f, 0);
}

void X3D::X3DContextD3D9::Set_Clear_Color(float r, float g, float b, float a)
{
    m_clearColor = D3DCOLOR_COLORVALUE(r, g, b, a);
}

void X3D::X3DContextD3D9::Present()
{
    assert(m_device != nullptr);
    m_device->Present(nullptr, nullptr, nullptr, nullptr);
}