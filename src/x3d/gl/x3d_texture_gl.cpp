#include "x3d_texture_gl.h"
#include <cassert>
#include <cstring>

X3D::X3DTextureGL::X3DTextureGL(int width, int height, X3DTextureFormat fmt, int levels) :
    X3DTexture(width, height, fmt, levels)
{
    glGenTextures(1, &m_texture);
    m_locked_levels = new uint8_t *[m_levels];
    for (int i = 0; i < levels; i++)
        m_locked_levels[i] = nullptr;
}

X3D::X3DTextureGL::~X3DTextureGL()
{
    glDeleteTextures(1, &m_texture);
    for (int level = 0; level < m_levels; level++) {
        if (m_locked_levels[level] != nullptr)
            delete[] m_locked_levels[level];
    }
    delete[] m_locked_levels;
}

void X3D::X3DTextureGL::Upload(uint8_t *data, size_t size, int level)
{
    assert(level < m_levels);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    switch (m_fmt) {
        case X3D_FMT_ARGB8:
            glTexImage2D(GL_TEXTURE_2D,
                level,
                GL_RGBA8,
                Get_Width(level),
                Get_Height(level),
                0,
                GL_BGRA,
                GL_UNSIGNED_INT_8_8_8_8_REV,
                data);
            break;
        case X3D_FMT_RGBA8:
            glTexImage2D(GL_TEXTURE_2D,
                level,
                GL_RGBA8,
                Get_Width(level),
                Get_Height(level),
                0,
                GL_RGBA,
                GL_UNSIGNED_INT_8_8_8_8,
                data);
            break;
        case X3D_FMT_RGB8:
            glTexImage2D(GL_TEXTURE_2D,
                level,
                GL_RGB8,
                Get_Width(level),
                Get_Height(level),
                0,
                GL_RGB,
                GL_UNSIGNED_INT_8_8_8_8,
                data);
            break;
        case X3D_FMT_RGBA16F:
            glTexImage2D(GL_TEXTURE_2D, level, GL_RGBA16F, Get_Width(level), Get_Height(level), 0, GL_RGBA, GL_FLOAT, data);
            break;
        case X3D_FMT_RGB16F:
            glTexImage2D(GL_TEXTURE_2D, level, GL_RGB16F, Get_Width(level), Get_Height(level), 0, GL_RGB, GL_FLOAT, data);
            break;
        case X3D_FMT_RGBA32F:
            glTexImage2D(GL_TEXTURE_2D, level, GL_RGBA32F, Get_Width(level), Get_Height(level), 0, GL_RGBA, GL_FLOAT, data);
            break;
        case X3D_FMT_RGB32F:
            glTexImage2D(GL_TEXTURE_2D, level, GL_RGB32F, Get_Width(level), Get_Height(level), 0, GL_RGB, GL_FLOAT, data);
            break;
        case X3D_FMT_BC1:
            glCompressedTexImage2D(
                GL_TEXTURE_2D, level, GL_COMPRESSED_RGB_S3TC_DXT1_EXT, Get_Width(level), Get_Height(level), 0, size, data);
            break;
        case X3D_FMT_BC3:
            glCompressedTexImage2D(
                GL_TEXTURE_2D, level, GL_COMPRESSED_RGBA_S3TC_DXT5_EXT, Get_Width(level), Get_Height(level), 0, size, data);
            break;
        default:
            break;
    }
}

// TODO: we had this implemented using PBO's - however it did not work
// Retry using PBOs in the future. Use a single PBO and always map ranges from it
void *X3D::X3DTextureGL::Lock(X3DLockUsage usage, int level)
{
    assert(level < m_levels);
    m_locked_levels[level] = new uint8_t[Get_Level_Bytesize(level)];
    return m_locked_levels[level];
}

void X3D::X3DTextureGL::Unlock(int level)
{
    assert(level < m_levels);
    assert(m_locked_levels[level] != nullptr);
    Upload(m_locked_levels[level], Get_Level_Bytesize(level), level);
    delete[] m_locked_levels[level];
    m_locked_levels[level] = nullptr;
}

void X3D::X3DTextureGL::Bind(int slot)
{
    glBindTexture(GL_TEXTURE_2D, m_texture);
}

void X3D::X3DTextureGL::Unbind(int slot)
{
    glBindTexture(GL_TEXTURE_2D, 0);
}