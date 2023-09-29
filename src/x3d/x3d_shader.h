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

    virtual int Build_PS_From_HLSL(const char *src) = 0;
    virtual int Build_PS_From_DXBC(uint8_t *bc, size_t bc_size) = 0;

    virtual int Link() = 0;
    virtual int Bind() = 0;
};

} // namespace X3D