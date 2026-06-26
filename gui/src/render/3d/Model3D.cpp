#include "render/3d/Model3D.hpp"

#include <utility>

namespace zappy::render3d {

Model3D::Model3D()
    : _vertices()
{
}

Model3D::Model3D(std::vector<Vertex3D> vertices)
    : _vertices(std::move(vertices))
{
}

bool Model3D::isValid() const
{
    return !_vertices.empty();
}

const std::vector<Vertex3D> &Model3D::vertices() const
{
    return _vertices;
}

}