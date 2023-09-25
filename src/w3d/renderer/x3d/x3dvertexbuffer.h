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
#pragma once
#include "always.h"
#include "vertexbuffer.h"

#include <x3d.h>

class X3DVertexBufferClass : public VertexBufferClass
{
    IMPLEMENT_W3D_POOL(X3DVertexBufferClass)
    ALLOW_HOOKING
public:
    enum UsageType
    {
        USAGE_DEFAULT = 0,
        USAGE_DYNAMIC = 1,
        USAGE_SOFTWAREPROCESSING = 2,
    };
    X3DVertexBufferClass(unsigned int fvf, unsigned short vertex_count_, UsageType usage, unsigned int flags);
    X3DVertexBufferClass(
        Vector3 *vertices, Vector3 *normals, Vector2 *tex_coords, unsigned short VertexCount, UsageType usage);
    X3DVertexBufferClass(Vector3 *vertices, Vector2 *tex_coords, unsigned short VertexCount, UsageType usage);
    X3DVertexBufferClass(Vector3 *vertices,
        Vector3 *normals,
        Vector4 *diffuse,
        Vector2 *tex_coords,
        unsigned short VertexCount,
        UsageType usage);
    X3DVertexBufferClass(
        Vector3 *vertices, Vector4 *diffuse, Vector2 *tex_coords, unsigned short VertexCount, UsageType usage);
    ~X3DVertexBufferClass();
    void Copy(Vector3 *loc, Vector3 *norm, Vector2 *uv, unsigned int first_vertex, unsigned int count);
    void Copy(Vector3 *loc, unsigned int first_vertex, unsigned int count);
    void Copy(Vector3 *loc, Vector2 *uv, unsigned int first_vertex, unsigned int count);
    void Copy(Vector3 *loc, Vector3 *norm, unsigned int first_vertex, unsigned int count);
    void Copy(Vector3 *loc, Vector3 *norm, Vector2 *uv, Vector4 *diffuse, unsigned int first_vertex, unsigned int count);
    void Copy(Vector3 *loc, Vector2 *uv, Vector4 *diffuse, unsigned int first_vertex, unsigned int count);
    void Create_Vertex_Buffer(UsageType usage);
    X3D::VertexBuffer *Get_X3D_Vertex_Buffer() { return m_vertexBuffer; }

private:
    X3D::VertexBuffer *m_vertexBuffer;
};