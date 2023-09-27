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
#include "dx8renderer.h"
#include "camera.h"
#include "decalmsh.h"
#include "dx8fvf.h"
#include "dx8indexbuffer.h"
#include "dx8polygonrenderer.h"
#include "dx8vertexbuffer.h"
#include "mapper.h"
#include "matpass.h"
#include "mesh.h"
#include "renderer.h"
#include "w3d.h"
#ifndef GAME_DLL
DX8MeshRendererClass g_theDX8MeshRenderer;
MultiListClass<TextureCategoryClass> g_textureCategoryDeleteList;
MultiListClass<FVFCategoryContainer> g_fvfCategoryContainerDeleteList;
#else
#include "hooker.h"
#endif

void DX8MeshRendererClass::Init()
{
    if (m_textureCategoryContainerListSkin == nullptr) {
        m_textureCategoryContainerListSkin = new MultiListClass<DX8FVFCategoryContainer>;
    }
}

void DX8MeshRendererClass::Shutdown()
{
    Invalidate(true);
    Clear_Pending_Delete_Lists();
    g_tempVertexBuffer.Clear();
    g_tempNormalBuffer.Clear();
}

void Invalidate_FVF_Category_Container_List(MultiListClass<DX8FVFCategoryContainer> &categoryContainer)
{
    for (DX8FVFCategoryContainer *i = categoryContainer.Remove_Head(); i != nullptr; i = categoryContainer.Remove_Head()) {
        delete i;
    }
}

void DX8MeshRendererClass::Invalidate(bool shutdown)
{
    g_registeredMeshList.Reset_List();

    for (int i = 0; i < m_textureCategoryContainerListsRigid.Count(); i++) {
        Invalidate_FVF_Category_Container_List(*m_textureCategoryContainerListsRigid[i]);
        delete m_textureCategoryContainerListsRigid[i];
    }

    if (m_textureCategoryContainerListSkin != nullptr) {
        Invalidate_FVF_Category_Container_List(*m_textureCategoryContainerListSkin);
        m_textureCategoryContainerListSkin = 0;
    }

    if (!shutdown) {
        m_textureCategoryContainerListSkin = new MultiListClass<DX8FVFCategoryContainer>;
    }

    m_textureCategoryContainerListsRigid.Delete_All();
}

void DX8MeshRendererClass::Clear_Pending_Delete_Lists()
{
    for (MultiListObjectClass *i = g_textureCategoryDeleteList.Remove_Head(); i != nullptr;
         i = g_textureCategoryDeleteList.Remove_Head()) {
        delete i;
    }

    for (MultiListObjectClass *i = g_fvfCategoryContainerDeleteList.Remove_Head(); i != nullptr;
         i = g_fvfCategoryContainerDeleteList.Remove_Head()) {
        delete i;
    }
}

void DX8MeshRendererClass::Request_Log_Statistics()
{
    // does nothing
}

DX8MeshRendererClass::DX8MeshRendererClass() :
    m_enableLighting(true), m_camera(nullptr), m_textureCategoryContainerListSkin(nullptr), m_visibleDecalMeshes(nullptr)
{
}

DX8MeshRendererClass::~DX8MeshRendererClass()
{
    Invalidate(true);
    Clear_Pending_Delete_Lists();

    if (m_textureCategoryContainerListSkin != nullptr) {
        delete m_textureCategoryContainerListSkin;
    }
}

void Render_FVF_Category_Container_List(MultiListClass<DX8FVFCategoryContainer> &containers)
{
    for (MultiListIterator<DX8FVFCategoryContainer> container(&containers); container; ++container) {
        container->Render();
    }
}

void Render_FVF_Category_Container_List_Delayed_Passes(MultiListClass<DX8FVFCategoryContainer> &containers)
{
    for (MultiListIterator<DX8FVFCategoryContainer> container(&containers); container; ++container) {
        container->Render_Delayed_Procedural_Material_Passes();
    }
}

void DX8MeshRendererClass::Flush()
{
    if (m_camera != nullptr) {
        Log_Statistics_String(true);

        for (int i = 0; i < m_textureCategoryContainerListsRigid.Count(); i++) {
            Render_FVF_Category_Container_List(*m_textureCategoryContainerListsRigid[i]);
        }

        Render_FVF_Category_Container_List(*m_textureCategoryContainerListSkin);
        Render_Decal_Meshes();

        for (int i = 0; i < m_textureCategoryContainerListsRigid.Count(); i++) {
            Render_FVF_Category_Container_List_Delayed_Passes(*m_textureCategoryContainerListsRigid[i]);
        }

        DX8Wrapper::Set_Vertex_Buffer(nullptr, 0);
        DX8Wrapper::Set_Index_Buffer(nullptr, 0);
    }
}

void DX8MeshRendererClass::Log_Statistics_String(bool only_visible)
{
    // does nothing
}

void Add_Rigid_Mesh_To_Container(
    MultiListClass<DX8FVFCategoryContainer> *container_list, unsigned int fvf, MeshModelClass *mmc)
{
    captainslog_assert(container_list != nullptr);
    bool sorting = W3D::Is_Sorting_Enabled() && mmc->Get_Flag(MeshGeometryClass::SORT) && mmc->Get_Sort_Level() == 0;

    for (MultiListIterator<DX8FVFCategoryContainer> container(container_list); container; ++container) {
        if (container->Is_Sorting() == sorting && container->Check_If_Mesh_Fits(mmc)) {
            container->Add_Mesh(mmc);
            return;
        }
    }

    DX8RigidFVFCategoryContainer *container = new DX8RigidFVFCategoryContainer(fvf, sorting);
    container_list->Add_Tail(container);
    container->Add_Mesh(mmc);
}

void DX8MeshRendererClass::Register_Mesh_Type(MeshModelClass *mmc)
{
    bool skin = mmc->Get_Flag(MeshGeometryClass::SKIN) && mmc->Get_Vertex_Bone_Links();
    bool sorting = mmc->Get_Flag(MeshGeometryClass::SORT) && W3D::Is_Sorting_Enabled() && mmc->Get_Sort_Level() == 0;

    if (skin) {
        captainslog_assert(m_textureCategoryContainerListSkin != nullptr);
        MultiListIterator<DX8FVFCategoryContainer> container(m_textureCategoryContainerListSkin);

        while (!container.Is_Done()) {
            if (container->Is_Sorting() == sorting && container->Check_If_Mesh_Fits(mmc)) {
                container->Add_Mesh(mmc);
                return;
            }

            container.Next();
        }

        DX8SkinFVFCategoryContainer *skin_fvf = new DX8SkinFVFCategoryContainer(sorting);
        m_textureCategoryContainerListSkin->Add_Tail(skin_fvf);
        skin_fvf->Add_Mesh(mmc);
    } else {
        captainslog_assert(g_registeredMeshList.Contains(mmc) == false);
        if (!g_registeredMeshList.Contains(mmc)) {

            unsigned int fvf = DX8FVFCategoryContainer::Define_FVF(mmc, m_enableLighting);

            for (int i = 0; i < m_textureCategoryContainerListsRigid.Count(); i++) {
                MultiListClass<DX8FVFCategoryContainer> *list = m_textureCategoryContainerListsRigid[i];
                captainslog_assert(list != nullptr);
                DX8FVFCategoryContainer *container = list->Get_Head();

                if (container == nullptr || container->Get_FVF() == fvf) {
                    Add_Rigid_Mesh_To_Container(list, fvf, mmc);
                    if (!mmc->m_polygonRendererList.Is_Empty()) {
                        g_registeredMeshList.Add(mmc);
                    }
                    return;
                }
            }

            MultiListClass<DX8FVFCategoryContainer> *containerList = new MultiListClass<DX8FVFCategoryContainer>;
            m_textureCategoryContainerListsRigid.Add(containerList);
            Add_Rigid_Mesh_To_Container(containerList, fvf, mmc);

            if (!mmc->m_polygonRendererList.Is_Empty()) {
                g_registeredMeshList.Add(mmc);
            }
        }
    }
}

void DX8MeshRendererClass::Unregister_Mesh_Type(MeshModelClass *mmc)
{
    for (DX8PolygonRendererClass *i = mmc->m_polygonRendererList.Remove_Head(); i != nullptr;
         i = mmc->m_polygonRendererList.Remove_Head()) {
        delete i;
    }

    g_registeredMeshList.Remove(mmc);
}

void DX8MeshRendererClass::Add_To_Render_List(DecalMeshClass *decalmesh)
{
    captainslog_assert(decalmesh != nullptr);
    decalmesh->Set_Next_Visible(m_visibleDecalMeshes);
    m_visibleDecalMeshes = decalmesh;
}

void DX8MeshRendererClass::Render_Decal_Meshes()
{
#ifdef BUILD_WITH_D3D8
    DecalMeshClass *d = m_visibleDecalMeshes;
    if (d != nullptr) {
        DX8Wrapper::Set_DX8_Render_State(D3DRS_ZBIAS, 8);

        do {
            d->Render();
            d = d->Peek_Next_Visible();
        } while (d != nullptr);

        m_visibleDecalMeshes = 0;
        DX8Wrapper::Set_DX8_Render_State(D3DRS_ZBIAS, 0);
    }
#endif
}

DX8TextureCategoryClass::DX8TextureCategoryClass(
    DX8FVFCategoryContainer *container, TextureClass **texs, ShaderClass shd, VertexMaterialClass *mat, int pass) :
    TextureCategoryClass(container, texs, shd, mat, pass)
{
    captainslog_assert(pass >= 0);
    captainslog_assert(pass < DX8FVFCategoryContainer::MAX_PASSES);

    for (int i = 0; i < 2; i++) {
        Ref_Ptr_Set(m_textures[i], texs[i]);
    }

    if (m_material != nullptr) {
        m_material->Add_Ref();
    }
}

DX8TextureCategoryClass::~DX8TextureCategoryClass()
{
    for (;;) {
        DX8PolygonRendererClass *r = m_polygonRendererList.Get_Head();

        if (r == nullptr) {
            break;
        }

        g_theDX8MeshRenderer.Unregister_Mesh_Type(r->Get_Mesh_Model_Class());
    }

    for (int i = 0; i < 2; i++) {
        Ref_Ptr_Release(m_textures[i]);
    }

    Ref_Ptr_Release(m_material);
}

void DX8TextureCategoryClass::Add_Render_Task(DX8PolygonRendererClass *p_renderer, MeshClass *p_mesh)
{
    auto task = new PolyRenderTaskClass(p_renderer, p_mesh);
    task->Set_Next_Visible(m_renderTaskHead);
    m_renderTaskHead = task;
    m_container->Add_Visible_Texture_Category(this, m_pass);
}

void DX8TextureCategoryClass::Render()
{
    for (int i = 0; i < 2; i++) {
        DX8Wrapper::Set_Texture(i, Peek_Texture(i));
    }

    DX8Wrapper::Set_Material(Peek_Material());
    ShaderClass shader = Get_Shader();
    ShaderClass shader2 = shader;
    shader2.Set_Src_Blend_Func(ShaderClass::SRCBLEND_SRC_ALPHA);
    shader2.Set_Dst_Blend_Func(ShaderClass::DSTBLEND_ONE_MINUS_SRC_ALPHA);
    DX8Wrapper::Set_Shader(shader);

    if (s_forceMultiply) {
        if (shader.Get_Dst_Blend_Func() == ShaderClass::DSTBLEND_ZERO) {
            shader.Set_Dst_Blend_Func(ShaderClass::DSTBLEND_SRC_COLOR);
            shader.Set_Src_Blend_Func(ShaderClass::SRCBLEND_ZERO);
            DX8Wrapper::Set_Shader(shader);
            DX8Wrapper::Apply_Render_State_Changes();
#ifdef BUILD_WITH_D3D8
            DX8Wrapper::Set_DX8_Render_State(D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR);
#endif
        }
    }

    bool b = false;
    auto task = m_renderTaskHead;
    PolyRenderTaskClass<DX8PolygonRendererClass> *task2 = nullptr;
    while (task != nullptr) {
        DX8PolygonRendererClass *renderer = task->Peek_Polygon_Renderer();
        MeshClass *mesh = task->Peek_Mesh();
        captainslog_assert(mesh != nullptr);

        if (mesh->Get_Base_Vertex_Offset() == 0xFFFF) {
            task2 = task;
            task = task->Get_Next_Visible();
            b = true;
        } else {
            if (mesh->Get_Lighting_Environment() != nullptr) {
                DX8Wrapper::Set_Light_Environment(mesh->Get_Lighting_Environment());
            }

            Matrix3D tm = mesh->Get_Transform();
            bool identity = mesh->Is_Transform_Identity();
            Matrix3D tm2;
            MeshModelClass *mmc = mesh->Peek_Model();

            if (mmc->Get_Flag(MeshGeometryClass::ALIGNED)) {
                CameraClass *camera = g_theDX8MeshRenderer.Peek_Camera();
                Matrix3D tm3 = camera->Get_Transform();
                Vector3 v1 = tm3.Get_Z_Vector();
                Matrix3D tm4 = mesh->Get_Transform();
                Vector3 v2 = tm4.Get_Translation();
                tm2.Obj_Look_At(v1, v1 + v2, 0);
                tm = tm2;
            } else if (mmc->Get_Flag(MeshGeometryClass::ORIENTED)) {
                Vector3 v1;
                Vector3 v2;
                CameraClass *camera = g_theDX8MeshRenderer.Peek_Camera();
                Matrix3D tm3 = camera->Get_Transform();
                tm3.Get_Translation(&v2);
                Matrix3D tm4 = mesh->Get_Transform();
                tm4.Get_Translation(&v1);
                tm2.Obj_Look_At(v1, v2, 0);
                tm = tm2;
            } else if (mmc->Get_Flag(MeshGeometryClass::SKIN)) {
                tm = Matrix3D::Identity;
                identity = true;
            }

            if (identity) {
#ifdef BUILD_WITH_D3D8
                DX8Wrapper::Set_World_Identity();
#endif
            } else {
#ifdef BUILD_WITH_D3D8
                DX8Wrapper::Set_Transform(D3DTS_WORLD, tm);
#endif
            }

            if (mesh->Get_ObjectScale() != 1.0f) {
#ifdef BUILD_WITH_D3D8
                DX8Wrapper::Set_DX8_Render_State(D3DRS_NORMALIZENORMALS, 1);
#endif
            }

            RenderObjClass::Material_Override *mat_override =
                static_cast<RenderObjClass::Material_Override *>(mesh->Get_User_Data());

            if (mmc->Get_Flag(MeshGeometryClass::SORT) && W3D::Is_Sorting_Enabled()) {
                renderer->Render_Sorted(mesh->Get_Base_Vertex_Offset(), mesh->Get_Bounding_Sphere());
            } else if (mesh->Get_Alpha_Override() != 1.0f
                || (mat_override != nullptr && mat_override->m_structID == RenderObjClass::USER_DATA_MATERIAL_OVERRIDE)) {
                float opacity = m_material->Get_Opacity();
                Vector3 diffuse;
                m_material->Get_Diffuse(&diffuse);
                TextureMapperClass *mapper = m_material->Peek_Mapper(0);
                unsigned int sync;
                Vector2 uv;

                if (mat_override != nullptr && mat_override->m_structID == RenderObjClass::USER_DATA_MATERIAL_OVERRIDE
                    && mapper != nullptr && mapper->Mapper_ID() == TextureMapperClass::MAPPER_ID_LINEAR_OFFSET) {
                    LinearOffsetTextureMapperClass *l = static_cast<LinearOffsetTextureMapperClass *>(mapper);
                    sync = l->Get_Sync_Time();
                    l->Set_Sync_Time(W3D::Get_Sync_Time());
                    uv = l->Get_Current_UV_Offset();
                    l->Set_Current_UV_Offset(mat_override->m_customUVOffset);
                } else {
                    mapper = nullptr;
                }

                if (mesh->Get_Alpha_Override() == 1.0f) {
                    renderer->Render(mesh->Get_Base_Vertex_Offset());
                } else {
                    if (mesh->Is_Additive()) {
                        m_material->Set_Diffuse(
                            mesh->Get_Alpha_Override(), mesh->Get_Alpha_Override(), mesh->Get_Alpha_Override());
                        shader2 = shader;
                    }

                    m_material->Set_Opacity(mesh->Get_Alpha_Override());
                    DX8Wrapper::Set_Shader(shader2);
                    DX8Wrapper::Apply_Render_State_Changes();
#ifdef BUILD_WITH_D3D8
                    DX8Wrapper::Set_DX8_Render_State(D3DRS_ALPHAREF, mesh->Get_Alpha_Override() * 96);
#endif
                    renderer->Render(mesh->Get_Base_Vertex_Offset());
#ifdef BUILD_WITH_D3D8
                    DX8Wrapper::Set_DX8_Render_State(D3DRS_ALPHAREF, 96);
#endif
                    m_material->Set_Opacity(opacity);
                    m_material->Set_Diffuse(diffuse.X, diffuse.Y, diffuse.Z);
                    DX8Wrapper::Set_Shader(shader);
                }

                if (mapper != nullptr) {
                    LinearOffsetTextureMapperClass *l = static_cast<LinearOffsetTextureMapperClass *>(mapper);
                    l->Set_Sync_Time(sync);
                    l->Set_Current_UV_Offset(uv);
                }

                DX8Wrapper::Set_Material(nullptr);
                DX8Wrapper::Set_Material(m_material);
            } else {
                renderer->Render(mesh->Get_Base_Vertex_Offset());
            }

            if (mesh->Get_ObjectScale() != 1.0f) {
#ifdef BUILD_WITH_D3D8
                DX8Wrapper::Set_DX8_Render_State(D3DRS_NORMALIZENORMALS, 0);
#endif
            }

            auto task3 = task->Get_Next_Visible();

            if (task2 != nullptr) {
                task2->Set_Next_Visible(task3);
            } else {
                m_renderTaskHead = task3;
            }

            if (task != nullptr) {
                delete task;
            }

            task = task3;
        }
    }

    if (!b) {
        captainslog_assert(m_renderTaskHead == nullptr);
        Clear_Render_List();
    }
}

DX8FVFCategoryContainer::DX8FVFCategoryContainer(unsigned int FVF, bool sorting) : FVFCategoryContainer(FVF, sorting)
{
    if ((FVF & D3DFVF_TEX1) == D3DFVF_TEX1) {
        m_uvCoordinateChannels = 1;
    }
    if ((FVF & D3DFVF_TEX2) == D3DFVF_TEX2) {
        m_uvCoordinateChannels = 2;
    }
    if ((FVF & D3DFVF_TEX3) == D3DFVF_TEX3) {
        m_uvCoordinateChannels = 3;
    }
    if ((FVF & D3DFVF_TEX4) == D3DFVF_TEX4) {
        m_uvCoordinateChannels = 4;
    }
    if ((FVF & D3DFVF_TEX5) == D3DFVF_TEX5) {
        m_uvCoordinateChannels = 5;
    }
    if ((FVF & D3DFVF_TEX6) == D3DFVF_TEX6) {
        m_uvCoordinateChannels = 6;
    }
    if ((FVF & D3DFVF_TEX7) == D3DFVF_TEX7) {
        m_uvCoordinateChannels = 7;
    }
    if ((FVF & D3DFVF_TEX8) == D3DFVF_TEX8) {
        m_uvCoordinateChannels = 8;
    }
}

DX8FVFCategoryContainer::~DX8FVFCategoryContainer() {}

unsigned int DX8FVFCategoryContainer::Define_FVF(MeshModelClass *mmc, bool enable_lighting)
{
    if (mmc->Get_Flag(MeshGeometryClass::SORT) && W3D::Is_Sorting_Enabled()) {
        return DX8_FVF_XYZNDUV2;
    }

    int fvf = D3DFVF_XYZ;

    if (mmc->Get_Color_Array(0, false) != nullptr) {
        fvf |= D3DFVF_DIFFUSE;
    }

    if (mmc->Get_Color_Array(1, false) != nullptr) {
        fvf |= D3DFVF_SPECULAR;
    }

    switch (mmc->Get_UV_Array_Count()) {
        case 1:
            fvf |= D3DFVF_TEX1;
            break;
        case 2:
            fvf |= D3DFVF_TEX2;
            break;
        case 3:
            fvf |= D3DFVF_TEX3;
            break;
        case 4:
            fvf |= D3DFVF_TEX4;
            break;
        case 5:
            fvf |= D3DFVF_TEX5;
            break;
        case 6:
            fvf |= D3DFVF_TEX6;
            break;
        case 7:
            fvf |= D3DFVF_TEX7;
            break;
        case 8:
            fvf |= D3DFVF_TEX8;
            break;
    }

    if (mmc->Needs_Vertex_Normals()) {
        fvf |= D3DFVF_NORMAL;
    }

    return fvf;
}

class PolyRemover : public MultiListObjectClass
{
public:
    DX8TextureCategoryClass *src;
    DX8TextureCategoryClass *dest;
    DX8PolygonRendererClass *pr;
};

void DX8FVFCategoryContainer::Generate_Texture_Categories(Vertex_Split_Table &split_table, unsigned int vertex_offset)
{
    int polygon_count = split_table.Get_Polygon_Count();
    int index_count = split_table.Get_Pass_Count() * 3 * polygon_count;

    if (m_indexBuffer == nullptr) {
        if (index_count < 12000) {
            index_count = 12000;
        }

        if (m_sorting) {
            m_indexBuffer = new SortingIndexBufferClass(index_count);
        } else {
            m_indexBuffer = new DX8IndexBufferClass(index_count, DX8IndexBufferClass::USAGE_DEFAULT);
        }
    }

    for (unsigned int i = 0; i < split_table.Get_Pass_Count(); i++) {
        Textures_Material_And_Shader_Booking_Struct booking;
        int old_used_indices = m_usedIndices;

        for (int j = 0; j < polygon_count; j++) {
            TextureClass *texs[2];
            texs[0] = split_table.Peek_Texture(j, i, 0);
            texs[1] = split_table.Peek_Texture(j, i, 1);
            VertexMaterialClass *vmat = split_table.Peek_Material(j, i);
            ShaderClass shader = split_table.Peek_Shader(j, i);

            if (booking.Add_Textures_Material_And_Shader(texs, vmat, shader)) {
                Insert_To_Texture_Category(split_table, texs, vmat, shader, i, vertex_offset);
            }
        }

        captainslog_assert(m_usedIndices - old_used_indices <= polygon_count * 3);
    }
}

DX8RigidFVFCategoryContainer::DX8RigidFVFCategoryContainer(unsigned int FVF, bool sorting) :
    DX8FVFCategoryContainer(FVF, sorting),
    m_vertexBuffer(nullptr),
    m_usedVertices(0),
    m_delayedVisibleMatpassHead(nullptr),
    m_delayedVisibleMatpassTail(nullptr)
{
}

DX8RigidFVFCategoryContainer::~DX8RigidFVFCategoryContainer()
{
    Ref_Ptr_Release(m_vertexBuffer);
}

void DX8RigidFVFCategoryContainer::Add_Mesh(MeshModelClass *mmc)
{
    captainslog_assert(Check_If_Mesh_Fits(mmc));
    Vertex_Split_Table split(mmc);
    int count = split.Get_Vertex_Count();
    if (m_vertexBuffer == nullptr) {
        unsigned short size = 4000;

        if (count > 4000) {
            size = count;
        }

        if (m_sorting) {
            m_vertexBuffer = new SortingVertexBufferClass(size);
            captainslog_assert(m_vertexBuffer->FVF_Info().Get_FVF() == m_FVF);
        } else {
            m_vertexBuffer = new DX8VertexBufferClass(m_FVF, size, DX8VertexBufferClass::USAGE_DEFAULT, 0);
        }
    }

    VertexBufferClass::AppendLockClass lock(m_vertexBuffer, m_usedVertices, split.Get_Vertex_Count());
    FVFInfoClass f = m_vertexBuffer->FVF_Info();
    char *outverts = static_cast<char *>(lock.Get_Vertex_Array());
    const Vector3 *verts = split.Get_Vertex_Array();
    const Vector3 *normals = split.Get_Vertex_Normal_Array();
    const unsigned int *diffuse = split.Get_Color_Array(0);
    const unsigned int *specular = split.Get_Color_Array(1);

#ifdef BUILD_WITH_D3D8
    for (unsigned int i = 0; i < split.Get_Vertex_Count(); i++) {

        *reinterpret_cast<Vector3 *>(&outverts[f.Get_Location_Offset()]) = verts[i];

        if (m_FVF & D3DFVF_NORMAL) {
            if (normals != nullptr) {
                *reinterpret_cast<Vector3 *>(&outverts[f.Get_Normal_Offset()]) = normals[i];
            }
        }

        if (m_FVF & D3DFVF_DIFFUSE) {
            if (diffuse != nullptr) {
                *reinterpret_cast<unsigned int *>(&outverts[f.Get_Diffuse_Offset()]) = diffuse[i];
            } else {
                *reinterpret_cast<unsigned int *>(&outverts[f.Get_Diffuse_Offset()]) = 0xFFFFFFFF;
            }
        }

        if (m_FVF & D3DFVF_SPECULAR) {
            if (specular != nullptr) {
                *reinterpret_cast<unsigned int *>(&outverts[f.Get_Specular_Offset()]) = specular[i];
            } else {
                *reinterpret_cast<unsigned int *>(&outverts[f.Get_Specular_Offset()]) = 0xFFFFFFFF;
            }
        }

        outverts += f.Get_FVF_Size();
    }

    int texcount = (m_FVF & D3DFVF_TEXCOUNT_MASK) >> D3DFVF_TEXCOUNT_SHIFT;

    for (int i = 0; i < texcount; i++) {
        char *outverts2 = static_cast<char *>(lock.Get_Vertex_Array());
        const Vector2 *uv = split.Get_UV_Array(i);

        if (uv != nullptr) {
            for (unsigned int j = 0; j < split.Get_Vertex_Count(); j++) {
                *reinterpret_cast<Vector2 *>(&outverts2[f.Get_Tex_Offset(i)]) = uv[j];
                outverts2 += f.Get_FVF_Size();
            }
        }
    }
#endif

    Generate_Texture_Categories(split, m_usedVertices);
    m_usedVertices += count;
}

void DX8RigidFVFCategoryContainer::Log(bool only_visible)
{
    // does nothing
}

bool DX8RigidFVFCategoryContainer::Check_If_Mesh_Fits(MeshModelClass *mmc)
{
    if (m_vertexBuffer == nullptr) {
        return true;
    }

    int count = mmc->Get_Polygon_Count();

    return mmc->Get_Vertex_Count() <= m_vertexBuffer->Get_Vertex_Count() - m_usedVertices
        && 3 * count * mmc->Get_Pass_Count() <= m_indexBuffer->Get_Index_Count() - m_usedIndices;
}

void DX8RigidFVFCategoryContainer::Render()
{
    if (Anything_To_Render()) {
        m_anythingToRender = false;
        DX8Wrapper::Set_Vertex_Buffer(m_vertexBuffer, 0);
        DX8Wrapper::Set_Index_Buffer(m_indexBuffer, 0);

        for (unsigned int i = 0; i < m_passes; i++) {
            for (DX8TextureCategoryClass *t = m_visibleTextureCategoryList[i].Remove_Head(); t != nullptr;
                 t = m_visibleTextureCategoryList[i].Remove_Head()) {
                t->Render();
            }
        }

        Render_Procedural_Material_Passes();
    }
}

void DX8RigidFVFCategoryContainer::Add_Delayed_Visible_Material_Pass(MaterialPassClass *pass, MeshClass *mesh)
{
    MatPassTaskClass *task = new MatPassTaskClass(pass, mesh);

    if (m_delayedVisibleMatpassHead != nullptr) {
        captainslog_assert(m_delayedVisibleMatpassTail != nullptr);
        m_delayedVisibleMatpassTail->Set_Next_Visible(task);
    } else {
        captainslog_assert(m_delayedVisibleMatpassTail == nullptr);
        m_delayedVisibleMatpassHead = task;
    }

    m_delayedVisibleMatpassTail = task;
    m_anyDelayedPassesToRender = true;
}

void DX8RigidFVFCategoryContainer::Render_Delayed_Procedural_Material_Passes()
{
    if (Any_Delayed_Passes_To_Render()) {
        m_anyDelayedPassesToRender = false;
        DX8Wrapper::Set_Vertex_Buffer(m_vertexBuffer, 0);
        DX8Wrapper::Set_Index_Buffer(m_indexBuffer, 0);

        MatPassTaskClass *m;

        for (MatPassTaskClass *i = m_delayedVisibleMatpassHead; i != nullptr; i = m) {
            i->Peek_Mesh()->Render_Material_Pass(i->Peek_Material_Pass(), m_indexBuffer);
            m = i->Get_Next_Visible();
            delete i;
        }

        m_delayedVisibleMatpassTail = nullptr;
        m_delayedVisibleMatpassHead = nullptr;
    }
}

DX8SkinFVFCategoryContainer::DX8SkinFVFCategoryContainer(bool sorting) :
    DX8FVFCategoryContainer(DX8_FVF_XYZNUV1, sorting),
    m_visibleVertexCount(0),
    m_visibleSkinHead(nullptr),
    m_visibleSkinTail(nullptr)
{
}

void DX8SkinFVFCategoryContainer::Add_Visible_Skin(MeshClass *mesh)
{
    if (mesh->Peek_Next_Visible_Skin() == nullptr && mesh != m_visibleSkinTail) {

        if (m_visibleSkinHead == nullptr) {
            m_visibleSkinTail = mesh;
        }

        mesh->Set_Next_Visible_Skin(m_visibleSkinHead);
        m_visibleSkinHead = mesh;
        m_visibleVertexCount += mesh->Peek_Model()->Get_Vertex_Count();
    }
}

DX8SkinFVFCategoryContainer::~DX8SkinFVFCategoryContainer()
{
    // does nothing
}

void DX8SkinFVFCategoryContainer::Add_Mesh(MeshModelClass *mmc)
{
    Vertex_Split_Table split(mmc);
    Generate_Texture_Categories(split, 0);
}

void DX8SkinFVFCategoryContainer::Log(bool only_visible)
{
    // does nothing
}

bool DX8SkinFVFCategoryContainer::Check_If_Mesh_Fits(MeshModelClass *mmc)
{
    if (m_indexBuffer == nullptr) {
        return true;
    }

    int count = mmc->Get_Polygon_Count();

    return 3 * count * mmc->Get_Pass_Count() <= m_indexBuffer->Get_Index_Count() - m_usedIndices;
}

void DX8SkinFVFCategoryContainer::Render()
{
    if (Anything_To_Render()) {
        m_anythingToRender = false;
        DX8Wrapper::Set_Vertex_Buffer(nullptr, 0);
        unsigned int vertcount = m_visibleVertexCount;

        if (vertcount > 65535) {
            vertcount = 65535;
        }

        DynamicVBAccessClass vb(m_sorting + 2, DX8_FVF_XYZNDUV2, vertcount);
        MeshClass *mesh1 = m_visibleSkinHead;
        unsigned int renderedvertexcount = 0;

        if (m_visibleVertexCount != 0) {
            for (;;) {
                MeshClass *mesh2 = mesh1;
                unsigned int vertex_offset = 0;
                MeshClass *mesh4 = nullptr;
                MeshClass *mesh3 = nullptr;

                { // added to control the lifetime of the DynamicVBAccessClass::WriteLockClass object
                    DynamicVBAccessClass::WriteLockClass lock(&vb);
                    VertexFormatXYZNDUV2 *vertexes = lock.Get_Formatted_Vertex_Array();

                    if (mesh1 != nullptr) {
                        for (;;) {
                            MeshModelClass *mmc = mesh1->Peek_Model();
                            int mesh_vertex_count = mmc->Get_Vertex_Count();
                            unsigned int offset = mesh_vertex_count + vertex_offset;

                            if (mesh_vertex_count + vertex_offset > vertcount || mesh3) {
                                mesh1->Set_Base_Vertex_Offset(0xFFFF);

                                if (mesh3 == nullptr) {
                                    mesh4 = mesh1;
                                }

                                mesh2 = mesh1->Peek_Next_Visible_Skin();
                            } else {
                                captainslog_assert((vertex_offset + mesh_vertex_count) <= m_visibleVertexCount);
                                // Debug_Statistics::Record_DX8_Skin_Polys_And_Vertices(mesh1->Get_Num_Polys(),
                                // mesh_vertex_count);

                                if (g_tempVertexBuffer.Length() < mesh_vertex_count) {
                                    g_tempVertexBuffer.Resize(mesh_vertex_count);
                                }

                                if (g_tempNormalBuffer.Length() < mesh_vertex_count) {
                                    g_tempNormalBuffer.Resize(mesh_vertex_count);
                                }

                                Vector3 *verts = &g_tempVertexBuffer[0];
                                Vector3 *normals = &g_tempNormalBuffer[0];
                                const Vector2 *uv1 = mmc->Get_UV_Array_By_Index(0);
                                const Vector2 *uv2 = mmc->Get_UV_Array_By_Index(1);
                                unsigned int *colors = mmc->Get_Color_Array(0, false);
                                mesh2->Get_Deformed_Vertices(verts, normals);
                                int vertcount3 = mesh_vertex_count;

                                if (mesh_vertex_count > 0) {
                                    do {
                                        vertexes->x = verts->X;
                                        vertexes->y = verts->Y;
                                        vertexes->z = verts->Z;
                                        vertexes->nx = normals->X;
                                        vertexes->ny = normals->Y;
                                        vertexes->nz = normals->Z;

                                        if (colors != nullptr) {
                                            vertexes->diffuse = *colors;
                                            colors++;
                                        } else {
                                            vertexes->diffuse = 0;
                                        }

                                        if (uv1 != nullptr) {
                                            vertexes->u1 = uv1->X;
                                            vertexes->v1 = uv1->Y;
                                            uv1++;
                                        } else {
                                            vertexes->u1 = 0;
                                            vertexes->v1 = 0;
                                        }

                                        if (uv2 != nullptr) {
                                            vertexes->u2 = uv2->X;
                                            vertexes->v2 = uv2->Y;
                                            uv2++;
                                        } else {
                                            vertexes->u2 = 0;
                                            vertexes->v2 = 0;
                                        }

                                        verts++;
                                        normals++;
                                        vertexes++;
                                        mesh_vertex_count--;
                                    } while (mesh_vertex_count != 0);
                                }

                                mesh2->Set_Base_Vertex_Offset(vertex_offset);
                                vertex_offset = offset;
                                renderedvertexcount += vertcount3;
                                mesh2 = mesh2->Peek_Next_Visible_Skin();
                            }

                            if (mesh2 == nullptr) {
                                break;
                            }

                            mesh1 = mesh2;
                            mesh3 = mesh4;
                        }
                    }
                }

                DX8Wrapper::Set_Vertex_Buffer(vb);
                DX8Wrapper::Set_Index_Buffer(m_indexBuffer, 0);

                for (unsigned int i = 0; i < m_passes; i++) {
                    MultiListIterator<DX8TextureCategoryClass> tex_it(&m_visibleTextureCategoryList[i]);

                    while (!tex_it.Is_Done()) {
                        tex_it.Peek_Obj()->Render();
                        tex_it.Next();
                    }
                }

                Render_Procedural_Material_Passes();

                if (renderedvertexcount >= m_visibleVertexCount) {
                    break;
                }

                mesh1 = mesh4;
            }
        }

        for (unsigned int i = 0; i < m_passes; i++) {
            while (m_visibleTextureCategoryList[i].Remove_Head() != nullptr) {
            }
        }

        captainslog_assert(renderedvertexcount == m_visibleVertexCount);
        Clear_Visible_Skin_List();
    }
}

void DX8SkinFVFCategoryContainer::Add_Delayed_Visible_Material_Pass(MaterialPassClass *pass, MeshClass *mesh)
{
    Add_Visible_Material_Pass(pass, mesh);
}

void DX8SkinFVFCategoryContainer::Render_Delayed_Procedural_Material_Passes()
{
    // does nothing
}

void DX8SkinFVFCategoryContainer::Reset()
{
    Clear_Visible_Skin_List();

    for (unsigned int i = 0; i < m_passes; i++) {
        for (;;) {
            TextureCategoryClass *t = m_textureCategoryList[i].Peek_Head();

            if (t == nullptr) {
                break;
            }

            delete t;
        }
    }

    Ref_Ptr_Release(m_indexBuffer);
    m_usedIndices = 0;
}

void DX8SkinFVFCategoryContainer::Clear_Visible_Skin_List()
{
    for (MeshClass *m = m_visibleSkinHead; m != nullptr; m_visibleSkinHead = m) {
        m = m_visibleSkinHead->Peek_Next_Visible_Skin();
        m_visibleSkinHead->Set_Next_Visible_Skin(nullptr);
    }

    m_visibleSkinHead = nullptr;
    m_visibleSkinTail = nullptr;
    m_visibleVertexCount = 0;
}
