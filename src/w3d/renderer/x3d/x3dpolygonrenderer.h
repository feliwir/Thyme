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
#include "polygonrenderer.h"
#include "sortingrenderer.h"
#include "sphere.h"
#include <x3d.h>
class MeshModelClass;
class X3DTextureCategoryClass;

class X3DPolygonRendererClass : public PolygonRendererClass
{
public:
    X3DPolygonRendererClass(unsigned int index_count,
        MeshModelClass *mmc,
        TextureCategoryClass *tex_cat,
        unsigned int vertex_offset,
        unsigned int index_offset,
        bool strip,
        unsigned int pass) :
        PolygonRendererClass(index_count, mmc, tex_cat, vertex_offset, index_offset, strip, pass){};
    virtual ~X3DPolygonRendererClass(){};

    void Render(int offset) override;
    void Render_Sorted(int offset, const SphereClass &sphere) override;
};

inline void X3DPolygonRendererClass::Render(int offset)
{
    // DX8Wrapper::Set_Index_Buffer_Index_Offset(offset);

     if (m_strip) {
        X3D::Draw_Indexed(X3D::X3D_TRIANGLE_STRIP, m_indexOffset, m_indexCount, m_minVertexIndex);
    } else {
        X3D::Draw_Indexed(X3D::X3D_TRIANGLES, m_indexOffset, m_indexCount, m_minVertexIndex);
    }
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