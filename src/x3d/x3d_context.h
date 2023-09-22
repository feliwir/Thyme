#pragma once
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

namespace X3D
{
class X3DContext
{
#ifdef _WIN32
    virtual int Init_From_Hwnd(HWND hwnd) = 0;
#endif
};
} // namespace X3D