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
#pragma once
#include "always.h"
#include "indexbuffer.h"
#include <x3d_buffer.h>

class X3DIndexBufferClass : public IndexBufferClass
{
    IMPLEMENT_W3D_POOL(X3DIndexBufferClass)
public:
    enum UsageType
    {
        USAGE_DEFAULT,
        USAGE_DYNAMIC,
        USAGE_SOFTWAREPROCESSING
    };
    X3DIndexBufferClass(unsigned short index_count_, UsageType usage);
    ~X3DIndexBufferClass();
    X3D::X3DIndexBuffer *Get_X3D_Index_Buffer() { return m_indexBuffer; }

private:
    X3D::X3DIndexBuffer *m_indexBuffer;
};