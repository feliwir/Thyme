#include "x3d_context_gl.h"
#include "x3d.h"
#include "x3d_buffer_gl.h"
#include "x3d_shader_gl.h"
#include "x3d_texture_gl.h"
#include "x3d_vertexlayout_gl.h"

#include <flextGL.h>

#include <cassert>

namespace X3D
{
extern std::function<void(X3DMessageSeverity, const char *)> s_msgCallback;

static void APIENTRY glDebugMessageCallback(
    GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
{
    // No handling
    if (s_msgCallback == nullptr)
        return;

    if (severity != GL_DEBUG_SEVERITY_HIGH_ARB)
        return;

    switch (type) {
        case GL_DEBUG_TYPE_ERROR_ARB:
            s_msgCallback(X3D_MSG_SEVERITY_ERROR, message);
            break;
        default:
            s_msgCallback(X3D_MSG_SEVERITY_INFO, message);
            break;
    }
}
} // namespace X3D

int X3D::X3DContextGL::Load_Functions()
{
    if (flextInit() == GL_FALSE) {
        return X3D_ERR_FAILED_DEVICE_INIT;
    }
    return X3D_ERR_OK;
}

int X3D::X3DContextGL::Init()
{
    int result = Load_Functions();

    // Check if the extension for debug output exists
    if (FLEXT_ARB_debug_output) {
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
        // TODO: we could pass our function here
        glDebugMessageCallbackARB(glDebugMessageCallback, nullptr);
    }

    return result;
}

#ifdef _WIN32
#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092

typedef HGLRC(APIENTRY PFNWGLCREATECONTEXTATTRIBS_PROC(HDC hDC, HGLRC hShareContext, const int *attribList));
PFNWGLCREATECONTEXTATTRIBS_PROC *wglCreateContextAttribsARB = nullptr;

int X3D::X3DContextGL::Init_From_Hwnd(HWND hwnd)
{
    // TODO: Use WGL to create a context
    m_hdc = GetDC(hwnd);

    PIXELFORMATDESCRIPTOR pfd;
    ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 32;
    pfd.iLayerType = PFD_MAIN_PLANE;

    int nPixelFormat = ChoosePixelFormat(m_hdc, &pfd);
    if (SetPixelFormat(m_hdc, nPixelFormat, &pfd) == FALSE)
        return X3D_ERR_FAILED_DEVICE_INIT;
    HGLRC hrc = wglCreateContext(m_hdc);
    if (wglMakeCurrent(m_hdc, hrc) == FALSE)
        return X3D_ERR_FAILED_DEVICE_INIT;

    wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBS_PROC *)wglGetProcAddress("wglCreateContextAttribsARB");
    if (wglCreateContextAttribsARB == nullptr)
        return X3D_ERR_FAILED_DEVICE_INIT;

    int const create_attribs[] = { WGL_CONTEXT_MAJOR_VERSION_ARB, 3, WGL_CONTEXT_MINOR_VERSION_ARB, 3, 0 };
    HGLRC core_hrc = wglCreateContextAttribsARB(m_hdc, 0, create_attribs);
    wglMakeCurrent(NULL, NULL);
    wglMakeCurrent(m_hdc, core_hrc);

    X3DDevice &dev = m_devices.emplace_back();
    const GLubyte *vendor = glGetString(GL_VENDOR); // Returns the vendor
    const GLubyte *renderer = glGetString(GL_RENDERER); // Returns a hint to the model
    strcpy(dev.Name, (char *)renderer);
    strcpy(dev.Vendor, (char *)vendor);

    return Init();
}
#endif

void X3D::X3DContextGL::Clear(bool clear_color, bool clear_depth)
{
    GLbitfield mask = 0;
    if (clear_color)
        mask |= GL_COLOR_BUFFER_BIT;
    if (clear_depth)
        mask |= GL_DEPTH_BUFFER_BIT;

    glClear(mask);
}

void X3D::X3DContextGL::Set_Clear_Color(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
}

void X3D::X3DContextGL::Present()
{
    // TODO: swap
#ifdef _WIN32
    wglSwapLayerBuffers(m_hdc, WGL_SWAP_MAIN_PLANE);
#endif
}

void X3D::X3DContextGL::Set_Viewport(int x, int y, int w, int h)
{
    glViewport(x, y, w, h);
}

X3D::X3DShader *X3D::X3DContextGL::Create_Shader()
{
    return new X3DShaderGL();
}

X3D::X3DVertexBuffer *X3D::X3DContextGL::Create_Vertex_Buffer(size_t size)
{
    return new X3DVertexBufferGL(this, size);
}

X3D::X3DIndexBuffer *X3D::X3DContextGL::Create_Index_Buffer(size_t size)
{
    return new X3DIndexBufferGL(this, size);
}

X3D::X3DVertexLayout *X3D::X3DContextGL::Create_Vertex_Layout()
{
    return new X3DVertexLayoutGL();
}

X3D::X3DTexture *X3D::X3DContextGL::Create_Texture(int width, int height, X3DTextureFormat format, int mip_count)
{
    return new X3DTextureGL(width, height, format, mip_count + 1);
}

static GLenum GetPrimitiveTypeGL(X3D::X3DPrimitive type)
{
    switch (type) {
        case X3D::X3D_TRIANGLES:
            return GL_TRIANGLES;
        case X3D::X3D_TRIANGLE_STRIP:
            return GL_TRIANGLE_STRIP;
        default:
            return GL_NONE;
    }
}

int X3D::X3DContextGL::Draw_Indexed(X3DPrimitive type, int count, int baseIndex)
{
    assert(m_vb != nullptr);
    assert(m_ib != nullptr);

    const void *buffer_offset = (const void *)(baseIndex * sizeof(uint16_t));
    glDrawElementsBaseVertex(GetPrimitiveTypeGL(type), count, GL_UNSIGNED_SHORT, buffer_offset, 0);
    return X3D_ERR_OK;
}

int X3D::X3DContextGL::Draw_Indexed_Range(X3DPrimitive type, int count, int minVertex, int maxVertex, int baseIndex)
{
    assert(m_vb != nullptr);
    assert(m_ib != nullptr);

    const void *buffer_offset = (const void *)(baseIndex * sizeof(uint16_t));
    glDrawRangeElementsBaseVertex(
        GetPrimitiveTypeGL(type), minVertex, maxVertex, count, GL_UNSIGNED_SHORT, buffer_offset, 0);
    return X3D_ERR_OK;
}
