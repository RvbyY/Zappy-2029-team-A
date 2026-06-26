#include "render/3d/MapRenderer3D.hpp"

#include "render/3d/RenderGeometry3D.hpp"
#include "render/3d/RenderMeshFactory3D.hpp"

#include <Magnum/GL/Mesh.h>
#include <Magnum/Math/Color.h>

#include <algorithm>
#include <cmath>
#include <vector>

namespace zappy::render3d {
namespace {

constexpr float Pi = 3.14159265358979323846f;
constexpr float GridLift = 0.025f;

float planetRadius(int width, int height)
{
    const float mapSize = static_cast<float>(std::max(width, height));
    return mapSize * 0.45f + 2.0f;
}

Magnum::Vector3 spherePoint(
    float radius,
    float longitude,
    float latitude
)
{
    const float cosLatitude = std::cos(latitude);

    return Magnum::Vector3{
        radius * cosLatitude * std::cos(longitude),
        radius * std::sin(latitude),
        radius * cosLatitude * std::sin(longitude)
    };
}

Magnum::Vector3 mapPoint(
    int x,
    int y,
    int width,
    int height,
    float radius
)
{
    const float longitude =
        (static_cast<float>(x) / static_cast<float>(width)) * 2.0f * Pi;

    const float latitude =
        -Pi * 0.5f
        + (static_cast<float>(y) / static_cast<float>(height)) * Pi;

    return spherePoint(radius, longitude, latitude);
}

void addLine(
    std::vector<Vertex3D> &vertices,
    const Magnum::Vector3 &from,
    const Magnum::Vector3 &to
)
{
    vertices.push_back({from});
    vertices.push_back({to});
}

void addTriangle(
    std::vector<Vertex3D> &vertices,
    const Magnum::Vector3 &a,
    const Magnum::Vector3 &b,
    const Magnum::Vector3 &c
)
{
    vertices.push_back({a});
    vertices.push_back({b});
    vertices.push_back({c});
}

void addQuad(
    std::vector<Vertex3D> &vertices,
    const Magnum::Vector3 &a,
    const Magnum::Vector3 &b,
    const Magnum::Vector3 &c,
    const Magnum::Vector3 &d
)
{
    addTriangle(vertices, a, b, c);
    addTriangle(vertices, a, c, d);
}

std::vector<Vertex3D> buildPlanetSurfaceVertices(int width, int height)
{
    std::vector<Vertex3D> vertices;

    if (width <= 0 || height <= 0)
        return vertices;

    const float radius = planetRadius(width, height);

    vertices.reserve(static_cast<std::size_t>(width * height * 6));

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            const Magnum::Vector3 a = mapPoint(x, y, width, height, radius);
            const Magnum::Vector3 b = mapPoint(x + 1, y, width, height, radius);
            const Magnum::Vector3 c = mapPoint(x + 1, y + 1, width, height, radius);
            const Magnum::Vector3 d = mapPoint(x, y + 1, width, height, radius);

            addQuad(vertices, a, b, c, d);
        }
    }

    return vertices;
}

std::vector<Vertex3D> buildPlanetGridVertices(int width, int height)
{
    std::vector<Vertex3D> vertices;

    if (width <= 0 || height <= 0)
        return vertices;

    const float radius = planetRadius(width, height) + GridLift;

    vertices.reserve(static_cast<std::size_t>(
        (width * height * 2) + ((height + 1) * width * 2)
    ));

    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            addLine(
                vertices,
                mapPoint(x, y, width, height, radius),
                mapPoint(x, y + 1, width, height, radius)
            );
        }
    }

    for (int y = 0; y <= height; ++y) {
        for (int x = 0; x < width; ++x) {
            addLine(
                vertices,
                mapPoint(x, y, width, height, radius),
                mapPoint(x + 1, y, width, height, radius)
            );
        }
    }

    return vertices;
}

void drawMesh(
    Magnum::Shaders::FlatGL3D &shader,
    const Magnum::Matrix4 &projection,
    MeshPrimitive3D primitive,
    const std::vector<Vertex3D> &vertices,
    const Magnum::Color4 &color
)
{
    if (vertices.empty())
        return;

    Magnum::GL::Mesh mesh = RenderMeshFactory3D::build(primitive, vertices);

    shader
        .setTransformationProjectionMatrix(projection)
        .setColor(color)
        .draw(mesh);
}

}

MapRenderer3D::MapRenderer3D(Magnum::Shaders::FlatGL3D &shader)
    : _shader(shader)
{
}

void MapRenderer3D::draw(const GameState &state, const Magnum::Matrix4 &projection)
{
    const std::vector<Vertex3D> surfaceVertices = buildPlanetSurfaceVertices(
        state.width(),
        state.height()
    );

    const std::vector<Vertex3D> gridVertices = buildPlanetGridVertices(
        state.width(),
        state.height()
    );

    drawMesh(
        _shader,
        projection,
        MeshPrimitive3D::Triangles,
        surfaceVertices,
        Magnum::Color4{0.08f, 0.10f, 0.15f, 1.0f}
    );

    drawMesh(
        _shader,
        projection,
        MeshPrimitive3D::Lines,
        gridVertices,
        Magnum::Color4{0.85f, 0.90f, 1.0f, 1.0f}
    );
}

}