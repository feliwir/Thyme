#pragma once
#include <cstdint>
#include <vector>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#endif

namespace X3D
{
struct X3DDevice
{
    char Name[256];
    char Vendor[256];
};

class X3DIndexBuffer;
class X3DIndexBuffer;
class X3DVertexBuffer;
enum X3DPrimitive : uint8_t;
class X3DContext
{
public:
    virtual int Init() = 0;
#ifdef _WIN32
    virtual int Init_From_Hwnd(HWND hwnd) = 0;
#endif
    const std::vector<X3DDevice> &Get_Device_List() const { return m_devices; }
    virtual int Set_Device(
        int dev, int resx, int resy, int bits, int windowed, bool resize_window, bool reset_device, bool restore_assets);
    int Get_Device_Id() { return m_dev; }

    void Set_Resolution(int width, int height, int bits, int windows, bool resuze_window);
    void Get_Resolution(int &resx, int &resy);

    virtual void Clear(bool clear_color, bool clear_depth) = 0;
    virtual void Set_Clear_Color(float r, float g, float b, float a) = 0;
    virtual void Present() = 0;

    virtual void Begin();
    virtual void End();

    virtual void Set_Viewport(int x, int y, int w, int h) = 0;

    virtual X3DVertexBuffer *Create_Vertex_Buffer(size_t size) = 0;
    virtual X3DIndexBuffer *Create_Index_Buffer(size_t size) = 0;

    virtual void Bind_Vertex_Buffer(X3DVertexBuffer *vb) { m_vb = vb; }
    virtual void Bind_Index_Buffer(X3DIndexBuffer *ib) { m_ib = ib; }

    virtual int Draw_Indexed(X3DPrimitive type, int start, int count, int baseVertex = 0) = 0;

protected:
    std::vector<X3DDevice> m_devices;
    int m_dev;
    int m_resx;
    int m_resy;
    int m_bits;
    int m_windowed;
    bool m_rendering = false;
    int m_vertexSize = 0;
    X3DVertexBuffer *m_vb = nullptr;
    X3DIndexBuffer *m_ib = nullptr;
};
} // namespace X3D