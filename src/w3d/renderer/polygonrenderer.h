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
#include "multilist.h"
#include "sortingrenderer.h"
#include "sphere.h"
class MeshModelClass;
class TextureCategoryClass;

class PolygonRendererClass : public MultiListObjectClass
{
public:
    PolygonRendererClass(unsigned int index_count,
        MeshModelClass *mmc,
        TextureCategoryClass *tex_cat,
        unsigned int vertex_offset,
        unsigned int index_offset,
        bool strip,
        unsigned int pass);
    PolygonRendererClass(const PolygonRendererClass &src, MeshModelClass *mmc);
    virtual ~PolygonRendererClass();

    virtual void Render(int offset) = 0;
    virtual void Render_Sorted(int offset, const SphereClass &sphere) = 0;

    void Set_Vertex_Index_Range(unsigned int min, unsigned int max);
    void Set_Texture_Category(TextureCategoryClass *category) { m_textureCategory = category; }

    unsigned int Get_Vertex_Offset() { return m_vertexOffset; }
    unsigned int Get_Index_Offset() { return m_indexOffset; }
    MeshModelClass *Get_Mesh_Model_Class() { return m_mmc; }
    TextureCategoryClass *Get_Texture_Category() { return m_textureCategory; }
    unsigned int Get_Pass() { return m_pass; }

    void Log();

protected:
    MeshModelClass *m_mmc;
    TextureCategoryClass *m_textureCategory;
    unsigned int m_indexOffset;
    unsigned int m_vertexOffset;
    unsigned int m_indexCount;
    unsigned int m_minVertexIndex;
    unsigned int m_vertexIndexRange;
    bool m_strip;
    unsigned int m_pass;
};

inline void PolygonRendererClass::Set_Vertex_Index_Range(unsigned int min, unsigned int max)
{
    m_minVertexIndex = min;
    m_vertexIndexRange = max;
}
