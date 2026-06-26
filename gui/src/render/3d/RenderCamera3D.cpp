#include "render/3d/RenderCamera3D.hpp"

#include <Magnum/Math/Angle.h>
#include <Magnum/Math/Matrix4.h>

#include <algorithm>

namespace zappy::render3d {
namespace {

constexpr float PlanetRadiusScale = 0.45f;
constexpr float PlanetRadiusPadding = 2.0f;
constexpr float CameraVerticalOffset = 0.75f;
constexpr float CameraDistance = 7.0f;
constexpr float FieldOfView = 40.0f;
constexpr float NearPlane = 0.1f;
constexpr float FarPlaneMultiplier = 12.0f;

float computePlanetRadius(int mapWidth, int mapHeight)
{
    const float width = static_cast<float>(mapWidth > 0 ? mapWidth : 1);
    const float height = static_cast<float>(mapHeight > 0 ? mapHeight : 1);
    const float mapSize = std::max(width, height);

    return mapSize * PlanetRadiusScale + PlanetRadiusPadding;
}

float computeAspectRatio(const Magnum::Vector2i &framebufferSize)
{
    if (framebufferSize.y() == 0)
        return 1.0f;

    return static_cast<float>(framebufferSize.x()) /
        static_cast<float>(framebufferSize.y());
}

}

Magnum::Matrix4 RenderCamera3D::projection(
    int mapWidth,
    int mapHeight,
    const Magnum::Vector2i &framebufferSize
) const
{
    const float radius = computePlanetRadius(mapWidth, mapHeight);
    const float aspect = computeAspectRatio(framebufferSize);

    const Magnum::Vector3 center{0.0f, 0.0f, 0.0f};

    const Magnum::Vector3 eye{
        0.0f,
        radius * CameraVerticalOffset,
        radius * CameraDistance
    };

    const Magnum::Matrix4 projection =
        Magnum::Matrix4::perspectiveProjection(
            Magnum::Deg{FieldOfView},
            aspect,
            NearPlane,
            radius * FarPlaneMultiplier
        );

    const Magnum::Matrix4 camera =
        Magnum::Matrix4::lookAt(
            eye,
            center,
            Magnum::Vector3::yAxis()
        );

    return projection * camera.inverted();
}

}