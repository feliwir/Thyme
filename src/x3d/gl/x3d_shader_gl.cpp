#include "x3d_shader_gl.h"
#include "x3d.h"
#include <cstring>
#include <hlsl2glsl.h>
#include <iostream>

int X3D::X3DShaderGL::Compile_Shader(GLenum type, const char *glsl_src, GLuint &result)
{
    GLuint shader = glCreateShader(type);
    GLint len = strlen(glsl_src);
    glShaderSource(shader, 1, &glsl_src, &len);
    glCompileShader(shader);

    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success) {
        result = shader;
        return X3D_ERR_OK;
    }

    GLint maxLength = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

    std::vector<GLchar> errorLog(maxLength);
    glGetShaderInfoLog(shader, maxLength, &maxLength, errorLog.data());
    glDebugMessageInsertARB(GL_DEBUG_SOURCE_SHADER_COMPILER_ARB,
        GL_DEBUG_TYPE_ERROR_ARB,
        0,
        GL_DEBUG_SEVERITY_HIGH_ARB,
        errorLog.size(),
        errorLog.data());
    glDeleteShader(shader);

    return X3D_ERR_FAILED_SHADER_COMPILE;
}

int X3D::X3DShaderGL::Build_VS_From_DXBC(uint8_t *bc, size_t bc_size)
{
    return X3D_ERR_OK;
}

int X3D::X3DShaderGL::Build_VS_From_HLSL(const char *src)
{
    ShHandle compiler = Hlsl2Glsl_ConstructCompiler(EShLangVertex);
    int success = Hlsl2Glsl_Parse(compiler, src, ETargetGLSL_120, nullptr, 0);
    if (success != 1) {
        const char *infoLog = Hlsl2Glsl_GetInfoLog(compiler);
        glDebugMessageInsertARB(GL_DEBUG_SOURCE_SHADER_COMPILER_ARB,
            GL_DEBUG_TYPE_ERROR_ARB,
            0,
            GL_DEBUG_SEVERITY_HIGH_ARB,
            strlen(infoLog),
            infoLog);

        return X3D_ERR_FAILED_SHADER_TRANSPILE;
    }

    success = Hlsl2Glsl_Translate(compiler, "vs_main", ETargetGLSL_120, 0);
    if (success != 1) {
        return X3D_ERR_FAILED_SHADER_TRANSPILE;
    }

    const char *glsl_src = Hlsl2Glsl_GetShader(compiler);
    int result = Compile_Shader(GL_VERTEX_SHADER, glsl_src, m_vertex);
    Hlsl2Glsl_DestructCompiler(compiler);

    return result;
}

int X3D::X3DShaderGL::Build_PS_From_DXBC(uint8_t *bc, size_t bc_size)
{
    return X3D_ERR_OK;
}

int X3D::X3DShaderGL::Build_PS_From_HLSL(const char *src)
{
    ShHandle compiler = Hlsl2Glsl_ConstructCompiler(EShLangFragment);
    int success = Hlsl2Glsl_Parse(compiler, src, ETargetGLSL_110, nullptr, 0);
    if (success != 1) {
        const char *infoLog = Hlsl2Glsl_GetInfoLog(compiler);
        glDebugMessageInsertARB(GL_DEBUG_SOURCE_SHADER_COMPILER_ARB,
            GL_DEBUG_TYPE_ERROR_ARB,
            0,
            GL_DEBUG_SEVERITY_HIGH_ARB,
            strlen(infoLog),
            infoLog);

        return X3D_ERR_FAILED_SHADER_TRANSPILE;
    }

    success = Hlsl2Glsl_Translate(compiler, "ps_main", ETargetGLSL_110, 0);
    if (success != 1) {
        return X3D_ERR_FAILED_SHADER_TRANSPILE;
    }

    const char *glsl_src = Hlsl2Glsl_GetShader(compiler);
    int result = Compile_Shader(GL_FRAGMENT_SHADER, glsl_src, m_pixel);
    Hlsl2Glsl_DestructCompiler(compiler);

    return result;
}

int X3D::X3DShaderGL::Link()
{
    if (m_vertex == 0 || m_pixel == 0) {
        return X3D_ERR_FAILED_SHADER_LINKING;
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, m_vertex);
    glAttachShader(program, m_pixel);
    glLinkProgram(program);
    glDetachShader(program, m_vertex);
    glDetachShader(program, m_pixel);
    glDeleteShader(m_vertex);
    glDeleteShader(m_pixel);

    GLint success = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (success) {
        m_program = program;
        return X3D_ERR_OK;
    }

    GLint maxLength = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

    std::vector<GLchar> infoLog(maxLength);
    glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
    glDebugMessageInsertARB(GL_DEBUG_SOURCE_SHADER_COMPILER_ARB,
        GL_DEBUG_TYPE_ERROR_ARB,
        0,
        GL_DEBUG_SEVERITY_HIGH_ARB,
        infoLog.size(),
        infoLog.data());

    glDeleteProgram(program);

    return X3D_ERR_FAILED_SHADER_LINKING;
}

int X3D::X3DShaderGL::Bind()
{
    if (!m_program) {
        return X3D_ERR_FAILED_SHADER_BIND;
    }

    glUseProgram(m_program);
    return X3D_ERR_OK;
}