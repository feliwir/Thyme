#pragma once
#include "w3dformat.h"
#include "x3d.h"
#include <vector3.h>

struct X3DMaterial
{
    Vector3 diffuse;
    Vector3 ambient;
    Vector3 specular;
    float opacity;
};

X3D::X3DTextureFormat WW3DFormat_To_X3DFormat(WW3DFormat format);
