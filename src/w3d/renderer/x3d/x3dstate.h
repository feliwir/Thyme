#pragma once
#include "lightenv.h"
#include "shader.h"
#include "texture.h"
#include "vertmaterial.h"

#ifdef BUILD_WITH_D3D8
#error "X3D and D3D8 cannot be compiled together"
#endif

// Some constants to control numbers of things.
enum
{
    MAX_TEXTURE_STAGES = 8,
    VERTEX_BUFFERS = 2,
    GFX_LIGHT_COUNT = 4,
};

template<typename T> struct X3DStateValue
{
    T value;
    bool dirty;
};

class X3DState
{
public:
    static void Apply_Changes();

    static void Set_Texture(int stage, TextureClass *texture)
    {
        if (stage >= MAX_TEXTURE_STAGES)
            return;
        if (m_textures[stage].value == texture)
            return;
        m_textures[stage].value = texture;
        m_textures[stage].dirty = true;
    }

    static void Set_Material(VertexMaterialClass *material)
    {
        if (m_material.value == material)
            return;
        m_material.value = material;
        m_material.dirty = true;
    }

    static void Set_Light_Environment(LightEnvironmentClass *light_env)
    {
        if (m_light_environment.value == light_env)
            return;
        m_light_environment.value = light_env;
        m_light_environment.dirty = true;
    }

    static void Set_Active_X3D_Shader(X3D::X3DShader *shader)
    {
        m_x3d_shader = shader;
        m_x3d_shader->Bind();
    }

    static X3D::X3DShader *Get_Active_X3D_Shader() { return m_x3d_shader; }

private:
    static X3D::X3DShader *m_x3d_shader;
    static X3DStateValue<ShaderClass> m_shader;
    static X3DStateValue<LightEnvironmentClass *> m_light_environment;
    static X3DStateValue<TextureClass *> m_textures[MAX_TEXTURE_STAGES];
    static X3DStateValue<VertexMaterialClass *> m_material;
};