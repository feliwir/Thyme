#include "x3d_vertexlayout_gl.h"
#include "x3d.h"

int X3D::X3DVertexLayoutGL::Build(X3DLayoutDescription *descr)
{
    Bind();
    m_descr = descr;
    // Setup our layout from the description
    size_t stride = Get_Vertex_Size(descr);
    int idx = 0;
    while ((*descr).type != X3D_LT_INVALID) {
        glVertexAttribPointer(idx, Get_Component_Count(descr), GL_FLOAT, GL_FALSE, stride, (GLvoid *)(intptr_t)descr->offset);
        glEnableVertexAttribArray(idx++);
        descr++;
    }

    return X3D_ERR_OK;
}