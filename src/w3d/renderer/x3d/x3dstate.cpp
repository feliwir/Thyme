#include "x3dstate.h"

void X3DState::Apply_Changes()
{
    for (int stage = 0; stage < MAX_TEXTURE_STAGES; stage++) {
        if (m_textures[stage].value != nullptr) {
            m_textures[stage].value->Apply(stage);
        }
    }
}