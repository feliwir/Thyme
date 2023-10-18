#pragma once
#include "x3d_texture.h"

#include <flextGL.h>

namespace X3D
{
class X3DTextureGL : public X3DTexture
{
public:
    X3DTextureGL(int width, int height, X3DTextureFormat fmt, int levels = 1);
    ~X3DTextureGL();

    void Upload(uint8_t *data, size_t size, int level = 0) override;
    void *Lock(X3DLockUsage, int level = 0) override;
    void Unlock(int level = 0) override;

    void Bind(int slot) override;
    void Unbind(int slot) override;

protected:
    GLuint m_texture = 0;
    // GLuint *m_pbos = nullptr;
    uint8_t **m_locked_levels = nullptr;
};
} // namespace X3D