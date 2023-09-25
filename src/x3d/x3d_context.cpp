#include "x3d_context.h"
#include "x3d.h"
#include <cassert>

int X3D::X3DContext::Set_Device(
    int dev, int resx, int resy, int bits, int windowed, bool resize_window, bool reset_device, bool restore_assets)
{
    m_dev = dev;
    m_resx = resx;
    m_resy = resy;
    m_bits = bits;
    m_windowed = windowed;
    return X3D_ERR_OK;
}

void X3D::X3DContext::Set_Resolution(int resx, int resy, int bits, int windowed, bool resize_window)
{
    m_resx = resx;
    m_resy = resy;
    m_bits = bits;
    m_windowed = windowed;
}

void X3D::X3DContext::Get_Resolution(int &resx, int &resy)
{
    resx = m_resx;
    resy = m_resy;
}

void X3D::X3DContext::Begin()
{
    assert(!m_rendering);
    m_rendering = true;
}

void X3D::X3DContext::End()
{
    assert(m_rendering);
    m_rendering = false;
}