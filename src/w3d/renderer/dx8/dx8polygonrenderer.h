/**
 * @file
 *
 * @author Jonathan Wilson
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
#include "dx8wrapper.h"
#include "multilist.h"
#include "polygonrenderer.h"
#include "sortingrenderer.h"
#include "sphere.h"
class MeshModelClass;
class DX8TextureCategoryClass;

class DX8PolygonRendererClass : public PolygonRendererClass
{
public:
    DX8PolygonRendererClass(unsigned int index_count,
        MeshModelClass *mmc,
        TextureCategoryClass *tex_cat,
        unsigned int vertex_offset,
        unsigned int index_offset,
        bool strip,
        unsigned int pass) :
        PolygonRendererClass(index_count, mmc, tex_cat, vertex_offset, index_offset, strip, pass){};
    DX8PolygonRendererClass(const PolygonRendererClass &src, MeshModelClass *mmc) : PolygonRendererClass(src, mmc){};
    virtual ~DX8PolygonRendererClass(){};

    void Render(int offset) override;
    void Render_Sorted(int offset, const SphereClass &sphere) override;
};

inline void DX8PolygonRendererClass::Render(int offset)
{
    DX8Wrapper::Set_Index_Buffer_Index_Offset(offset);

    if (m_strip) {
        DX8Wrapper::Draw_Strip((unsigned short)m_indexOffset,
            (unsigned short)m_indexCount - 2,
            (unsigned short)m_minVertexIndex,
            (unsigned short)m_vertexIndexRange);
    } else {
        DX8Wrapper::Draw_Triangles((unsigned short)m_indexOffset,
            (unsigned short)m_indexCount / 3,
            (unsigned short)m_minVertexIndex,
            (unsigned short)m_vertexIndexRange);
    }
}

inline void DX8PolygonRendererClass::Render_Sorted(int offset, const SphereClass &sphere)
{
    DX8Wrapper::Set_Index_Buffer_Index_Offset(offset);

    SortingRendererClass::Insert_Triangles(sphere,
        (unsigned short)m_indexOffset,
        (unsigned short)(m_indexCount / 3),
        (unsigned short)m_minVertexIndex,
        (unsigned short)m_vertexIndexRange);
}