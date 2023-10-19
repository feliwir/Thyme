/**
 * @file
 *
 * @author feliwir
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
#include "meshmatdesc.h"
#include "meshmdl.h"
#include "multilist.h"
#include "renderer.h"
#include "shader.h"
#include "simplevec.h"
#include "vector.h"
#include "vector3.h"
#include "x3dpolygonrenderer.h"
#include "x3dstate.h"

class CameraClass;
class DecalMeshClass;
class TextureClass;
class VertexMaterialClass;
template<typename R> class PolyRenderTaskClass;
class MatPassTaskClass;
class IndexBufferClass;
class MeshClass;
class MaterialPassClass;
class MeshModelClass;
class Vertex_Split_Table;
class VertexBufferClass;
class X3DFVFCategoryContainer;

class X3DMeshRendererClass
{
    friend class X3DRigidFVFCategoryContainer;

public:
    X3DMeshRendererClass();
    ~X3DMeshRendererClass();

    void Init();
    void Shutdown();

    void Flush();
    void Clear_Pending_Delete_Lists();

    void Log_Statistics_String(bool only_visible);

    void Register_Mesh_Type(MeshModelClass *mmc);
    void Unregister_Mesh_Type(MeshModelClass *mmc);

    void Set_Light_Environment(LightEnvironmentClass *light_env) { m_light_env = light_env; }
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
    LightEnvironmentClass *m_light_env;
    SimpleDynVecClass<MultiListClass<X3DFVFCategoryContainer> *> m_textureCategoryContainerListsRigid;
    MultiListClass<X3DFVFCategoryContainer> *m_textureCategoryContainerListSkin;
    DecalMeshClass *m_visibleDecalMeshes;
};

class X3DTextureCategoryClass : public TextureCategoryClass
{
public:
    X3DTextureCategoryClass(
        X3DFVFCategoryContainer *container, TextureClass **texs, ShaderClass shd, VertexMaterialClass *mat, int pass);
    ~X3DTextureCategoryClass() override;

    PolygonRendererClass *Create_Polygon_Renderer(unsigned int index_count,
        MeshModelClass *mmc,
        unsigned int vertex_offset,
        unsigned int index_offset,
        bool strip,
        unsigned int pass)
    {
        return new X3DPolygonRendererClass(index_count, mmc, this, vertex_offset, index_offset, strip, pass);
    }
    void Render() override;
    X3D::X3DShader *Get_X3D_Shader();
};

class X3DFVFCategoryContainer : public FVFCategoryContainer
{
public:
    X3DFVFCategoryContainer(unsigned int FVF, bool sorting);
    ~X3DFVFCategoryContainer() override;

    TextureCategoryClass *Create_Texture_Category(
        TextureClass **texs, ShaderClass shd, VertexMaterialClass *mat, int pass) override
    {
        return new X3DTextureCategoryClass(this, texs, shd, mat, pass);
    }
    void Generate_Texture_Categories(Vertex_Split_Table &split_table, unsigned int vertex_offset) override;

    static unsigned int Define_FVF(MeshModelClass *mmc, bool enable_lighting);
    X3D::X3DShader *Get_X3D_Shader() const { return m_shader; }

protected:
    X3D::X3DVertexLayout *m_layout = nullptr;
    X3D::X3DShader *m_shader = nullptr;
};

class X3DRigidFVFCategoryContainer : public X3DFVFCategoryContainer
{
public:
    X3DRigidFVFCategoryContainer(unsigned int FVF, bool sorting);
    ~X3DRigidFVFCategoryContainer() override;

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

class X3DSkinFVFCategoryContainer : public X3DFVFCategoryContainer
{
public:
    X3DSkinFVFCategoryContainer(bool sorting);
    ~X3DSkinFVFCategoryContainer() override;

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

extern X3DMeshRendererClass g_theX3DMeshRenderer;