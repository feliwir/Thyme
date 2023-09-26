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
#include "w3d.h"
#include "x3d_vertexbuffer.h"

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
    captainslog_assert(!m_vertexBuffer);

    m_vertexBuffer = X3D::Create_Vertex_Buffer(m_fvfInfo->Get_FVF_Size() * m_vertexCount);

    if (m_vertexBuffer == nullptr) {
        captainslog_warn("Vertex buffer creation failed, trying to release assets...");
    }
}