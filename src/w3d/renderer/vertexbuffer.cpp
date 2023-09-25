/**
 * @file
 *
 * @author Jonathan Wilson
 * @author tomsons26
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
#include "vertexbuffer.h"
#include "dx8vertexbuffer.h"
#include "w3d.h"

unsigned int g_vertexBufferCount;
unsigned int g_vertexBufferTotalVertices;
unsigned int g_vertexBufferTotalSize;
bool g_dynamicSortingVertexArrayInUse;
SortingVertexBufferClass *g_dynamicSortingVertexArray;
unsigned short g_dynamicSortingVertexArraySize;
unsigned short g_dynamicSortingVertexArrayOffset;
bool g_dynamicDX8VertexBufferInUse;

VertexBufferClass::VertexBufferClass(
    unsigned int type_, unsigned int fvf, unsigned short vertex_count_, unsigned int vertex_size) :
    m_type(type_), m_vertexCount(vertex_count_), m_engineRefs(0)
{
    captainslog_assert(m_vertexCount);
    captainslog_assert(m_type == BUFFER_TYPE_DX8 || m_type == BUFFER_TYPE_SORTING);
#ifndef BUILD_WITH_X3D
    captainslog_assert((fvf != 0 && vertex_size == 0) || (fvf == 0 && vertex_size != 0));
#endif
    m_fvfInfo = new FVFInfoClass(fvf, vertex_size);
    g_vertexBufferCount++;
    g_vertexBufferTotalVertices += m_vertexCount;
    g_vertexBufferTotalSize += m_vertexCount * m_fvfInfo->Get_FVF_Size();
}

VertexBufferClass::~VertexBufferClass()
{
    g_vertexBufferCount--;
    g_vertexBufferTotalVertices -= m_vertexCount;
    g_vertexBufferTotalSize -= m_vertexCount * m_fvfInfo->Get_FVF_Size();

    if (m_fvfInfo) {
        delete m_fvfInfo;
    }
}

unsigned int VertexBufferClass::Get_Total_Buffer_Count()
{
    return g_vertexBufferCount;
}

unsigned int VertexBufferClass::Get_Total_Allocated_Indices()
{
    return g_vertexBufferTotalVertices;
}

unsigned int VertexBufferClass::Get_Total_Allocated_Memory()
{
    return g_vertexBufferTotalSize;
}

void VertexBufferClass::Add_Engine_Ref() const
{
    m_engineRefs++;
}

void VertexBufferClass::Release_Engine_Ref()
{
    m_engineRefs--;
    captainslog_assert(m_engineRefs >= 0);
}

VertexBufferClass::WriteLockClass::WriteLockClass(VertexBufferClass *vertex_buffer, int flags) :
    VertexBufferLockClass(vertex_buffer)
{
    captainslog_assert(vertex_buffer);
    captainslog_assert(!vertex_buffer->Engine_Refs());
    vertex_buffer->Add_Ref();

    switch (vertex_buffer->Type()) {
        case BUFFER_TYPE_DX8: {
#ifdef BUILD_WITH_D3D8
            static_cast<DX8VertexBufferClass *>(vertex_buffer)
                ->Get_DX8_Vertex_Buffer()
                ->Lock(0, 0, reinterpret_cast<BYTE **>(&m_vertices), flags);
#endif
            break;
        }
        case BUFFER_TYPE_SORTING: {
            m_vertices = static_cast<SortingVertexBufferClass *>(vertex_buffer)->Get_Sorting_Vertex_Buffer();
            break;
        }
        default:
            captainslog_assert(0);
            break;
    }
}

VertexBufferClass::WriteLockClass::~WriteLockClass()
{
    switch (m_vertexBuffer->Type()) {
        case BUFFER_TYPE_DX8: {
#ifdef BUILD_WITH_D3D8
            static_cast<DX8VertexBufferClass *>(m_vertexBuffer)->Get_DX8_Vertex_Buffer()->Unlock();
#endif
            break;
        }
        case BUFFER_TYPE_SORTING:
            break;
        default:
            captainslog_assert(0);
            break;
    }

    m_vertexBuffer->Release_Ref();
}

VertexBufferClass::AppendLockClass::AppendLockClass(
    VertexBufferClass *vertex_buffer, unsigned int start_index, unsigned int index_range) :
    VertexBufferLockClass(vertex_buffer)
{
    captainslog_assert(vertex_buffer);
    captainslog_assert(!vertex_buffer->Engine_Refs());
    captainslog_assert(start_index + index_range <= vertex_buffer->Get_Vertex_Count());
    vertex_buffer->Add_Ref();

    switch (vertex_buffer->Type()) {
        case BUFFER_TYPE_DX8: {
#ifdef BUILD_WITH_D3D8
            static_cast<DX8VertexBufferClass *>(vertex_buffer)
                ->Get_DX8_Vertex_Buffer()
                ->Lock(vertex_buffer->FVF_Info().Get_FVF_Size() * start_index,
                    vertex_buffer->FVF_Info().Get_FVF_Size() * index_range,
                    reinterpret_cast<BYTE **>(&m_vertices),
                    0);
#endif
            break;
        }
        case BUFFER_TYPE_SORTING: {
            m_vertices = static_cast<SortingVertexBufferClass *>(vertex_buffer)->Get_Sorting_Vertex_Buffer() + start_index;
            break;
        }
        default:
            captainslog_assert(0);
            break;
    }
}

VertexBufferClass::AppendLockClass::~AppendLockClass()
{
    switch (m_vertexBuffer->Type()) {
        case BUFFER_TYPE_DX8: {
#ifdef BUILD_WITH_D3D8
            static_cast<DX8VertexBufferClass *>(m_vertexBuffer)->Get_DX8_Vertex_Buffer()->Unlock();
#endif
            break;
        }
        case BUFFER_TYPE_SORTING:
            break;
        default:
            captainslog_assert(0);
            break;
    }

    m_vertexBuffer->Release_Ref();
}

SortingVertexBufferClass::SortingVertexBufferClass(unsigned short m_vertexCount) :
    VertexBufferClass(BUFFER_TYPE_SORTING, DX8_FVF_XYZNDUV2, m_vertexCount, 0)
{
    m_vertexBuffer = new VertexFormatXYZNDUV2[m_vertexCount];
}

SortingVertexBufferClass::~SortingVertexBufferClass()
{
    delete[] m_vertexBuffer;
}