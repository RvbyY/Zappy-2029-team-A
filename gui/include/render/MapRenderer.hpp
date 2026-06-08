#pragma once

#include "state/GameState.hpp"

#include <Magnum/GL/Mesh.h>
#include <Magnum/Math/Matrix3.h>
#include <Magnum/Shaders/FlatGL.h>

#include <optional>

namespace zappy::render {

class MapRenderer {
public:
    explicit MapRenderer(Magnum::Shaders::FlatGL2D &shader);

    void draw(const GameState &state, const Magnum::Matrix3 &projection);

private:
    void rebuildGridMesh(int width, int height);
    bool shouldRebuildGrid(int width, int height) const;

    Magnum::Shaders::FlatGL2D &_shader;

    std::optional<Magnum::GL::Mesh> _gridMesh;
    int _cachedWidth;
    int _cachedHeight;
};

}