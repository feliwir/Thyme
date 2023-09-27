#include "indexbuffer.h"
#include "dx8indexbuffer.h"
#ifdef BUILD_WITH_X3D
#include "x3dindexbuffer.h"
#endif

unsigned int g_indexBufferCount;
unsigned int g_indexBufferTotalIndices;
unsigned int g_indexBufferTotalSize;
bool g_dynamicSortingIndexArrayInUse;
SortingIndexBufferClass *g_dynamicSortingIndexArray;
unsigned short g_dynamicSortingIndexArraySize;
unsigned short g_dynamicSortingIndexArrayOffset;

IndexBufferClass::IndexBufferClass(unsigned int type_, unsigned short index_count_) :
    m_engineRefs(0), m_indexCount(index_count_), m_type(type_)
{
#ifdef BUILD_WITH_X3D
    captainslog_assert(m_type == BUFFER_TYPE_DX8 || m_type == BUFFER_TYPE_X3D || m_type == BUFFER_TYPE_SORTING);
#else
    captainslog_assert(m_type == BUFFER_TYPE_DX8 || m_type == BUFFER_TYPE_SORTING);
#endif
    captainslog_assert(m_indexCount);
    g_indexBufferCount++;
    g_indexBufferTotalIndices += m_indexCount;
    g_indexBufferTotalSize += 2 * m_indexCount;
}

IndexBufferClass::~IndexBufferClass()
{
    g_indexBufferCount--;
    g_indexBufferTotalIndices -= m_indexCount;
    g_indexBufferTotalSize -= 2 * m_indexCount;
}

void IndexBufferClass::Add_Engine_Ref() const
{
    m_engineRefs++;
}

void IndexBufferClass::Release_Engine_Ref()
{
    m_engineRefs--;
    captainslog_assert(m_engineRefs >= 0);
}

unsigned int IndexBufferClass::Get_Total_Buffer_Count()
{
    return g_indexBufferCount;
}

unsigned int IndexBufferClass::Get_Total_Allocated_Indices()
{
    return g_indexBufferTotalIndices;
}

unsigned int IndexBufferClass::Get_Total_Allocated_Memory()
{
    return g_indexBufferTotalSize;
}

IndexBufferClass::WriteLockClass::WriteLockClass(IndexBufferClass *index_buffer_, unsigned int flags) :
    m_indexBuffer(index_buffer_)
{
    captainslog_assert(m_indexBuffer);
    captainslog_assert(!m_indexBuffer->Engine_Refs());
    m_indexBuffer->Add_Ref();

    switch (m_indexBuffer->Type()) {
        case BUFFER_TYPE_DX8: {
            m_indices = nullptr;
#ifdef BUILD_WITH_D3D8
            static_cast<DX8IndexBufferClass *>(m_indexBuffer)
                ->Get_DX8_Index_Buffer()
                ->Lock(0, 0, reinterpret_cast<BYTE **>(&m_indices), flags);
#endif
            break;
        }
        case BUFFER_TYPE_SORTING: {
            m_indices = static_cast<SortingIndexBufferClass *>(m_indexBuffer)->Get_Sorting_Index_Buffer();
            break;
        }
        default:
            captainslog_assert(0);
            m_indices = nullptr;
            break;
    }
}

IndexBufferClass::WriteLockClass::~WriteLockClass()
{
    switch (m_indexBuffer->Type()) {
        case BUFFER_TYPE_DX8: {
#ifdef BUILD_WITH_D3D8
            static_cast<DX8IndexBufferClass *>(m_indexBuffer)->Get_DX8_Index_Buffer()->Unlock();
#endif
            break;
        }
        case BUFFER_TYPE_SORTING:
            break;
        default:
            captainslog_assert(0);
            break;
    }

    m_indexBuffer->Release_Ref();
}

IndexBufferClass::AppendLockClass::AppendLockClass(
    IndexBufferClass *index_buffer_, unsigned int start_index, unsigned int index_range) :
    m_indexBuffer(index_buffer_)
{
    captainslog_assert(start_index + index_range <= m_indexBuffer->Get_Index_Count());
    captainslog_assert(m_indexBuffer);
    captainslog_assert(!m_indexBuffer->Engine_Refs());
    m_indexBuffer->Add_Ref();

    switch (m_indexBuffer->Type()) {
        case BUFFER_TYPE_DX8: {
            m_indices = nullptr;
#ifdef BUILD_WITH_D3D8
            static_cast<DX8IndexBufferClass *>(m_indexBuffer)
                ->Get_DX8_Index_Buffer()
                ->Lock(sizeof(uint16_t) * start_index, sizeof(uint16_t) * index_range, (BYTE **)&m_indices, 0);
#endif
            break;
        }
#ifdef BUILD_WITH_X3D
        case BUFFER_TYPE_X3D: {
            m_indices = static_cast<uint16_t *>(
                static_cast<X3DIndexBufferClass *>(m_indexBuffer)
                    ->Get_X3D_Index_Buffer()
                    ->Lock(X3D::X3D_LOCK_WRITE, sizeof(uint16_t) * start_index, sizeof(uint16_t) * index_range));
            break;
        }
#endif // BUILD_WITH_X3D
        case BUFFER_TYPE_SORTING: {
            m_indices = static_cast<SortingIndexBufferClass *>(m_indexBuffer)->Get_Sorting_Index_Buffer() + start_index;
            break;
        }
        default:
            captainslog_assert(0);
            m_indices = nullptr;
            break;
    }
}

IndexBufferClass::AppendLockClass::~AppendLockClass()
{
    switch (m_indexBuffer->Type()) {
        case BUFFER_TYPE_DX8: {
#ifdef BUILD_WITH_D3D8
            static_cast<DX8IndexBufferClass *>(m_indexBuffer)->Get_DX8_Index_Buffer()->Unlock();
#endif
            break;
        }
#ifdef BUILD_WITH_X3D
        case BUFFER_TYPE_X3D: {
            static_cast<X3DIndexBufferClass *>(m_indexBuffer)->Get_X3D_Index_Buffer()->Unlock();
        }
#endif // BUILD_WITH_X3D

        case BUFFER_TYPE_SORTING:
            break;
        default:
            captainslog_assert(0);
            break;
    }

    m_indexBuffer->Release_Ref();
}

SortingIndexBufferClass::SortingIndexBufferClass(unsigned short index_count_) :
    IndexBufferClass(BUFFER_TYPE_SORTING, index_count_)
{
    captainslog_assert(m_indexCount);
    m_indexBuffer = new unsigned short[m_indexCount];
}

SortingIndexBufferClass::~SortingIndexBufferClass()
{
    delete[] m_indexBuffer;
}
