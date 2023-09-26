/**
 * @file
 *
 * @author feliwir
 *
 * @brief Polygon Renderer
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
#include "multilist.h"
#include "sortingrenderer.h"
#include "sphere.h"
class MeshModelClass;
class X3DTextureCategoryClass;

class X3DPolygonRendererClass : public MultiListObjectClass
{
public:
    X3DPolygonRendererClass(unsigned int index_count,
        MeshModelClass *mmc,
        X3DTextureCategoryClass *tex_cat,
        unsigned int vertex_offset,
        unsigned int index_offset,
        bool strip,
        unsigned int pass);
    X3DPolygonRendererClass(const X3DPolygonRendererClass &src, MeshModelClass *mmc);
    virtual ~X3DPolygonRendererClass();

    void Render(int offset);
    void Render_Sorted(int offset, const SphereClass &sphere);

    void Set_Vertex_Index_Range(unsigned int min, unsigned int max);
    void Set_Texture_Category(X3DTextureCategoryClass *category) { m_textureCategory = category; }

    unsigned int Get_Vertex_Offset() { return m_vertexOffset; }
    unsigned int Get_Index_Offset() { return m_indexOffset; }
    MeshModelClass *Get_Mesh_Model_Class() { return m_mmc; }
    X3DTextureCategoryClass *Get_Texture_Category() { return m_textureCategory; }
    unsigned int Get_Pass() { return m_pass; }

    void Log();

private:
    MeshModelClass *m_mmc;
    X3DTextureCategoryClass *m_textureCategory;
    unsigned int m_indexOffset;
    unsigned int m_vertexOffset;
    unsigned int m_indexCount;
    unsigned int m_minVertexIndex;
    unsigned int m_vertexIndexRange;
    bool m_strip;
    unsigned int m_pass;
};

inline void X3DPolygonRendererClass::Render(int offset)
{
    // DX8Wrapper::Set_Index_Buffer_Index_Offset(offset);

    // if (m_strip) {
    //     DX8Wrapper::Draw_Strip((unsigned short)m_indexOffset,
    //         (unsigned short)m_indexCount - 2,
    //         (unsigned short)m_minVertexIndex,
    //         (unsigned short)m_vertexIndexRange);
    // } else {
    //     DX8Wrapper::Draw_Triangles((unsigned short)m_indexOffset,
    //         (unsigned short)m_indexCount / 3,
    //         (unsigned short)m_minVertexIndex,
    //         (unsigned short)m_vertexIndexRange);
    // }
}

inline void X3DPolygonRendererClass::Render_Sorted(int offset, const SphereClass &sphere)
{
    // DX8Wrapper::Set_Index_Buffer_Index_Offset(offset);

    SortingRendererClass::Insert_Triangles(sphere,
        (unsigned short)m_indexOffset,
        (unsigned short)(m_indexCount / 3),
        (unsigned short)m_minVertexIndex,
        (unsigned short)m_vertexIndexRange);
}
inline void X3DPolygonRendererClass::Set_Vertex_Index_Range(unsigned int min, unsigned int max)
{
    m_minVertexIndex = min;
    m_vertexIndexRange = max;
}
