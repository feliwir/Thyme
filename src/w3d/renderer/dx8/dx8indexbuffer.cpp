/**
 * @file
 *
 * @author Jonathan Wilson
 *
 * @brief Index Buffer classes
 *
 * @copyright Thyme is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "dx8indexbuffer.h"
#include "dx8caps.h"
#include "dx8wrapper.h"
#include "w3d.h"

bool g_dynamicDX8IndexBufferInUse;
DX8IndexBufferClass *g_dynamicDX8IndexBuffer;
unsigned short g_dynamicDX8IndexBufferSize = 5000;
unsigned short g_dynamicDX8IndexBufferOffset;

extern unsigned int g_indexBufferCount;
extern unsigned int g_indexBufferTotalIndices;
extern unsigned int g_indexBufferTotalSize;
extern bool g_dynamicSortingIndexArrayInUse;
extern SortingIndexBufferClass *g_dynamicSortingIndexArray;
extern unsigned short g_dynamicSortingIndexArraySize;
extern unsigned short g_dynamicSortingIndexArrayOffset;

DX8IndexBufferClass::DX8IndexBufferClass(unsigned short index_count_, UsageType usage) :
    IndexBufferClass(BUFFER_TYPE_DX8, index_count_)
{
#ifdef BUILD_WITH_D3D8
    captainslog_assert(m_indexCount);
    int d3dusage = ((usage & USAGE_DYNAMIC) < 1 ? D3DUSAGE_WRITEONLY : D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY)
        | ((usage & USAGE_SOFTWAREPROCESSING) >= 1 ? D3DUSAGE_SOFTWAREPROCESSING : 0);

    if (!DX8Wrapper::Get_Current_Caps()->Use_Hardware_TnL()) {
        d3dusage |= D3DUSAGE_SOFTWAREPROCESSING;
    }

    HRESULT res;
    DX8CALL_HRES(CreateIndexBuffer(
                     2 * m_indexCount, d3dusage, D3DFMT_INDEX16, (D3DPOOL)(unsigned __int8)(usage & 1 ^ 1), &m_indexBuffer),
        res);

    if (FAILED(res)) {
        captainslog_warn("Index buffer creation failed, trying to release assets...");
        TextureBaseClass::Invalidate_Old_Unused_Textures(5000);
        W3D::Invalidate_Mesh_Cache();
        DX8CALL(ResourceManagerDiscardBytes(0));
        DX8CALL_HRES(
            CreateIndexBuffer(
                2 * m_indexCount, d3dusage, D3DFMT_INDEX16, (D3DPOOL)(unsigned __int8)(usage & 1 ^ 1), &m_indexBuffer),
            res);
        captainslog_warn("...Index buffer creation succesful");
    }
#endif
}

DX8IndexBufferClass::~DX8IndexBufferClass()
{
#ifdef BUILD_WITH_D3D8
    m_indexBuffer->Release();
#endif
}

DynamicIBAccessClass::DynamicIBAccessClass(unsigned short type_, unsigned short index_count_) :
    m_type(type_), m_indexCount(index_count_), m_indexBuffer(nullptr)
{
    captainslog_assert(
        m_type == IndexBufferClass::BUFFER_TYPE_DYNAMIC_DX8 || m_type == IndexBufferClass::BUFFER_TYPE_DYNAMIC_SORTING);

    if (m_type == IndexBufferClass::BUFFER_TYPE_DYNAMIC_DX8) {
        Allocate_DX8_Dynamic_Buffer();
    } else {
        Allocate_Sorting_Dynamic_Buffer();
    }
}

DynamicIBAccessClass::~DynamicIBAccessClass()
{
    Ref_Ptr_Release(m_indexBuffer);

    if (m_type == IndexBufferClass::BUFFER_TYPE_DYNAMIC_DX8) {
        g_dynamicDX8IndexBufferInUse = false;
        g_dynamicDX8IndexBufferOffset += m_indexCount;
    } else {
        g_dynamicSortingIndexArrayInUse = false;
        g_dynamicSortingIndexArrayOffset += m_indexCount;
    }
}

void DynamicIBAccessClass::Allocate_Sorting_Dynamic_Buffer()
{
    captainslog_assert(!g_dynamicSortingIndexArrayInUse);
    g_dynamicSortingIndexArrayInUse = true;
    int new_index_count = g_dynamicSortingIndexArrayOffset + m_indexCount;
    captainslog_assert(new_index_count < 65536);

    if (new_index_count > g_dynamicSortingIndexArraySize) {
        Ref_Ptr_Release(g_dynamicSortingIndexArray);
        g_dynamicSortingIndexArraySize = 5000;

        if (new_index_count > 4999) {
            g_dynamicSortingIndexArraySize = new_index_count;
        }
    }

    if (!g_dynamicSortingIndexArray) {
        g_dynamicSortingIndexArray = new SortingIndexBufferClass(g_dynamicSortingIndexArraySize);
        g_dynamicSortingIndexArrayOffset = 0;
    }

    g_dynamicSortingIndexArray->Add_Ref();
    Ref_Ptr_Release(m_indexBuffer);
    m_indexBuffer = g_dynamicSortingIndexArray;
    m_indexBufferOffset = g_dynamicSortingIndexArrayOffset;
}

void DynamicIBAccessClass::Allocate_DX8_Dynamic_Buffer()
{
    captainslog_assert(!g_dynamicDX8IndexBufferInUse);
    g_dynamicDX8IndexBufferInUse = true;

    if (m_indexCount > g_dynamicDX8IndexBufferSize) {
        Ref_Ptr_Release(g_dynamicDX8IndexBuffer);
        g_dynamicDX8IndexBufferSize = 5000;

        if (m_indexCount > 4999) {
            g_dynamicDX8IndexBufferSize = m_indexCount;
        }
    }

    if (!g_dynamicDX8IndexBuffer) {
        DX8IndexBufferClass::UsageType usage = DX8IndexBufferClass::USAGE_DYNAMIC;
        g_dynamicDX8IndexBuffer = new DX8IndexBufferClass(g_dynamicDX8IndexBufferSize, usage);
        g_dynamicDX8IndexBufferOffset = 0;
    }

    if (g_dynamicDX8IndexBufferOffset + m_indexCount > g_dynamicDX8IndexBufferSize) {
        g_dynamicDX8IndexBufferOffset = 0;
    }

    g_dynamicDX8IndexBuffer->Add_Ref();
    Ref_Ptr_Release(m_indexBuffer);
    m_indexBuffer = g_dynamicDX8IndexBuffer;
    m_indexBufferOffset = g_dynamicDX8IndexBufferOffset;
}

DynamicIBAccessClass::WriteLockClass::WriteLockClass(DynamicIBAccessClass *ib_access_) : m_dynamicIBAccess(ib_access_)
{
    m_dynamicIBAccess->m_indexBuffer->Add_Ref();

    if (m_dynamicIBAccess->m_type == IndexBufferClass::BUFFER_TYPE_DYNAMIC_DX8) {
        captainslog_assert(m_dynamicIBAccess != nullptr);
        m_indices = nullptr;
#ifdef BUILD_WITH_D3D8
        DX8IndexBufferClass *buffer = static_cast<DX8IndexBufferClass *>(m_dynamicIBAccess->m_indexBuffer);
        buffer->Get_DX8_Index_Buffer()->Lock(2 * m_dynamicIBAccess->m_indexBufferOffset,
            2 * m_dynamicIBAccess->m_indexCount,
            reinterpret_cast<BYTE **>(&m_indices),
            m_dynamicIBAccess->m_indexBufferOffset != 0 ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);
#endif
    } else if (m_dynamicIBAccess->m_type == IndexBufferClass::BUFFER_TYPE_DYNAMIC_SORTING) {
        SortingIndexBufferClass *buffer = static_cast<SortingIndexBufferClass *>(m_dynamicIBAccess->m_indexBuffer);
        m_indices = buffer->Get_Sorting_Index_Buffer() + m_dynamicIBAccess->m_indexBufferOffset;
    } else {
        captainslog_assert(0);
        m_indices = nullptr;
    }
}

DynamicIBAccessClass::WriteLockClass::~WriteLockClass()
{
    if (m_dynamicIBAccess->m_type == IndexBufferClass::BUFFER_TYPE_DYNAMIC_DX8) {
#ifdef BUILD_WITH_D3D8
        DX8IndexBufferClass *buffer = static_cast<DX8IndexBufferClass *>(m_dynamicIBAccess->m_indexBuffer);
        buffer->Get_DX8_Index_Buffer()->Unlock();
#endif
    } else if (m_dynamicIBAccess->m_type != IndexBufferClass::BUFFER_TYPE_DYNAMIC_SORTING) {
        captainslog_assert(0);
    }

    m_dynamicIBAccess->m_indexBuffer->Release_Ref();
}

void DynamicIBAccessClass::Deinit()
{
    if (g_dynamicDX8IndexBuffer) {
        captainslog_assert((g_dynamicDX8IndexBuffer == NULL) || (g_dynamicDX8IndexBuffer->Num_Refs() == 1));
        g_dynamicDX8IndexBuffer->Release_Ref();
    }

    g_dynamicDX8IndexBuffer = nullptr;
    g_dynamicDX8IndexBufferInUse = false;
    g_dynamicDX8IndexBufferSize = 5000;
    g_dynamicDX8IndexBufferOffset = 0;

    if (g_dynamicSortingIndexArray) {
        captainslog_assert((g_dynamicSortingIndexArray == NULL) || (g_dynamicSortingIndexArray->Num_Refs() == 1));
        g_dynamicSortingIndexArray->Release_Ref();
    }

    g_dynamicSortingIndexArray = nullptr;
    g_dynamicSortingIndexArrayInUse = false;
    g_dynamicSortingIndexArraySize = 0;
    g_dynamicSortingIndexArrayOffset = 0;
}

void DynamicIBAccessClass::Reset(bool frame_changed)
{
    g_dynamicSortingIndexArrayOffset = 0;

    if (frame_changed) {
        g_dynamicDX8IndexBufferOffset = 0;
    }
}

unsigned short DynamicIBAccessClass::Get_Default_Index_Count()
{
    return g_dynamicDX8IndexBufferSize;
}

unsigned short DynamicIBAccessClass::Get_Next_Index()
{
    return g_indexBufferTotalIndices + 1;
}

// unimplemented, not used
// void IndexBufferClass::Copy(unsigned short *indices, unsigned int first_index, unsigned int count)
// void IndexBufferClass::Copy(unsigned int *indices, unsigned int first_index, unsigned int count)
