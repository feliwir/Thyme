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
#include "x3dpolygonrenderer.h"
#include "meshmdl.h"
#include "x3drenderer.h"

X3DPolygonRendererClass::X3DPolygonRendererClass(unsigned int index_count,
    MeshModelClass *mmc,
    X3DTextureCategoryClass *tex_cat,
    unsigned int vertex_offset,
    unsigned int index_offset,
    bool strip,
    unsigned int pass) :
    m_mmc(mmc),
    m_textureCategory(tex_cat),
    m_indexOffset(index_offset),
    m_vertexOffset(vertex_offset),
    m_indexCount(index_count),
    m_minVertexIndex(0),
    m_vertexIndexRange(0),
    m_strip(strip),
    m_pass(pass)
{
    captainslog_assert(index_count);
    mmc->m_x3d_polygonRendererList.Add_Tail(this);
}

X3DPolygonRendererClass::X3DPolygonRendererClass(const X3DPolygonRendererClass &src, MeshModelClass *mmc) :
    m_mmc(mmc),
    m_textureCategory(src.m_textureCategory),
    m_indexOffset(src.m_indexOffset),
    m_vertexOffset(src.m_vertexOffset),
    m_indexCount(src.m_indexCount),
    m_minVertexIndex(src.m_minVertexIndex),
    m_vertexIndexRange(src.m_vertexIndexRange),
    m_strip(src.m_strip),
    m_pass(src.m_pass)
{
    mmc->m_x3d_polygonRendererList.Add_Tail(this);
}

void X3DPolygonRendererClass::Log() {}

X3DPolygonRendererClass::~X3DPolygonRendererClass()
{
    if (m_textureCategory) {
        m_textureCategory->Remove_Polygon_Renderer(this);
    }
}
