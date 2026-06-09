#include "render/ExpulsionRenderer.hpp"

#include "render/RenderGeometry.hpp"
#include "render/RenderMeshFactory.hpp"

#include <Magnum/GL/Mesh.h>
#include <Magnum/Math/Color.h>

#include <vector>

namespace {

void appendExpulsionCross(
    std::vector<zappy::render::Vertex> &vertices,
    int x,
    int y
)
{
    constexpr float Radius = 0.44f;

    const float centerX = static_cast<float>(x) + 0.5f;
    const float centerY = static_cast<float>(y) + 0.5f;

    vertices.push_back({{centerX - Radius, centerY}});
    vertices.push_back({{centerX + Radius, centerY}});

    vertices.push_back({{centerX, centerY - Radius}});
    vertices.push_back({{centerX, centerY + Radius}});

    vertices.push_back({{centerX - Radius, centerY - Radius}});
    vertices.push_back({{centerX + Radius, centerY + Radius}});

    vertices.push_back({{centerX - Radius, centerY + Radius}});
    vertices.push_back({{centerX + Radius, centerY - Radius}});
}

std::vector<zappy::render::Vertex> buildExpulsionVertices(const GameState &state)
{
    std::vector<zappy::render::Vertex> vertices;

    for (const Expulsion &expulsion : state.expulsions())
        appendExpulsionCross(vertices, expulsion.x(), expulsion.y());

    return vertices;
}

}

namespace zappy::render {

ExpulsionRenderer::ExpulsionRenderer(Magnum::Shaders::FlatGL2D &shader)
    : _shader(shader)
{
}

void ExpulsionRenderer::draw(const GameState &state, const Magnum::Matrix3 &projection)
{
    if (state.width() <= 0 || state.height() <= 0)
        return;

    const std::vector<Vertex> vertices = buildExpulsionVertices(state);

    if (vertices.empty())
        return;

    Magnum::GL::Mesh mesh =
        RenderMeshFactory::build(Magnum::GL::MeshPrimitive::Lines, vertices);

    _shader
        .setColor(Magnum::Color4{1.0f, 0.20f, 0.10f, 1.0f})
        .setTransformationProjectionMatrix(projection)
        .draw(mesh);
}

}