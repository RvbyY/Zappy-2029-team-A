#pragma once

#include "render/3d/RenderGeometry3D.hpp"
#include "render/3d/RenderMeshFactory3D.hpp"

#include <vector>

namespace zappy::render3d {

class Model3D {
public:
    Model3D();
    explicit Model3D(std::vector<Vertex3D> vertices);

    bool isValid() const;
    const std::vector<Vertex3D> &vertices() const;

private:
    std::vector<Vertex3D> _vertices;
};

}