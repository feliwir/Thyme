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

template<typename R> class PolyRenderTaskClass : public AutoPoolClass<PolyRenderTaskClass<R>, 256>
{
public:
    PolyRenderTaskClass(R *renderer, MeshClass *mesh) : m_renderer(renderer), m_mesh(mesh), m_nextVisible(nullptr)
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

class FVFCategoryContainer;
class TextureCategoryClass : public MultiListObjectClass
{
public:
    TextureCategoryClass(
        FVFCategoryContainer *container, TextureClass **texs, ShaderClass shd, VertexMaterialClass *mat, int pass);
    ~TextureCategoryClass() override;

    virtual void Render() = 0;
    virtual PolygonRendererClass *Create_Polygon_Renderer(unsigned int index_count,
        MeshModelClass *mmc,
        unsigned int vertex_offset,
        unsigned int index_offset,
        bool strip,
        unsigned int pass) = 0;

    void Add_Render_Task(PolygonRendererClass *p_renderer, MeshClass *p_mesh);
    bool Anything_To_Render() { return m_renderTaskHead != nullptr; }
    void Clear_Render_List() { m_renderTaskHead = nullptr; }

    TextureClass *Peek_Texture(int stage) { return m_textures[stage]; }
    VertexMaterialClass *Peek_Material() { return m_material; }

    ShaderClass Get_Shader() { return m_shader; }
    FVFCategoryContainer *Get_Container() { return m_container; }
    MultiListClass<PolygonRendererClass> &Get_Polygon_Renderer_List() { return m_polygonRendererList; }

    unsigned int Add_Mesh(Vertex_Split_Table &split_table,
        unsigned int vertex_offset,
        unsigned int index_offset,
        IndexBufferClass *index_buffer,
        unsigned int pass);

    void Log(bool only_visible);

    void Remove_Polygon_Renderer(PolygonRendererClass *p_renderer);
    void Add_Polygon_Renderer(PolygonRendererClass *p_renderer, PolygonRendererClass *add_after_this);

    static void Set_Force_Multiply(bool multiply) { s_forceMultiply = multiply; }

protected:
    int m_pass;
    TextureClass *m_textures[2] = {};
    ShaderClass m_shader;
    VertexMaterialClass *m_material;
    MultiListClass<PolygonRendererClass> m_polygonRendererList;
    FVFCategoryContainer *m_container;
    PolyRenderTaskClass<PolygonRendererClass> *m_renderTaskHead;
#ifdef GAME_DLL
    static bool &s_forceMultiply;
#else
    static bool s_forceMultiply;
#endif
};

class FVFCategoryContainer : public MultiListObjectClass
{
public:
    enum
    {
        MAX_PASSES = 4
    };

    FVFCategoryContainer(unsigned int FVF, bool sorting);
    ~FVFCategoryContainer() override;

    virtual TextureCategoryClass *Create_Texture_Category(
        TextureClass **texs, ShaderClass shd, VertexMaterialClass *mat, int pass) = 0;

    virtual void Render() = 0;

    virtual void Add_Mesh(MeshModelClass *mmc) = 0;

    virtual void Log(bool only_visible) = 0;

    virtual bool Check_If_Mesh_Fits(MeshModelClass *mmc) = 0;

    virtual void Add_Delayed_Visible_Material_Pass(MaterialPassClass *pass, MeshClass *mesh) = 0;
    virtual void Render_Delayed_Procedural_Material_Passes() = 0;

    bool Is_Sorting() { return m_sorting; }

    void Change_Polygon_Renderer_Texture(MultiListClass<PolygonRendererClass> &polygon_renderer_list,
        TextureClass *texture,
        TextureClass *new_texture,
        unsigned int pass,
        unsigned int stage);
    void Change_Polygon_Renderer_Material(MultiListClass<PolygonRendererClass> &polygon_renderer_list,
        VertexMaterialClass *vmat,
        VertexMaterialClass *new_vmat,
        unsigned int pass);

    void Remove_Texture_Category(TextureCategoryClass *tex_category);

    unsigned int Get_FVF() { return m_FVF; }

    void Add_Visible_Texture_Category(TextureCategoryClass *tex_category, int pass)
    {
        captainslog_assert(pass >= 0 && pass < MAX_PASSES);
        captainslog_assert(tex_category != nullptr);
        captainslog_assert(m_textureCategoryList[pass].Contains(tex_category));
        m_visibleTextureCategoryList[pass].Add(tex_category);
        m_anythingToRender = true;
    }

    void Add_Visible_Material_Pass(MaterialPassClass *pass, MeshClass *mesh);

protected:
    bool Anything_To_Render() { return m_anythingToRender; }
    bool Any_Delayed_Passes_To_Render() { return m_anyDelayedPassesToRender; }

    void Render_Procedural_Material_Passes();

    TextureCategoryClass *Find_Matching_Texture_Category(
        VertexMaterialClass *vmat, unsigned int pass, TextureCategoryClass *ref_category);
    TextureCategoryClass *Find_Matching_Texture_Category(
        TextureClass *texture, unsigned int pass, unsigned int stage, TextureCategoryClass *ref_category);

    void Insert_To_Texture_Category(Vertex_Split_Table &split_table,
        TextureClass **texs,
        VertexMaterialClass *mat,
        ShaderClass shader,
        int pass,
        unsigned int vertex_offset);

    virtual void Generate_Texture_Categories(Vertex_Split_Table &split_table, unsigned int vertex_offset) = 0;

    MultiListClass<TextureCategoryClass> m_textureCategoryList[MAX_PASSES];
    MultiListClass<TextureCategoryClass> m_visibleTextureCategoryList[MAX_PASSES];
    MatPassTaskClass *m_visibleMatpassHead;
    MatPassTaskClass *m_visibleMatpassTail;
    IndexBufferClass *m_indexBuffer;
    int m_usedIndices;
    unsigned int m_FVF;
    unsigned int m_passes;
    unsigned int m_uvCoordinateChannels;
    bool m_sorting;
    bool m_anythingToRender;
    bool m_anyDelayedPassesToRender;
};

#ifdef GAME_DLL
extern DynamicVectorClass<Vector3> &g_tempVertexBuffer;
extern DynamicVectorClass<Vector3> &g_tempNormalBuffer;
extern MultiListClass<MeshModelClass> &g_registeredMeshList;
extern MultiListClass<TextureCategoryClass> &g_textureCategoryDeleteList;
extern MultiListClass<FVFCategoryContainer> &g_fvfCategoryContainerDeleteList;
#else
extern DynamicVectorClass<Vector3> g_tempVertexBuffer;
extern DynamicVectorClass<Vector3> g_tempNormalBuffer;
extern MultiListClass<MeshModelClass> g_registeredMeshList;
extern MultiListClass<TextureCategoryClass> g_textureCategoryDeleteList;
extern MultiListClass<FVFCategoryContainer> g_fvfCategoryContainerDeleteList;
#endif
