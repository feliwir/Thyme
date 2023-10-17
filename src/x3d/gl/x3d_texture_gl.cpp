#include "x3d_texture_gl.h"
#include <cassert>

X3D::X3DTextureGL::X3DTextureGL(int width, int height, X3DTextureFormat fmt, int levels) : X3DTexture(width, height, fmt, levels)
{
    glGenTextures(1, &m_texture);
    m_pbos = new GLuint[m_levels];
}

X3D::X3DTextureGL::~X3DTextureGL()
{
    glDeleteTextures(1, &m_texture);
    delete[] m_pbos;
}

void X3D::X3DTextureGL::Upload(uint8_t *data, size_t size, int level)
{
    glBindTexture(GL_TEXTURE_2D, m_texture);
    switch (m_fmt) {
        case X3D_FMT_RGBA8:
            glTexImage2D(GL_TEXTURE_2D, level, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            break;
        case X3D_FMT_RGB8:
            glTexImage2D(GL_TEXTURE_2D, level, GL_RGB8, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            break;
        case X3D_FMT_RGBA16F:
            glTexImage2D(GL_TEXTURE_2D, level, GL_RGBA16F, m_width, m_height, 0, GL_RGBA, GL_FLOAT, data);
            break;
        case X3D_FMT_RGB16F:
            glTexImage2D(GL_TEXTURE_2D, level, GL_RGB16F, m_width, m_height, 0, GL_RGB, GL_FLOAT, data);
            break;
        case X3D_FMT_RGBA32F:
            glTexImage2D(GL_TEXTURE_2D, level, GL_RGBA32F, m_width, m_height, 0, GL_RGBA, GL_FLOAT, data);
            break;
        case X3D_FMT_RGB32F:
            glTexImage2D(GL_TEXTURE_2D, level, GL_RGB32F, m_width, m_height, 0, GL_RGB, GL_FLOAT, data);
            break;
        default:
            break;
    }
}

void* X3D::X3DTextureGL::Lock(X3DLockUsage usage, int level)
{
    assert(level < m_levels);
    glGenBuffers(1, &m_pbos[level]);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_pbos[level]);
    glBufferData(GL_PIXEL_UNPACK_BUFFER, m_width * m_height * 4, 0, GL_STREAM_DRAW);
    return glMapBuffer(GL_PIXEL_UNPACK_BUFFER, (usage == X3D_LOCK_WRITE) ? GL_WRITE_ONLY : GL_READ_ONLY);
}

void X3D::X3DTextureGL::Unlock(int level)
{
    // Copy changes to texture
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_pbos[level]);
    Upload(NULL, 0, level);
    glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    Unbind(0);
    glFlush();
    glFinish();
    glDeleteBuffers(1, &m_pbos[level]);
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