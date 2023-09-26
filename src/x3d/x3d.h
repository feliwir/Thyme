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

/// @brief This function assumes the context underlying context is already created (E.g. by SDL2)
/// @param backend
/// @return
int Init(X3DBackend backend = X3D_AUTO);
#ifdef _WIN32
/// @brief This function creates the context from the HWND
/// @param backend
/// @param hwnd
/// @return
int Init_From_Hwnd(X3DBackend backend, HWND hwnd);
#endif

/// @brief Shutdown all internal variables & devices
void Shutdown();

const std::vector<X3DDevice> &Get_Device_List();
int Set_Device(
    int dev, int resx, int resy, int bits, int windowed, bool resize_window, bool reset_device, bool restore_assets);
void Get_Device_Resolution(int dev, int &resx, int &resy);
int Set_Resolution(int resx, int resy, int bits, int windowed, bool resize_window);
int Get_Current_Device_Id();

/// @brief Clear the framebuffer with the specified parameters
/// @param clear_color
/// @param clear_depth
void Clear(bool clear_color, bool clear_depth);

/// @brief Set the clear color
/// @param r
/// @param g
/// @param b
/// @param a
void Set_Clear_Color(float r, float g, float b, float a);

/// @brief Swap the backbuffer to the front
void Present();

/// @brief Start the current drawing context
void Begin();

/// @brief End the current drawing context
void End();

/// @brief Set the viewport
/// @param
/// @param
/// @param
/// @param
void Set_Viewport(int, int, int, int);

X3DVertexBuffer *Create_Vertex_Buffer(size_t size);

} // namespace X3D