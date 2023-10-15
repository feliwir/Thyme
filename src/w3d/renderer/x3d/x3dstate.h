#pragma once
#include "shader.h"
#include "texture.h"

// Some constants to control numbers of things.
enum
{
    MAX_TEXTURE_STAGES = 8,
    VERTEX_BUFFERS = 2,
    GFX_LIGHT_COUNT = 4,
};

template<typename T>
struct X3DStateValue
{
    T value;
    bool dirty;
};

class X3DState
{
    public:
    void Apply_Changes();

    void Set_Texture(int stage, TextureClass *texture)
    {
        if(stage >= MAX_TEXTURE_STAGES)
            return;
        if(m_textures[stage].value == texture)
            return;
        m_textures[stage].value = texture;
        m_textures[stage].dirty = true;
    }

    private:
    X3DStateValue<ShaderClass> m_shader;
    X3DStateValue<TextureClass*> m_textures[MAX_TEXTURE_STAGES];
};