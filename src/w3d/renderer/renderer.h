/**
 * @file
 *
 * @author feliwir
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
#pragma once
#include "camera.h"
#include "decalmsh.h"
#include "mapper.h"
#include "matpass.h"
#include "mesh.h"
#include "meshmdl.h"
#include "shader.h"

#include <string.h>

class VertexMaterialClass;
bool Compare_Materials(const VertexMaterialClass *a, const VertexMaterialClass *b);

class TextureClass;
struct Textures_Material_And_Shader_Booking_Struct
{
    Textures_Material_And_Shader_Booking_Struct() : m_addedTypeCount(0)
    {
        memset(m_addedMaterials, 0, sizeof(m_addedMaterials));
        memset(m_addedTextures, 0, sizeof(m_addedTextures));
    }

    enum
    {
        MAX_ADDED_TYPE_COUNT = 64
    };

    bool Add_Textures_Material_And_Shader(TextureClass **texs, VertexMaterialClass *mat, ShaderClass shd);
    TextureClass *m_addedTextures[2][MAX_ADDED_TYPE_COUNT];
    VertexMaterialClass *m_addedMaterials[MAX_ADDED_TYPE_COUNT];
    ShaderClass m_addedShaders[MAX_ADDED_TYPE_COUNT];
    unsigned int m_addedTypeCount;
};

template<typename R>
class PolyRenderTaskClass : public AutoPoolClass<PolyRenderTaskClass<R>, 256>
{
public:
    PolyRenderTaskClass(R *renderer, MeshClass *mesh) :
        m_renderer(renderer), m_mesh(mesh), m_nextVisible(nullptr)
    {
        captainslog_assert(renderer != nullptr);
        captainslog_assert(mesh != nullptr);
        m_mesh->Add_Ref();
    }
    ~PolyRenderTaskClass() { m_mesh->Release_Ref(); }

    R *Peek_Polygon_Renderer() { return m_renderer; }
    MeshClass *Peek_Mesh() { return m_mesh; }

    PolyRenderTaskClass *Get_Next_Visible() { return m_nextVisible; }

    void Set_Next_Visible(PolyRenderTaskClass *visible) { m_nextVisible = visible; }

private:
    R *m_renderer;
    MeshClass *m_mesh;
    PolyRenderTaskClass *m_nextVisible;
};

class MatPassTaskClass : public AutoPoolClass<MatPassTaskClass, 256>
{
public:
    MatPassTaskClass(MaterialPassClass *pass, MeshClass *mesh) : m_materialPass(pass), m_mesh(mesh), m_nextVisible(nullptr)
    {
        captainslog_assert(m_materialPass != nullptr);
        captainslog_assert(m_mesh != nullptr);
        m_materialPass->Add_Ref();
        m_mesh->Add_Ref();
    }

    ~MatPassTaskClass()
    {
        m_materialPass->Release_Ref();
        m_mesh->Release_Ref();
    }

    MaterialPassClass *Peek_Material_Pass() { return m_materialPass; }
    MeshClass *Peek_Mesh() { return m_mesh; }

    MatPassTaskClass *Get_Next_Visible() { return m_nextVisible; }

    void Set_Next_Visible(MatPassTaskClass *task) { m_nextVisible = task; }

private:
    MaterialPassClass *m_materialPass;
    MeshClass *m_mesh;
    MatPassTaskClass *m_nextVisible;
};

class Vertex_Split_Table
{
public:
    Vertex_Split_Table(MeshModelClass *mmc);
    ~Vertex_Split_Table();

    const Vector3 *Get_Vertex_Array() { return m_mmc->Get_Vertex_Array(); }
    const Vector3 *Get_Vertex_Normal_Array() { return m_mmc->Get_Vertex_Normal_Array(); }
    const unsigned int *Get_Color_Array(unsigned int array_index) { return m_mmc->Get_Color_Array(array_index, false); }
    const Vector2 *Get_UV_Array(unsigned int array_index) { return m_mmc->Get_UV_Array_By_Index(array_index); }
    unsigned int Get_Vertex_Count() { return m_mmc->Get_Vertex_Count(); }
    unsigned int Get_Polygon_Count() { return m_polygonCount; }
    unsigned int Get_Pass_Count() { return m_mmc->Get_Pass_Count(); }
    MeshModelClass *Get_Mesh_Model_Class() { return m_mmc; }
    TriIndex *Get_Polygon_Array(unsigned int) { return m_polygonArray; }

    TextureClass *Peek_Texture(unsigned int pidx, unsigned int pass, unsigned int stage);
    VertexMaterialClass *Peek_Material(unsigned int pidx, unsigned int pass);
    ShaderClass Peek_Shader(unsigned int pidx, unsigned int pass);

private:
    MeshModelClass *m_mmc;
    bool m_npatchEnable;
    unsigned int m_polygonCount;
    TriIndex *m_polygonArray;
    bool m_allocatedPolygonArray;
};

#ifdef GAME_DLL
extern DynamicVectorClass<Vector3> &g_tempVertexBuffer;
extern DynamicVectorClass<Vector3> &g_tempNormalBuffer;
extern MultiListClass<MeshModelClass> &g_registeredMeshList;
#else
extern DynamicVectorClass<Vector3> g_tempVertexBuffer;
extern DynamicVectorClass<Vector3> g_tempNormalBuffer;
extern MultiListClass<MeshModelClass> g_registeredMeshList;
#endif
