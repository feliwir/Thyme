#pragma once
#include "x3d_vertexlayout.h"

#include <flextGL.h>

namespace X3D
{
class X3DVertexLayoutGL : public X3DVertexLayout
{
public:
    X3DVertexLayoutGL() { glGenVertexArrays(1, &m_vao); }

    void Bind() override { glBindVertexArray(m_vao); }
    int Build(X3DLayoutDescription *descr) override;

    ~X3DVertexLayoutGL() { glDeleteVertexArrays(1, &m_vao); }

protected:
    GLuint m_vao;
};
} // namespace X3D