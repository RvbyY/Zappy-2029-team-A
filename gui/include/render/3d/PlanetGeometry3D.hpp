#pragma once

#include <Magnum/Math/Matrix4.h>
#include <Magnum/Math/Vector3.h>

namespace zappy::render3d {

class PlanetGeometry3D {
public:
    static float radius(int mapWidth, int mapHeight);

    static Magnum::Vector3 tileCenter(
        int tileX,
        int tileY,
        int mapWidth,
        int mapHeight,
        float surfaceLift = 0.0f
    );

    static Magnum::Matrix4 surfaceTransform(
        int tileX,
        int tileY,
        int mapWidth,
        int mapHeight,
        float surfaceLift,
        float scale
    );

private:
    static float tileLongitude(int tileX, int mapWidth);
    static float tileLatitude(int tileY, int mapHeight);

    static Magnum::Vector3 spherePoint(
        float radius,
        float longitude,
        float latitude
    );

    static Magnum::Matrix4 alignUpToNormal(const Magnum::Vector3 &position);
};

}