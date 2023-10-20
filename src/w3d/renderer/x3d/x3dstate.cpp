#include "x3dstate.h"
X3D::X3DShader *X3DState::m_x3d_shader;
X3DStateValue<ShaderClass> X3DState::m_shader;
X3DStateValue<TextureClass *> X3DState::m_textures[MAX_TEXTURE_STAGES];
X3DStateValue<VertexMaterialClass *> X3DState::m_material;
X3DStateValue<LightEnvironmentClass *> X3DState::m_light_environment;

void X3DState::Apply_Changes()
{
    for (int stage = 0; stage < MAX_TEXTURE_STAGES; stage++) {
        if (m_textures[stage].value != nullptr) {
            m_textures[stage].value->Apply(stage);
        }
    }

    if (m_material.value != nullptr)
        m_material.value->Apply();

    if (m_light_environment.value != nullptr) {
        if (m_x3d_shader != nullptr) {
            auto light_env = m_light_environment.value;
            m_x3d_shader->Set_Uniform_Vector3("LightAmbientColor", &light_env->Get_Equivalent_Ambient()[0]);
            int light_count = 0;
            for (light_count = 0; light_count < light_env->Get_Light_Count(); light_count++) {
                StringClass light_index_name;
                light_index_name.Format("Lights[%i]", light_count);
                m_x3d_shader->Set_Uniform_Int(light_index_name + ".lType", light_env->Is_Point_Light(light_count) ? 2 : 1);
                m_x3d_shader->Set_Uniform_Vector3(
                    light_index_name + ".lDirection", &light_env->Get_Light_Direction(light_count)[0]);
            }
        }
    }
}