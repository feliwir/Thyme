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
#include "vertexbuffer.h"

class FVFInfoClass;
class VertexBufferClass;
#define dynamic_fvf_type DX8_FVF_XYZNDUV2

class DX8VertexBufferClass : public VertexBufferClass
{
    IMPLEMENT_W3D_POOL(DX8VertexBufferClass)
    ALLOW_HOOKING
public:
    enum UsageType
    {
        USAGE_DEFAULT = 0,
        USAGE_DYNAMIC = 1,
        USAGE_SOFTWAREPROCESSING = 2,
    };
    DX8VertexBufferClass(unsigned int fvf, unsigned short vertex_count_, UsageType usage, unsigned int flags);
    DX8VertexBufferClass(
        Vector3 *vertices, Vector3 *normals, Vector2 *tex_coords, unsigned short VertexCount, UsageType usage);
    DX8VertexBufferClass(Vector3 *vertices, Vector2 *tex_coords, unsigned short VertexCount, UsageType usage);
    DX8VertexBufferClass(Vector3 *vertices,
        Vector3 *normals,
        Vector4 *diffuse,
        Vector2 *tex_coords,
        unsigned short VertexCount,
        UsageType usage);
    DX8VertexBufferClass(
        Vector3 *vertices, Vector4 *diffuse, Vector2 *tex_coords, unsigned short VertexCount, UsageType usage);
    ~DX8VertexBufferClass();
    void Copy(Vector3 *loc, Vector3 *norm, Vector2 *uv, unsigned int first_vertex, unsigned int count);
    void Copy(Vector3 *loc, unsigned int first_vertex, unsigned int count);
    void Copy(Vector3 *loc, Vector2 *uv, unsigned int first_vertex, unsigned int count);
    void Copy(Vector3 *loc, Vector3 *norm, unsigned int first_vertex, unsigned int count);
    void Copy(Vector3 *loc, Vector3 *norm, Vector2 *uv, Vector4 *diffuse, unsigned int first_vertex, unsigned int count);
    void Copy(Vector3 *loc, Vector2 *uv, Vector4 *diffuse, unsigned int first_vertex, unsigned int count);
    void Create_Vertex_Buffer(UsageType usage);
#ifdef BUILD_WITH_D3D8
    IDirect3DVertexBuffer8 *Get_DX8_Vertex_Buffer() { return m_vertexBuffer; }
#endif

private:
#ifdef GAME_DLL
    DX8VertexBufferClass *Hook_Ctor(unsigned int fvf, unsigned short vertex_count_, UsageType usage, unsigned int flags)
    {
        return new (this) DX8VertexBufferClass(fvf, vertex_count_, usage, flags);
    }
#endif
#ifdef BUILD_WITH_D3D8
    IDirect3DVertexBuffer8 *m_vertexBuffer;
#endif
};

class DynamicVBAccessClass
{
    ALLOW_HOOKING
public:
    class WriteLockClass
    {
        ALLOW_HOOKING
    public:
        WriteLockClass(DynamicVBAccessClass *dynamic_vb_access_);
        ~WriteLockClass();
        VertexFormatXYZNDUV2 *Get_Formatted_Vertex_Array()
        {
#ifdef BUILD_WITH_D3D8
            captainslog_assert(m_dynamicVBAccess->m_vertexBuffer->FVF_Info().Get_FVF()
                == (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX2 | D3DFVF_DIFFUSE));
#endif
            return m_vertices;
        }

    private:
#ifdef GAME_DLL
        WriteLockClass *Hook_Ctor(DynamicVBAccessClass *dynamic_vb_access_)
        {
            return new (this) WriteLockClass(dynamic_vb_access_);
        }
        void Hook_Dtor() { WriteLockClass::~WriteLockClass(); }
#endif
        DynamicVBAccessClass *m_dynamicVBAccess;
        VertexFormatXYZNDUV2 *m_vertices;
    };

public:
    DynamicVBAccessClass(unsigned int t, unsigned int fvf, unsigned short vertex_count_);
    ~DynamicVBAccessClass();
    void Allocate_Sorting_Dynamic_Buffer();
    void Allocate_DX8_Dynamic_Buffer();
    static void Reset(bool frame_changed);
    static void Deinit();
    static unsigned short Get_Default_Vertex_Count();
    FVFInfoClass &FVF_Info() const { return m_fvfInfo; }
    unsigned int Get_Type() const { return m_type; }
    unsigned short Get_Vertex_Count() const { return m_vertexCount; }
    unsigned short Get_Vertex_Offset() const { return m_vertexBufferOffset; }
    VertexBufferClass *Get_Vertex_Buffer() const { return m_vertexBuffer; }

private:
#ifdef GAME_DLL
    DynamicVBAccessClass *Hook_Ctor(unsigned int t, unsigned int fvf, unsigned short vertex_count_)
    {
        return new (this) DynamicVBAccessClass(t, fvf, vertex_count_);
    }
    void Hook_Dtor() { DynamicVBAccessClass::~DynamicVBAccessClass(); }
#endif
    FVFInfoClass &m_fvfInfo;
    unsigned int m_type;
    unsigned short m_vertexCount;
    unsigned short m_vertexBufferOffset;
    VertexBufferClass *m_vertexBuffer;
    friend class WriteLockClass;
};
