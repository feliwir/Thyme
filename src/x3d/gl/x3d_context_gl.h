#pragma once
#include "x3d_context.h"

namespace X3D
{
class X3DContextGL : public X3DContext
{
#ifdef _WIN32
    int Init_From_Hwnd(HWND hwnd) override;
#endif
    int Set_Device(
        int dev, int resx, int resy, int bits, int windowed, bool resize_window, bool reset_device, bool restore_assets)
        override;

    void Clear(bool clear_color, bool clear_depth) override;
    void Set_Clear_Color(float r, float g, float b, float a) override;
    void Present() override;

private:
    int LoadFunctions();

#ifdef _WIN32
    HDC m_hdc;
#endif
};
} // namespace X3D