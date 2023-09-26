/**
 * @file
 *
 * @author feliwir
 *
 * @brief Vertex Buffer classes
 *
 * @copyright Thyme is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "x3dvertexbuffer.h"
#include "x3d_vertexbuffer.h"
#include "w3d.h"

X3DVertexBufferClass::X3DVertexBufferClass(
    unsigned int fvf, unsigned short vertex_count_, UsageType usage, unsigned int flags) :
    VertexBufferClass(BUFFER_TYPE_X3D, fvf, vertex_count_, flags), m_vertexBuffer(nullptr)
{
    Create_Vertex_Buffer(usage);
}

X3DVertexBufferClass::~X3DVertexBufferClass()
{
    delete m_vertexBuffer;
}

void X3DVertexBufferClass::Create_Vertex_Buffer(UsageType usage)
{
#ifdef BUILD_WITH_D3D8
    captainslog_assert(!m_vertexBuffer);
    int d3dusage = ((usage & USAGE_DYNAMIC) < 1 ? D3DUSAGE_WRITEONLY : D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY)
        | ((usage & USAGE_SOFTWAREPROCESSING) >= 1 ? D3DUSAGE_SOFTWAREPROCESSING : 0);

    if (!DX8Wrapper::Get_Current_Caps()->Use_Hardware_TnL()) {
        d3dusage |= D3DUSAGE_SOFTWAREPROCESSING;
    }

    HRESULT res;
    DX8CALL_HRES(CreateVertexBuffer(m_fvfInfo->Get_FVF_Size() * m_vertexCount,
                     d3dusage,
                     m_fvfInfo->Get_FVF(),
                     (D3DPOOL)(unsigned __int8)(usage & 1 ^ 1),
                     &m_vertexBuffer),
        res);

    if (FAILED(res)) {
        captainslog_warn("Vertex buffer creation failed, trying to release assets...");
        TextureBaseClass::Invalidate_Old_Unused_Textures(5000);
        W3D::Invalidate_Mesh_Cache();
        DX8CALL(ResourceManagerDiscardBytes(0));
        DX8CALL_HRES(CreateVertexBuffer(m_fvfInfo->Get_FVF_Size() * m_vertexCount,
                         d3dusage,
                         m_fvfInfo->Get_FVF(),
                         (D3DPOOL)(unsigned __int8)(usage & 1 ^ 1),
                         &m_vertexBuffer),
            res);
        captainslog_warn("...Vertex buffer creation succesful");
    }
#endif
}