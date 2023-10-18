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
#include "indexbuffer.h"
#include "polygonrenderer.h"
#include <captainslog.h>

#ifndef GAME_DLL
DynamicVectorClass<Vector3> g_tempVertexBuffer;
DynamicVectorClass<Vector3> g_tempNormalBuffer;
MultiListClass<MeshModelClass> g_registeredMeshList;
MultiListClass<TextureCategoryClass> g_textureCategoryDeleteList;
MultiListClass<FVFCategoryContainer> g_fvfCategoryContainerDeleteList;
bool TextureCategoryClass::s_forceMultiply;
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

TextureCategoryClass::TextureCategoryClass(
    FVFCategoryContainer *container, TextureClass **texs, ShaderClass shd, VertexMaterialClass *mat, int pass) :
    m_pass(pass), m_shader(shd), m_material(mat), m_container(container), m_renderTaskHead(0)
{
    captainslog_assert(pass >= 0);
    captainslog_assert(pass < FVFCategoryContainer::MAX_PASSES);

    for (int i = 0; i < 2; i++) {
        Ref_Ptr_Set(m_textures[i], texs[i]);
    }

    if (m_material != nullptr) {
        m_material->Add_Ref();
    }
}

TextureCategoryClass::~TextureCategoryClass()
{
    for (;;) {
        PolygonRendererClass *r = m_polygonRendererList.Get_Head();

        if (r == nullptr) {
            break;
        }

        // TODO:
        // g_theDX8MeshRenderer.Unregister_Mesh_Type(r->Get_Mesh_Model_Class());
    }

    for (int i = 0; i < 2; i++) {
        Ref_Ptr_Release(m_textures[i]);
    }

    Ref_Ptr_Release(m_material);
}

void TextureCategoryClass::Add_Render_Task(PolygonRendererClass *p_renderer, MeshClass *p_mesh)
{
    auto task = new PolyRenderTaskClass(p_renderer, p_mesh);
    task->Set_Next_Visible(m_renderTaskHead);
    m_renderTaskHead = task;
    m_container->Add_Visible_Texture_Category(this, m_pass);
}

unsigned int TextureCategoryClass::Add_Mesh(Vertex_Split_Table &split_table,
    unsigned int vertex_offset,
    unsigned int index_offset,
    IndexBufferClass *index_buffer,
    unsigned int pass)
{
    int polycount = split_table.Get_Polygon_Count();
    unsigned int index_count = 0;
    int polygons = 0;

    for (int i = 0; i < polycount; i++) {
        if (split_table.Peek_Texture(i, pass, 0) == m_textures[0]) {
            if (split_table.Peek_Texture(i, pass, 1) == m_textures[1]) {
                if (Compare_Materials(split_table.Peek_Material(i, pass), m_material)) {
                    if (split_table.Peek_Shader(i, pass) == m_shader) {
                        polygons++;
                    }
                }
            }
        }
    }

    if (polygons != 0) {
        index_count = 3 * polygons;
        TriIndex *polys = split_table.Get_Polygon_Array(pass);
        PolygonRendererClass *renderer =
            Create_Polygon_Renderer(index_count, split_table.Get_Mesh_Model_Class(), vertex_offset, index_offset, 0, pass);
        m_polygonRendererList.Add_Tail(renderer);
        IndexBufferClass::AppendLockClass lock(index_buffer, index_offset, index_count);
        unsigned short *indices = lock.Get_Index_Array();
        unsigned short vmax = 0;
        unsigned short vmin = 0xFFFF;

        for (int i = 0; i < polycount; i++) {
            if (split_table.Peek_Texture(i, pass, 0) == m_textures[0]) {
                if (split_table.Peek_Texture(i, pass, 1) == m_textures[1]) {
                    if (Compare_Materials(split_table.Peek_Material(i, pass), m_material)) {
                        if (split_table.Peek_Shader(i, pass) == m_shader) {
                            unsigned short index = vertex_offset + polys[i].I;

                            vmin = std::min(index, vmin);
                            vmax = std::max(index, vmax);

                            *indices = index;
                            indices++;

                            index = vertex_offset + polys[i].J;

                            vmin = std::min(index, vmin);
                            vmax = std::max(index, vmax);

                            *indices = index;
                            indices++;

                            index = vertex_offset + polys[i].K;

                            vmin = std::min(index, vmin);
                            vmax = std::max(index, vmax);

                            *indices = index;
                            indices++;
                        }
                    }
                }
            }
        }

        captainslog_assert((vmax - vmin) < split_table.Get_Mesh_Model_Class()->Get_Vertex_Count());
        renderer->Set_Vertex_Index_Range(vmin, vmax - vmin + 1);
        captainslog_assert(index_count <= unsigned(split_table.Get_Polygon_Count() * 3));
    }

    return index_count;
}

void TextureCategoryClass::Log(bool only_visible)
{
    // does nothing
}

void TextureCategoryClass::Remove_Polygon_Renderer(PolygonRendererClass *p_renderer)
{
    m_polygonRendererList.Remove(p_renderer);
    p_renderer->Set_Texture_Category(nullptr);

    if (m_polygonRendererList.Get_Head() == nullptr) {
        m_container->Remove_Texture_Category(this);
        g_textureCategoryDeleteList.Add_Tail(this);
    }
}

void TextureCategoryClass::Add_Polygon_Renderer(PolygonRendererClass *p_renderer, PolygonRendererClass *add_after_this)
{
    captainslog_assert(p_renderer != nullptr);
    captainslog_assert(!m_polygonRendererList.Contains(p_renderer));
    if (add_after_this != nullptr) {
        bool res = m_polygonRendererList.Add_After(p_renderer, add_after_this, false);
        captainslog_assert(res);
    } else {
        m_polygonRendererList.Add(p_renderer);
    }

    p_renderer->Set_Texture_Category(this);
}

FVFCategoryContainer::FVFCategoryContainer(unsigned int FVF, bool sorting) :
    m_visibleMatpassHead(nullptr),
    m_visibleMatpassTail(nullptr),
    m_indexBuffer(nullptr),
    m_usedIndices(0),
    m_FVF(FVF),
    m_passes(MAX_PASSES),
    m_uvCoordinateChannels(0),
    m_sorting(sorting),
    m_anythingToRender(false),
    m_anyDelayedPassesToRender(false)
{
}

FVFCategoryContainer::~FVFCategoryContainer()
{
    Ref_Ptr_Release(m_indexBuffer);
    for (unsigned int i = 0; i < m_passes; i++) {
        for (TextureCategoryClass *j = m_textureCategoryList->Remove_Head(); j != nullptr;
             j = m_textureCategoryList->Remove_Head()) {
            delete j;
        }
    }
}

class PolyRemover : public MultiListObjectClass
{
public:
    TextureCategoryClass *src;
    TextureCategoryClass *dest;
    PolygonRendererClass *pr;
};

void FVFCategoryContainer::Change_Polygon_Renderer_Texture(MultiListClass<PolygonRendererClass> &polygon_renderer_list,
    TextureClass *texture,
    TextureClass *new_texture,
    unsigned int pass,
    unsigned int stage)
{
    captainslog_assert(pass < m_passes);
    MultiListClass<PolyRemover> prl;
    bool foundtexture = false;

    if (texture == new_texture) {
        return;
    } else {
        MultiListIterator<TextureCategoryClass> src_it(&m_textureCategoryList[pass]);

        while (!src_it.Is_Done()) {
            TextureCategoryClass *texcat = src_it.Peek_Obj();

            if (texcat->Peek_Texture(stage) == texture) {
                foundtexture = true;
                MultiListIterator<PolygonRendererClass> poly_it(&polygon_renderer_list);

                while (!poly_it.Is_Done()) {
                    PolygonRendererClass *pr = poly_it.Peek_Obj();

                    if (pr->Get_Texture_Category() == texcat) {
                        TextureCategoryClass *tc = Find_Matching_Texture_Category(new_texture, pass, stage, texcat);

                        if (tc == nullptr) {
                            TextureClass *tmp_textures[2];
                            tmp_textures[0] = texcat->Peek_Texture(0);
                            tmp_textures[1] = texcat->Peek_Texture(1);
                            tmp_textures[stage] = new_texture;
                            tc = Create_Texture_Category(tmp_textures, texcat->Get_Shader(), texcat->Peek_Material(), pass);
                            bool b = false;
                            MultiListIterator<TextureCategoryClass> tc_it(&m_textureCategoryList[pass]);

                            while (!tc_it.Is_Done()) {
                                if (tmp_textures[0] == tc_it.Peek_Obj()->Peek_Texture(0)) {
                                    b = true;
                                    m_textureCategoryList[pass].Add_After(tc, tc_it.Peek_Obj());
                                    break;
                                }

                                tc_it.Next();
                            }

                            if (!b) {
                                m_textureCategoryList[pass].Add_Tail(tc);
                            }
                        }

                        PolyRemover *remover = new PolyRemover();
                        remover->src = texcat;
                        remover->dest = tc;
                        remover->pr = pr;
                        prl.Add(remover);
                    }

                    poly_it.Next();
                }
            } else if (foundtexture) {
                break;
            }

            src_it.Next();
        }

        MultiListIterator<PolyRemover> prl_it(&prl);

        while (!prl_it.Is_Done()) {
            PolyRemover *remover = prl_it.Peek_Obj();
            remover->src->Remove_Polygon_Renderer(remover->pr);
            remover->dest->Add_Polygon_Renderer(remover->pr, nullptr);
            prl_it.Remove_Current_Object();
            delete remover;
        }
    }
}

void FVFCategoryContainer::Change_Polygon_Renderer_Material(MultiListClass<PolygonRendererClass> &polygon_renderer_list,
    VertexMaterialClass *vmat,
    VertexMaterialClass *new_vmat,
    unsigned int pass)
{
    captainslog_assert(pass < m_passes);
    MultiListClass<PolyRemover> prl;
    bool foundmat = false;

    if (vmat == new_vmat) {
        return;
    } else {
        MultiListIterator<TextureCategoryClass> src_it(&m_textureCategoryList[pass]);

        while (!src_it.Is_Done()) {
            TextureCategoryClass *texcat = src_it.Peek_Obj();

            if (texcat->Peek_Material() == vmat) {
                foundmat = true;
                MultiListIterator<PolygonRendererClass> poly_it(&polygon_renderer_list);

                while (!poly_it.Is_Done()) {
                    PolygonRendererClass *pr = poly_it.Peek_Obj();

                    if (pr->Get_Texture_Category() == texcat) {
                        TextureCategoryClass *tc = Find_Matching_Texture_Category(new_vmat, pass, texcat);

                        if (tc == nullptr) {
                            TextureClass *tmp_textures[2];
                            tmp_textures[0] = texcat->Peek_Texture(0);
                            tmp_textures[1] = texcat->Peek_Texture(1);
                            tc = Create_Texture_Category(tmp_textures, texcat->Get_Shader(), new_vmat, pass);
                            bool b = false;
                            MultiListIterator<TextureCategoryClass> tc_it(&m_textureCategoryList[pass]);

                            while (!tc_it.Is_Done()) {
                                if (tmp_textures[0] == tc_it.Peek_Obj()->Peek_Texture(0)) {
                                    b = true;
                                    m_textureCategoryList[pass].Add_After(tc, tc_it.Peek_Obj());
                                    break;
                                }

                                tc_it.Next();
                            }

                            if (!b) {
                                m_textureCategoryList[pass].Add_Tail(tc);
                            }
                        }

                        PolyRemover *remover = new PolyRemover();
                        remover->src = texcat;
                        remover->dest = tc;
                        remover->pr = pr;
                        prl.Add(remover);
                    }

                    poly_it.Next();
                }
            } else if (foundmat) {
                break;
            }

            src_it.Next();
        }

        MultiListIterator<PolyRemover> prl_it(&prl);

        while (!prl_it.Is_Done()) {
            PolyRemover *remover = prl_it.Peek_Obj();
            remover->src->Remove_Polygon_Renderer(remover->pr);
            remover->dest->Add_Polygon_Renderer(remover->pr, nullptr);
            prl_it.Remove_Current_Object();
            delete remover;
        }
    }
}

void FVFCategoryContainer::Remove_Texture_Category(TextureCategoryClass *tex_category)
{
    for (unsigned int i = 0; i < m_passes; i++) {
        m_textureCategoryList[i].Remove(tex_category);
    }

    for (unsigned int i = 0; i < m_passes; i++) {
        if (m_textureCategoryList[i].Peek_Head() != nullptr) {
            return;
        }
    }

    g_fvfCategoryContainerDeleteList.Add_Tail(this);
}

void FVFCategoryContainer::Add_Visible_Material_Pass(MaterialPassClass *pass, MeshClass *mesh)
{
    MatPassTaskClass *task = new MatPassTaskClass(pass, mesh);

    if (m_visibleMatpassHead != nullptr) {
        captainslog_assert(m_visibleMatpassTail != nullptr);
        m_visibleMatpassTail->Set_Next_Visible(task);
    } else {
        captainslog_assert(m_visibleMatpassTail == nullptr);
        m_visibleMatpassHead = task;
    }

    m_visibleMatpassTail = task;
    m_anythingToRender = true;
}

void FVFCategoryContainer::Render_Procedural_Material_Passes()
{
    MatPassTaskClass *task = m_visibleMatpassHead;
    MatPassTaskClass *task2 = nullptr;
    bool b = false;

    while (task != nullptr) {
        if (task->Peek_Mesh()->Get_Base_Vertex_Offset() == 0xFFFF) {
            task2 = task;
            task = task->Get_Next_Visible();
            b = true;
        } else {
            task->Peek_Mesh()->Render_Material_Pass(task->Peek_Material_Pass(), m_indexBuffer);
            MatPassTaskClass *task3 = task->Get_Next_Visible();

            if (task2 != nullptr) {
                task2->Set_Next_Visible(task3);
            } else {
                m_visibleMatpassHead = task3;
            }

            delete task;
            task = task3;
        }
    }

    m_visibleMatpassTail = b ? task2 : nullptr;
}

TextureCategoryClass *FVFCategoryContainer::Find_Matching_Texture_Category(
    VertexMaterialClass *vmat, unsigned int pass, TextureCategoryClass *ref_category)
{
    MultiListIterator<TextureCategoryClass> dest_it(&m_textureCategoryList[pass]);

    while (!dest_it.Is_Done()) {
        if (Compare_Materials(dest_it.Peek_Obj()->Peek_Material(), vmat)) {
            TextureCategoryClass *tc = dest_it.Peek_Obj();
            bool b = true;

            for (int i = 0; i < 2; i++) {
                b = b & (tc->Peek_Texture(i) == ref_category->Peek_Texture(i));
            }

            if (b) {
                if (tc->Get_Shader() == ref_category->Get_Shader()) {
                    return tc;
                }
            }
        }

        dest_it.Next();
    }

    return nullptr;
}

TextureCategoryClass *FVFCategoryContainer::Find_Matching_Texture_Category(
    TextureClass *texture, unsigned int pass, unsigned int stage, TextureCategoryClass *ref_category)
{
    MultiListIterator<TextureCategoryClass> dest_it(&m_textureCategoryList[pass]);

    while (!dest_it.Is_Done()) {
        if (dest_it.Peek_Obj()->Peek_Texture(stage) == texture) {
            TextureCategoryClass *tc = dest_it.Peek_Obj();
            bool b = true;

            for (int i = 0; i < 2; i++) {
                if (stage != i) {
                    b = b & (tc->Peek_Texture(i) == ref_category->Peek_Texture(i));
                }
            }

            if (b) {
                if (Compare_Materials(tc->Peek_Material(), ref_category->Peek_Material())) {
                    if (tc->Get_Shader() == ref_category->Get_Shader()) {
                        return tc;
                    }
                }
            }
        }

        dest_it.Next();
    }

    return nullptr;
}

void FVFCategoryContainer::Insert_To_Texture_Category(Vertex_Split_Table &split_table,
    TextureClass **texs,
    VertexMaterialClass *mat,
    ShaderClass shader,
    int pass,
    unsigned int vertex_offset)
{
    bool b = false;
    MultiListIterator<TextureCategoryClass> dst_it(&m_textureCategoryList[pass]);

    while (!dst_it.Is_Done()) {
        TextureCategoryClass *tc = dst_it.Peek_Obj();

        if (tc->Peek_Texture(0) == texs[0]) {
            if (tc->Peek_Texture(1) == texs[1]) {
                if (Compare_Materials(tc->Peek_Material(), mat)) {
                    if (tc->Get_Shader() == shader) {
                        m_usedIndices += tc->Add_Mesh(split_table, vertex_offset, m_usedIndices, m_indexBuffer, pass);
                        b = true;
                        break;
                    }
                }
            }
        }

        dst_it.Next();
    }

    if (!b) {
        TextureCategoryClass *tc = Create_Texture_Category(texs, shader, mat, pass);
        m_usedIndices += tc->Add_Mesh(split_table, vertex_offset, m_usedIndices, m_indexBuffer, pass);

        bool b2 = false;
        MultiListIterator<TextureCategoryClass> tc_it(&m_textureCategoryList[pass]);

        while (!tc_it.Is_Done()) {
            if (texs[0] == tc_it.Peek_Obj()->Peek_Texture(0)) {
                b2 = true;
                m_textureCategoryList[pass].Add_After(tc, tc_it.Peek_Obj());
                break;
            }

            tc_it.Next();
        }

        if (!b2) {
            m_textureCategoryList[pass].Add_Tail(tc);
        }
    }
}