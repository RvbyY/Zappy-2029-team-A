#include "render/MapRenderer.hpp"

#include "render/RenderGeometry.hpp"
#include "render/RenderMeshFactory.hpp"

#include <Magnum/Math/Color.h>

namespace zappy::render {

MapRenderer::MapRenderer(Magnum::Shaders::FlatGL2D &shader)
    : _shader(shader),
      _gridMesh(std::nullopt),
      _cachedWidth(0),
      _cachedHeight(0)
{
}

void MapRenderer::draw(const GameState &state, const Magnum::Matrix3 &projection)
{
    const int width = state.width();
    const int height = state.height();

    if (width <= 0 || height <= 0)
        return;

    if (shouldRebuildGrid(width, height))
        rebuildGridMesh(width, height);

    if (!_gridMesh.has_value())
        return;

    _shader
        .setColor(Magnum::Color4{0.75f, 0.75f, 0.85f, 1.0f})
        .setTransformationProjectionMatrix(projection)
        .draw(*_gridMesh);
}

bool MapRenderer::shouldRebuildGrid(int width, int height) const
{
    return !_gridMesh.has_value() ||
           _cachedWidth != width ||
           _cachedHeight != height;
}

void MapRenderer::rebuildGridMesh(int width, int height)
{
    const std::vector<Vertex> vertices = buildGridVertices(width, height);

    _gridMesh = RenderMeshFactory::build(
        Magnum::GL::MeshPrimitive::Lines,
        vertices
    );

    _cachedWidth = width;
    _cachedHeight = height;
}

}