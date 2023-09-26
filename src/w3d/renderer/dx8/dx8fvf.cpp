/**
 * @file
 *
 * @author Jonathan Wilson
 * @author thomsons26
 *
 * @brief FVFInfo class
 *
 * @copyright Thyme is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "dx8fvf.h"
#ifdef BUILD_WITH_D3D8
#include <d3dx8.h>
#elif defined BUILD_WITH_X3D
#include <x3dvertexbuffer.h>
#endif

unsigned int Get_FVF_Vertex_Size(unsigned int fvf)
{
#ifdef BUILD_WITH_D3D8
    return D3DXGetFVFVertexSize(fvf);
#elif defined BUILD_WITH_X3D
    unsigned int size = 0;
    if (fvf & X3D_VF_XYZ)
        size += sizeof(float) * 3;
    // TODO: ADD OTHERS
    return size;
#endif
}

FVFInfoClass::FVFInfoClass(unsigned int fvf_, unsigned int fvf_size_)
{
    m_FVF = fvf_;
    if (fvf_) {
        m_fvfSize = Get_FVF_Vertex_Size(m_FVF);
    } else {
        m_fvfSize = fvf_size_;
    }

    m_locationOffset = 0;
    m_blendOffset = 0;
#if defined BUILD_WITH_D3D8
    if ((m_FVF & D3DFVF_XYZ) == D3DFVF_XYZ) {
        m_blendOffset = 0x0C;
    }

    m_normalOffset = m_blendOffset;

    if (((m_FVF & D3DFVF_XYZB4) == D3DFVF_XYZB4) && ((m_FVF & D3DFVF_LASTBETA_UBYTE4) == D3DFVF_LASTBETA_UBYTE4)) {
        m_normalOffset = m_blendOffset + 0x10;
    }

    m_diffuseOffset = m_normalOffset;

    if ((m_FVF & D3DFVF_NORMAL) == D3DFVF_NORMAL) {
        m_diffuseOffset = m_normalOffset + 0x0C;
    }

    m_specularOffset = m_diffuseOffset;

    if ((m_FVF & D3DFVF_DIFFUSE) == D3DFVF_DIFFUSE) {
        m_specularOffset = m_diffuseOffset + 4;
    }
    m_texcoordOffset[0] = m_specularOffset;
    if ((m_FVF & D3DFVF_SPECULAR) == D3DFVF_SPECULAR) {
        m_texcoordOffset[0] = m_specularOffset + 4;
    }

    int a = 1;

    for (unsigned int i = 0x0F; (i - 0xE) < 8; i++) {
        m_texcoordOffset[a] = m_texcoordOffset[a - 1];
        if ((m_FVF & (3 << i)) == (unsigned int)(3 << i)) {
            m_texcoordOffset[a] = m_texcoordOffset[a - 1] + 4;
        } else {
            m_texcoordOffset[a] = m_texcoordOffset[a - 1] + 8;
        }
        a++;
    }
#elif defined BUILD_WITH_X3D

#endif
}

// unimplemented, not required
// void FVFInfoClass::Get_FVF_Name(StringClass &fvfname) const
