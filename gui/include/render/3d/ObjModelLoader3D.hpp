#pragma once

#include "render/3d/Model3D.hpp"

#include <optional>
#include <string>

namespace zappy::render3d {

class ObjModelLoader3D {
public:
    std::optional<Model3D> load(const std::string &path) const;

private:
    static bool parseVertexLine(const std::string &line, Magnum::Vector3 &vertex);
    static bool parseFaceLine(
        const std::string &line,
        const std::vector<Magnum::Vector3> &positions,
        std::vector<Vertex3D> &vertices
    );

    static std::optional<int> parseFaceIndex(const std::string &token, int vertexCount);
};

}