#include "x3dstate.h"
X3D::X3DShader *X3DState::m_x3d_shader;
X3DStateValue<ShaderClass> X3DState::m_shader;
X3DStateValue<TextureClass *> X3DState::m_textures[MAX_TEXTURE_STAGES];
X3DStateValue<VertexMaterialClass *> X3DState::m_material;

void X3DState::Apply_Changes()
{
    for (int stage = 0; stage < MAX_TEXTURE_STAGES; stage++) {
        if (m_textures[stage].value != nullptr) {
            m_textures[stage].value->Apply(stage);
        }
    }

    if (m_material.value != nullptr)
        m_material.value->Apply();
}