#pragma once
#include <vector>

namespace X3D
{
enum X3DLayoutType
{
    X3D_LT_INVALID = 0,
    X3D_LT_VEC3,
    X3D_LT_VEC4,
};

enum X3DLayoutUsage
{
    X3D_LU_CUSTOM = 0,
    X3D_LU_POSITION,
    X3D_LU_NORMAL,
};

struct X3DLayoutDescription
{
    int buffer_idx = 0;
    int offset = 0;
    X3DLayoutType type = X3D_LT_INVALID;
    X3DLayoutUsage usage = X3D_LU_CUSTOM;
};

constexpr X3DLayoutDescription LayoutEnd{ -1, 0, X3D_LT_INVALID, X3D_LU_CUSTOM };

class X3DVertexLayout
{
public:
    X3DVertexLayout(X3DLayoutDescription *descr) : m_descr(descr) {}

protected:
    X3DLayoutDescription *m_descr = nullptr;
};

} // namespace X3D