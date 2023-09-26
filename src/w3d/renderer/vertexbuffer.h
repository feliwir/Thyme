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
#pragma once
#include "always.h"
#include "dx8fvf.h"
#include "refcount.h"
#include "vector2.h"
#include "vector3.h"
#include "vector4.h"
#include "w3dmpo.h"
#include "w3dtypes.h"

class FVFInfoClass;
class VertexBufferClass;
#define dynamic_fvf_type DX8_FVF_XYZNDUV2

struct VertexFormatXYZNDUV2
{
    float x;
    float y;
    float z;
    float nx;
    float ny;
    float nz;
    unsigned int diffuse;
    float u1;
    float v1;
    float u2;
    float v2;
};

struct VertexFormatXYZDUV1
{
    float x;
    float y;
    float z;
    unsigned int diffuse;
    float u1;
    float v1;
};

struct VertexFormatXYZUV1
{
    float x;
    float y;
    float z;
    float u1;
    float v1;
};

class VertexBufferLockClass
{
public:
    VertexBufferLockClass(VertexBufferClass *vertex_buffer) : m_vertexBuffer(vertex_buffer), m_vertices(nullptr) {}
    void *Get_Vertex_Array() const { return m_vertices; }

protected:
    VertexBufferClass *m_vertexBuffer;
    void *m_vertices;
};

class VertexBufferClass : public W3DMPO, public RefCountClass
{
    ALLOW_HOOKING
public:
    enum
    {
        BUFFER_TYPE_DX8,
        BUFFER_TYPE_SORTING,
        BUFFER_TYPE_DYNAMIC_DX8,
        BUFFER_TYPE_DYNAMIC_SORTING,
        BUFFER_TYPE_INVALID,
#ifdef BUILD_WITH_X3D
        BUFFER_TYPE_X3D,
#endif
    };
    class WriteLockClass : public VertexBufferLockClass
    {
        ALLOW_HOOKING
    public:
        WriteLockClass(VertexBufferClass *vertex_buffer, int flags);
        ~WriteLockClass();

#ifdef GAME_DLL
    private:
        WriteLockClass *Hook_Ctor(VertexBufferClass *vertex_buffer, int flags)
        {
            return new (this) WriteLockClass(vertex_buffer, flags);
        }
        void Hook_Dtor() { WriteLockClass::~WriteLockClass(); }
#endif
    };

    class AppendLockClass : public VertexBufferLockClass
    {
        ALLOW_HOOKING
    public:
        AppendLockClass(VertexBufferClass *vertex_buffer, unsigned int start_index, unsigned int index_range);
        ~AppendLockClass();

#ifdef GAME_DLL
    private:
        AppendLockClass *Hook_Ctor(VertexBufferClass *vertex_buffer, unsigned int start_index, unsigned int index_range)
        {
            return new (this) AppendLockClass(vertex_buffer, start_index, index_range);
        }
        void Hook_Dtor() { AppendLockClass::~AppendLockClass(); }
#endif
    };

public:
    VertexBufferClass(unsigned int type_, unsigned int fvf, unsigned short vertex_count_, unsigned int vertex_size);
    ~VertexBufferClass();
    void Add_Engine_Ref() const;
    void Release_Engine_Ref();
    FVFInfoClass &FVF_Info() { return *m_fvfInfo; }
    unsigned short Get_Vertex_Count() { return m_vertexCount; }
    unsigned int Type() const { return m_type; }
    unsigned int Engine_Refs() const { return m_engineRefs; }
    static unsigned int Get_Total_Buffer_Count();
    static unsigned int Get_Total_Allocated_Indices();
    static unsigned int Get_Total_Allocated_Memory();

protected:
    unsigned int m_type;
    unsigned short m_vertexCount;
    mutable int m_engineRefs;
    FVFInfoClass *m_fvfInfo;
};

class SortingVertexBufferClass : public VertexBufferClass
{
    IMPLEMENT_W3D_POOL(SortingVertexBufferClass)
    ALLOW_HOOKING
public:
    SortingVertexBufferClass(unsigned short VertexCount);
    ~SortingVertexBufferClass();
    VertexFormatXYZNDUV2 *Get_Sorting_Vertex_Buffer() { return m_vertexBuffer; }

private:
#ifdef GAME_DLL
    SortingVertexBufferClass *Hook_Ctor(unsigned short VertexCount)
    {
        return new (this) SortingVertexBufferClass(VertexCount);
    }
#endif
    VertexFormatXYZNDUV2 *m_vertexBuffer;
};