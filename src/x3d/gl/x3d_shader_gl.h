#pragma once
#include "x3d_shader.h"

#include <flextGL.h>

namespace X3D
{
class X3DShaderGL : public X3DShader
{
public:
    int Build_VS_From_HLSL(const char *hlsl_src, const char *entry) override;
    int Build_VS_From_DXBC(uint8_t *dxbc, size_t dxbc_size) override;

    int Build_PS_From_HLSL(const char *hlsl_src, const char *entry) override;
    int Build_PS_From_DXBC(uint8_t *dxbc, size_t dxbc_size) override;

    int Link() override;
    int Bind() override;

    int Set_Uniform_Matrix4x4(const char *name, const float *matrix) override;
    int Set_Uniform_Vector3(const char *name, const float *vector) override;
protected:
    int Compile_Shader(GLenum type, const char *glsl_src, GLuint &shader);

    GLuint m_vertex = 0;
    GLuint m_pixel = 0;
    GLuint m_program = 0;
};
} // namespace X3D