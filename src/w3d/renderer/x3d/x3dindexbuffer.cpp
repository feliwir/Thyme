/**
 * @file
 *
 * @author feliwir
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
#include "x3dindexbuffer.h"
#include "w3d.h"
#include <x3d.h>

// bool g_dynamicDX8IndexBufferInUse;
// X3DIndexBufferClass *g_dynamicDX8IndexBuffer;
// unsigned short g_dynamicDX8IndexBufferSize = 5000;
// unsigned short g_dynamicDX8IndexBufferOffset;

extern unsigned int g_indexBufferCount;
extern unsigned int g_indexBufferTotalIndices;
extern unsigned int g_indexBufferTotalSize;
extern bool g_dynamicSortingIndexArrayInUse;
extern SortingIndexBufferClass *g_dynamicSortingIndexArray;
extern unsigned short g_dynamicSortingIndexArraySize;
extern unsigned short g_dynamicSortingIndexArrayOffset;

X3DIndexBufferClass::X3DIndexBufferClass(unsigned short index_count_, UsageType usage) :
    IndexBufferClass(BUFFER_TYPE_X3D, index_count_)
{
    m_indexBuffer = X3D::Create_Index_Buffer(index_count_ * sizeof(uint16_t));
}

X3DIndexBufferClass::~X3DIndexBufferClass()
{
    delete m_indexBuffer;
}
