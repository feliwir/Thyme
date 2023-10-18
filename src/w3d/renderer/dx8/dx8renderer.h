/**
 * @file
 *
 * @author Jonathan Wilson
 *
 * @brief Mesh Renderer
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
#include "autopool.h"
#include "dx8polygonrenderer.h"
#include "meshmatdesc.h"
#include "meshmdl.h"
#include "multilist.h"
#include "renderer.h"
#include "shader.h"
#include "simplevec.h"
#include "vector.h"
#include "vector3.h"

class CameraClass;
class DX8FVFCategoryContainer;
class DecalMeshClass;
class DX8TextureCategoryClass;
class TextureClass;
class VertexMaterialClass;
class DX8PolygonRendererClass;
template<typename R> class PolyRenderTaskClass;
class MatPassTaskClass;
class IndexBufferClass;
class MeshClass;
class MaterialPassClass;
class MeshModelClass;
class Vertex_Split_Table;
class VertexBufferClass;

class DX8MeshRendererClass
{
public:
    DX8MeshRendererClass();
    ~DX8MeshRendererClass();

    void Init();
    void Shutdown();

    void Flush();
    void Clear_Pending_Delete_Lists();

    void Log_Statistics_String(bool only_visible);

    void Register_Mesh_Type(MeshModelClass *mmc);
    void Unregister_Mesh_Type(MeshModelClass *mmc);

    void Set_Camera(CameraClass *camera) { m_camera = camera; }
    CameraClass *Peek_Camera() { return m_camera; }

    void Add_To_Render_List(DecalMeshClass *decalmesh);

    void Enable_Lighting(bool enable) { m_enableLighting = enable; }

    void Invalidate(bool shutdown);

    static void Request_Log_Statistics();

protected:
    void Render_Decal_Meshes();

    bool m_enableLighting;
    CameraClass *m_camera;
    SimpleDynVecClass<MultiListClass<DX8FVFCategoryContainer> *> m_textureCategoryContainerListsRigid;
    MultiListClass<DX8FVFCategoryContainer> *m_textureCategoryContainerListSkin;
    DecalMeshClass *m_visibleDecalMeshes;
};

class DX8TextureCategoryClass : public TextureCategoryClass
{
public:
    DX8TextureCategoryClass(
        DX8FVFCategoryContainer *container, TextureClass **texs, ShaderClass shd, VertexMaterialClass *mat, int pass);
    ~DX8TextureCategoryClass() override;

    PolygonRendererClass *Create_Polygon_Renderer(unsigned int index_count,
        MeshModelClass *mmc,
        unsigned int vertex_offset,
        unsigned int index_offset,
        bool strip,
        unsigned int pass)
    {
        return new DX8PolygonRendererClass(index_count, mmc, this, vertex_offset, index_offset, strip, pass);
    }
    void Render() override;
};

class DX8FVFCategoryContainer : public FVFCategoryContainer
{
public:
    DX8FVFCategoryContainer(unsigned int FVF, bool sorting);
    ~DX8FVFCategoryContainer() override;

    TextureCategoryClass *Create_Texture_Category(
        TextureClass **texs, ShaderClass shd, VertexMaterialClass *mat, int pass) override
    {
        return new DX8TextureCategoryClass(this, texs, shd, mat, pass);
    }
    void Generate_Texture_Categories(Vertex_Split_Table &split_table, unsigned int vertex_offset) override;

    static unsigned int Define_FVF(MeshModelClass *mmc, bool enable_lighting);
};

class DX8RigidFVFCategoryContainer : public DX8FVFCategoryContainer
{
public:
    DX8RigidFVFCategoryContainer(unsigned int FVF, bool sorting);
    ~DX8RigidFVFCategoryContainer() override;

    void Add_Mesh(MeshModelClass *mmc) override;

    void Log(bool only_visible) override;

    bool Check_If_Mesh_Fits(MeshModelClass *mmc) override;

    void Render() override;

    void Add_Delayed_Visible_Material_Pass(MaterialPassClass *pass, MeshClass *mesh) override;
    void Render_Delayed_Procedural_Material_Passes() override;

protected:
    VertexBufferClass *m_vertexBuffer;
    int m_usedVertices;
    MatPassTaskClass *m_delayedVisibleMatpassHead;
    MatPassTaskClass *m_delayedVisibleMatpassTail;
};

class DX8SkinFVFCategoryContainer : public DX8FVFCategoryContainer
{
public:
    DX8SkinFVFCategoryContainer(bool sorting);
    ~DX8SkinFVFCategoryContainer() override;

    void Add_Mesh(MeshModelClass *mmc) override;

    void Log(bool only_visible) override;

    bool Check_If_Mesh_Fits(MeshModelClass *mmc) override;

    void Render() override;

    void Add_Delayed_Visible_Material_Pass(MaterialPassClass *pass, MeshClass *mesh) override;
    void Render_Delayed_Procedural_Material_Passes() override;

    void Add_Visible_Skin(MeshClass *mesh);

private:
    void Reset();
    void Clear_Visible_Skin_List();

    unsigned int m_visibleVertexCount;
    MeshClass *m_visibleSkinHead;
    MeshClass *m_visibleSkinTail;
};

#ifdef GAME_DLL
extern DX8MeshRendererClass &g_theDX8MeshRenderer;
#else
extern DX8MeshRendererClass g_theDX8MeshRenderer;
#endif
