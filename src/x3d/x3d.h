#pragma once
#include "x3d_context.h"

namespace X3D
{
enum X3DError
{
    X3D_ERR_OK = 0,
    X3D_ERR_UNSUPPORTED,
    X3D_ERR_ALREADY_INITIALIZED,
    X3D_ERR_FAILED_LOAD,
    X3D_ERR_FAILED_DEVICE_INIT,
};

enum X3DBackend
{
    X3D_AUTO,
    X3D_D3D9,
    X3D_D3D11,
    X3D_OPENGL,
    X3D_VULKAN,
};

struct X3DDeviceDescr
{
    char name[256];
    char vendor[256];
};

/// <summary>
/// This function assumes the context underlying context is already created (E.g. by SDL2)
/// </summary>
/// <param name="backend"></param>
/// <returns></returns>
int Init(X3DBackend backend = X3D_AUTO);
#ifdef _WIN32
/// <summary>
/// This function creates the context from the HWND
/// </summary>
/// <param name="backend"></param>
/// <param name="hwnd"></param>
/// <returns></returns>
int Init_From_Hwnd(X3DBackend backend, HWND hwnd);
#endif
/// <summary>
/// Shutdown all internal variables & devices
/// </summary>
void Shutdown();

const std::vector<X3DDevice> &Get_Device_List();
int Set_Device(
    int dev, int resx, int resy, int bits, int windowed, bool resize_window, bool reset_device, bool restore_assets);
void Get_Device_Resolution(int dev, int& resx, int& resy);
int Set_Resolution(int resx, int resy, int bits, int windowed, bool resize_window);
int Get_Current_Device_Id();

/// <summary>
/// Clear the framebuffer with the specified parameters
/// </summary>
/// <param name="clear_color"></param>
/// <param name="clear_depth"></param>
void Clear(bool clear_color, bool clear_depth);

/// <summary>
/// Set the clear color
/// </summary>
/// <param name="r"></param>
/// <param name="g"></param>
/// <param name="b"></param>
/// <param name="a"></param>
void Set_Clear_Color(float r, float g, float b, float a);

/// <summary>
/// Swap the backbuffer to the front
/// </summary>
void Present();

/// @brief Start the current drawing context
void Begin();

/// @brief End the current drawing context
void End();

void Set_Viewport(int, int, int, int);

} // namespace X3D