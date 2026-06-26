#pragma once

#include "render/3d/Model3D.hpp"

#include <Magnum/Math/Color.h>
#include <Magnum/Math/Matrix4.h>
#include <Magnum/Shaders/FlatGL.h>

namespace zappy::render3d {

class ModelRenderer3D {
public:
    explicit ModelRenderer3D(Magnum::Shaders::FlatGL3D &shader);

    void draw(
        const Model3D &model,
        const Magnum::Matrix4 &projection,
        const Magnum::Matrix4 &transform,
        const Magnum::Color4 &color
    );

private:
    Magnum::Shaders::FlatGL3D &_shader;
};

}