#pragma once
#include "x3d_context.h"

namespace X3D
{
class X3DContextGL : public X3DContext
{
    int Init() override;
#ifdef _WIN32
    int Init_From_Hwnd(HWND hwnd) override;
#endif
    void Clear(bool clear_color, bool clear_depth) override;
    void Set_Clear_Color(float r, float g, float b, float a) override;
    void Present() override;

    void Set_Viewport(int x, int y, int w, int h) override;

    X3DShader *Create_Shader() override;

    X3DVertexBuffer *Create_Vertex_Buffer(size_t size) override;
    X3DIndexBuffer *Create_Index_Buffer(size_t size) override;

    X3DVertexLayout *Create_Vertex_Layout(X3DLayoutDescription *descr) override;

    int Draw_Indexed(X3DPrimitive type, int start, int count, int baseVertex = 0) override;

private:
    int Load_Functions();

#ifdef _WIN32
    HDC m_hdc;
#endif
};
} // namespace X3D