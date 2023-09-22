#include "x3d.h"
#include "gl/x3d_context_gl.h"

namespace X3D
{
static X3DContext *s_context = nullptr;

static X3DBackend GetPreferredBackend()
{
    return X3D_OPENGL;
}
} // namespace X3D

int X3D::Init(X3DBackend backend)
{
    if (s_context) {
        return X3D_ALREADY_INITIALIZED;
    }

    if (backend == X3D_AUTO) {
        backend = GetPreferredBackend();
    }

    switch (backend) {
        case X3D_OPENGL:
            s_context = new X3DContextGL;
            break;

#ifdef _WIN32
        case X3D_D3D9:
            break;

        case X3D_D3D11:
            break;
#endif

        default:
            return X3D_UNSUPPORTED;
    }

    return X3D_OK;
}

#ifdef _WIN32
int X3D::Init_From_Hwnd(X3DBackend backend, HWND hwnd)
{
    int result = Init(backend);
    if (result != X3D_OK)
        return result;

    result = s_context->Init_FromHwnd(hwnd);
    return result;
}
#endif

void X3D::Shutdown()
{
    delete s_context;
}