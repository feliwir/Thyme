#pragma once
#include <cstddef>
#include <vector>

namespace X3D
{
enum X3DLayoutType
{
    X3D_LT_INVALID = 0,
    X3D_LT_VEC2,
    X3D_LT_VEC3,
    X3D_LT_VEC4,
};

enum X3DLayoutUsage
{
    X3D_LU_CUSTOM = 0,
    X3D_LU_POSITION,
    X3D_LU_NORMAL,
    X3D_LU_TEX1,
};

struct X3DLayoutDescription
{
    unsigned int buffer_idx = 0;
    unsigned int offset = 0;
    X3DLayoutType type = X3D_LT_INVALID;
    X3DLayoutUsage usage = X3D_LU_CUSTOM;
};

constexpr X3DLayoutDescription LayoutEnd{ 0, 0, X3D_LT_INVALID, X3D_LU_CUSTOM };

class X3DVertexLayout
{
public:
    X3DVertexLayout() : m_descr(nullptr) {}

    virtual void Bind() = 0;
    virtual int Build(X3DLayoutDescription *descr) = 0;

protected:
    static size_t Get_Vertex_Size(X3DLayoutDescription *descr)
    {
        size_t size = 0;
        while ((*descr).type != X3D_LT_INVALID) {
            switch (descr->type) {
                case X3D_LT_VEC2:
                    size += 2 * sizeof(float);
                    break;
                case X3D_LT_VEC3:
                    size += 3 * sizeof(float);
                    break;
                case X3D_LT_VEC4:
                    size += 4 * sizeof(float);
                    break;
                default:
                    break;
            }
            descr++;
        }
        return size;
    }

    static size_t Get_Component_Count(X3DLayoutDescription *descr)
    {
        switch (descr->type) {
            case X3D_LT_VEC2:
                return 2;
            case X3D_LT_VEC3:
                return 3;
            case X3D_LT_VEC4:
                return 4;
            default:
                return 1;
        }
    }

    X3DLayoutDescription *m_descr = nullptr;
};

} // namespace X3D