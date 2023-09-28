#pragma once
#include <cstdint>
#include <cstdlib>

namespace X3D
{
class X3DShader
{
public:
    virtual int Build_VS_From_HLSL(const char *src) = 0;
    virtual int Build_VS_From_DXBC(uint8_t *bc, size_t bc_size) = 0;
};

} // namespace X3D