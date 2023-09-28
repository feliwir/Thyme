#pragma once
#include "x3d_shader.h"

namespace X3D
{
class X3DShaderGL : public X3DShader
{
  public:
    int Build_VS_From_HLSL(const char *src) override;
    int Build_VS_From_DXBC(uint8_t *bc, size_t bc_size) override;
};
} // namespace X3D