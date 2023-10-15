#include "x3d_texture_gl.h"

X3D::X3DTextureGL::X3DTextureGL(int width, int height, X3DTextureFormat fmt) : X3DTexture(width, height, fmt)
{
    glGenTextures(1, &m_texture);
}

X3D::X3DTextureGL::~X3DTextureGL()
{
    glDeleteTextures(1, &m_texture);
}

void X3D::X3DTextureGL::Upload(uint8_t *data, size_t size, int layer)
{
    glBindTexture(GL_TEXTURE_2D, m_texture);
    switch (m_fmt) {
        case X3D_FMT_RGBA8:
            glTexImage2D(GL_TEXTURE_2D, layer, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            break;
        case X3D_FMT_RGB8:
            glTexImage2D(GL_TEXTURE_2D, layer, GL_RGB8, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            break;
        case X3D_FMT_RGBA16F:
            glTexImage2D(GL_TEXTURE_2D, layer, GL_RGBA16F, m_width, m_height, 0, GL_RGBA, GL_FLOAT, data);
            break;
        case X3D_FMT_RGB16F:
            glTexImage2D(GL_TEXTURE_2D, layer, GL_RGB16F, m_width, m_height, 0, GL_RGB, GL_FLOAT, data);
            break;
        case X3D_FMT_RGBA32F:
            glTexImage2D(GL_TEXTURE_2D, layer, GL_RGBA32F, m_width, m_height, 0, GL_RGBA, GL_FLOAT, data);
            break;
        case X3D_FMT_RGB32F:
            glTexImage2D(GL_TEXTURE_2D, layer, GL_RGB32F, m_width, m_height, 0, GL_RGB, GL_FLOAT, data);
            break;
        default:
            break;
    }
}

void X3D::X3DTextureGL::Bind(int slot)
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_texture);
}

void X3D::X3DTextureGL::Unbind(int slot)
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, 0);
}