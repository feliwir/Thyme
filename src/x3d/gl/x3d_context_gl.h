#pragma once
#include "x3d_context.h"

namespace X3D
{
class X3DContextGL : public X3DContext
{
#ifdef _WIN32
    int Init_From_Hwnd(HWND hwnd) override;
#endif
};
} // namespace X3D