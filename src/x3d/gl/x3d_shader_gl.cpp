#include "x3d_shader_gl.h"
#include "x3d.h"
#undef min
#undef max
#include <Xsc/Xsc.h>
#include <cstring>
#include <iostream>
#include <sstream>

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
    glDebugMessageInsertARB(GL_DEBUG_SOURCE_APPLICATION_ARB,
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

int X3D::X3DShaderGL::Build_VS_From_HLSL(const char *src, const char *entry)
{
    auto input_stream = std::make_shared<std::istringstream>(src);

    Xsc::ShaderInput input_desc;
    input_desc.sourceCode = input_stream;
    input_desc.entryPoint = entry;
    input_desc.shaderVersion = Xsc::InputShaderVersion::HLSL3;
    input_desc.shaderTarget = Xsc::ShaderTarget::VertexShader;

    std::ostringstream output_stream;
    Xsc::ShaderOutput output_desc;
    output_desc.sourceCode = &output_stream;
    output_desc.options.explicitBinding = true;
    output_desc.vertexSemantics.emplace_back(Xsc::VertexSemantic{ "POSITION0", 0 });
    output_desc.vertexSemantics.emplace_back(Xsc::VertexSemantic{ "NORMAL0", 1 });
    output_desc.vertexSemantics.emplace_back(Xsc::VertexSemantic{ "TEXCOORD0", 2 });
    output_desc.options.writeGeneratorHeader = false;

    bool result = false;
    Xsc::StdLog log;
    try {
        result = Xsc::CompileShader(input_desc, output_desc, &log, NULL);
    } catch (const std::exception &e) {
        glDebugMessageInsertARB(GL_DEBUG_SOURCE_APPLICATION_ARB,
            GL_DEBUG_TYPE_ERROR_ARB,
            0,
            GL_DEBUG_SEVERITY_HIGH_ARB,
            strlen(e.what()),
            e.what());
    }

    if (!result) {
        log.PrintAll(true);
        return X3D_ERR_FAILED_SHADER_TRANSPILE;
    }

    return Compile_Shader(GL_VERTEX_SHADER, output_stream.str().c_str(), m_vertex);
}

int X3D::X3DShaderGL::Build_PS_From_DXBC(uint8_t *bc, size_t bc_size)
{
    return X3D_ERR_OK;
}

int X3D::X3DShaderGL::Build_PS_From_HLSL(const char *src, const char *entry)
{
    auto input_stream = std::make_shared<std::istringstream>(src);

    Xsc::ShaderInput input_desc;
    input_desc.sourceCode = input_stream;
    input_desc.entryPoint = entry;
    input_desc.shaderVersion = Xsc::InputShaderVersion::HLSL3;
    input_desc.shaderTarget = Xsc::ShaderTarget::FragmentShader;

    std::ostringstream output_stream;
    Xsc::ShaderOutput output_desc;
    output_desc.sourceCode = &output_stream;
    output_desc.options.explicitBinding = true;
    output_desc.options.writeGeneratorHeader = false;
    bool result = false;
    Xsc::StdLog log;
    try {
        result = Xsc::CompileShader(input_desc, output_desc, &log, NULL);
    } catch (const std::exception &e) {
        glDebugMessageInsertARB(GL_DEBUG_SOURCE_APPLICATION_ARB,
            GL_DEBUG_TYPE_ERROR_ARB,
            0,
            GL_DEBUG_SEVERITY_HIGH_ARB,
            strlen(e.what()),
            e.what());
    }

    if (!result) {
        log.PrintAll(true);
        return X3D_ERR_FAILED_SHADER_TRANSPILE;
    }

    return Compile_Shader(GL_FRAGMENT_SHADER, output_stream.str().c_str(), m_pixel);
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
    glDebugMessageInsertARB(GL_DEBUG_SOURCE_APPLICATION_ARB,
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

int X3D::X3DShaderGL::Set_Matrix4x4(const char *name, const float *matrix)
{
    if (m_program == 0) {
        return X3D_ERR_SHADER_NOT_LINKED;
    }
    GLint location = glGetUniformLocation(m_program, name);
    if (location == -1) {
        return X3D_ERR_UNIFORM_NOT_FOUND;
    }
    glUniformMatrix4fv(location, 1, GL_TRUE, matrix);
    return X3D_ERR_OK;
}