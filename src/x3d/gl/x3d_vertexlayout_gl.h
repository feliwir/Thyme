#pragma once
#include "x3d_vertexlayout.h"

#include "flextGL.h"

namespace X3D
{
class X3DVertexLayoutGL : public X3DVertexLayout
{
public:
    X3DVertexLayoutGL(X3DLayoutDescription *descr) : X3DVertexLayout(descr) 
    { 
      glGenVertexArrays(1, &m_vao);
      glBindVertexArray(m_vao);

    }

    ~X3DVertexLayoutGL() { glDeleteVertexArrays(1, &m_vao); }

protected:
    GLuint m_vao;
};
} // namespace X3D