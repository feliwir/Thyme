/**
 * @file
 *
 * @author Jonathan Wilson
 *
 * @brief Renderer utilities
 *
 * @copyright Thyme is free software: you can redistribute it and/or
 *            modify it under the terms of the GNU General Public License
 *            as published by the Free Software Foundation, either version
 *            2 of the License, or (at your option) any later version.
 *            A full copy of the GNU General Public License can be found in
 *            LICENSE
 */
#include "renderer.h"
#include <captainslog.h>

#ifndef GAME_DLL
DynamicVectorClass<Vector3> g_tempVertexBuffer;
DynamicVectorClass<Vector3> g_tempNormalBuffer;
MultiListClass<MeshModelClass> g_registeredMeshList;
#else
#include "hooker.h"
#endif

bool Compare_Materials(const VertexMaterialClass *a, const VertexMaterialClass *b)
{
    int crc1;
    int crc2;

    if (a != nullptr) {
        crc1 = a->Get_CRC();
    } else {
        crc1 = 0;
    }

    if (b != nullptr) {
        crc2 = b->Get_CRC();
    } else {
        crc2 = 0;
    }

    return crc1 == crc2;
}

bool Textures_Material_And_Shader_Booking_Struct::Add_Textures_Material_And_Shader(
    TextureClass **texs, VertexMaterialClass *mat, ShaderClass shd)
{
    for (unsigned int i = 0; i < m_addedTypeCount; i++) {
        bool b = true;

        for (int j = 0; j < 2; j++) {
            b = b && texs[j] == m_addedTextures[j][i];
        }

        if (b && Compare_Materials(mat, m_addedMaterials[i]) && shd == m_addedShaders[i]) {
            return false;
        }
    }

    captainslog_assert(m_addedTypeCount < MAX_ADDED_TYPE_COUNT);
    for (int k = 0; k < 2; k++) {
        m_addedTextures[k][m_addedTypeCount] = texs[k];
    }

    m_addedMaterials[m_addedTypeCount] = mat;
    m_addedShaders[m_addedTypeCount] = shd;
    m_addedTypeCount++;
    return true;
}

Vertex_Split_Table::Vertex_Split_Table(MeshModelClass *mmc) :
    m_mmc(mmc), m_npatchEnable(false), m_allocatedPolygonArray(false)
{
    m_polygonCount = mmc->Get_Polygon_Count();

    m_allocatedPolygonArray = true;
    m_polygonArray = new TriIndex[m_polygonCount];
    memcpy(m_polygonArray, mmc->Get_Polygon_Array(), m_mmc->Get_Polygon_Count() * sizeof(TriIndex));
}

Vertex_Split_Table::~Vertex_Split_Table()
{
    if (m_allocatedPolygonArray) {
        delete[] m_polygonArray;
    }
}

TextureClass *Vertex_Split_Table::Peek_Texture(unsigned int pidx, unsigned int pass, unsigned int stage)
{
    if (m_mmc->Has_Texture_Array(pass, stage)) {
        if (pidx < (unsigned int)m_mmc->Get_Polygon_Count()) {
            return m_mmc->Peek_Texture(pidx, pass, stage);
        } else {
            captainslog_dbgassert(0, "GapFillerClass removed");
            return nullptr;
        }
    } else {
        return m_mmc->Peek_Single_Texture(pass, stage);
    }
}

VertexMaterialClass *Vertex_Split_Table::Peek_Material(unsigned int pidx, unsigned int pass)
{
    if (m_mmc->Has_Material_Array(pass)) {
        if (pidx < (unsigned int)m_mmc->Get_Polygon_Count()) {
            return m_mmc->Peek_Material(m_mmc->Get_Polygon_Array()[pidx][0], pass);
        } else {
            captainslog_dbgassert(0, "GapFillerClass removed");
            return nullptr;
        }
    } else {
        return m_mmc->Peek_Single_Material(pass);
    }
}

ShaderClass Vertex_Split_Table::Peek_Shader(unsigned int pidx, unsigned int pass)
{
    if (m_mmc->Has_Shader_Array(pass)) {
        if (pidx < (unsigned int)m_mmc->Get_Polygon_Count()) {
            return m_mmc->Get_Shader(pidx, pass);
        } else {
            captainslog_dbgassert(0, "GapFillerClass removed");
            return ShaderClass();
        }
    } else {
        return m_mmc->Get_Single_Shader(pass);
    }
}