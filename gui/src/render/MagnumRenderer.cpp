#include "render/MagnumRenderer.hpp"

#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/Math/Color.h>

namespace {

constexpr Magnum::Color4 ClearColor{0.06f, 0.06f, 0.09f, 1.0f};

}

MagnumRenderer::MagnumRenderer(const Arguments &arguments)
    : Magnum::Platform::Sdl2Application(
          arguments,
          Configuration{}
              .setTitle("Zappy GUI")
              .setSize({1280, 720})
      ),
      _state(nullptr),
      _isOpen(true),
      _shader(),
      _camera(),
      _mapRenderer(_shader),
      _resourceRenderer(_shader),
      _incantationRenderer(_shader),
      _broadcastRenderer(_shader),
      _expulsionRenderer(_shader),
      _eggRenderer(_shader),
      _playerRenderer(_shader),
      _shader3D(),
      _camera3D(),
      _mapRenderer3D(_shader3D)
{
    configureRenderer();
}

bool MagnumRenderer::isOpen() const
{
    return _isOpen;
}

void MagnumRenderer::handleEvents()
{
    mainLoopIteration();
}

void MagnumRenderer::render(const GameState &state)
{
    _state = &state;
    redraw();
}

void MagnumRenderer::configureRenderer()
{
    Magnum::GL::Renderer::setClearColor(ClearColor);
    Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::DepthTest);
    Magnum::GL::Renderer::enable(Magnum::GL::Renderer::Feature::FaceCulling);
}

void MagnumRenderer::clearFrame()
{
    Magnum::GL::defaultFramebuffer.clear(
        Magnum::GL::FramebufferClear::Color |
        Magnum::GL::FramebufferClear::Depth
    );
}

bool MagnumRenderer::canRender() const
{
    return _state != nullptr && _state->isReady();
}

void MagnumRenderer::draw3DMap()
{
    const Magnum::Matrix4 projection =
        _camera3D.projection(_state->width(), _state->height(), framebufferSize());

    _mapRenderer3D.draw(*_state, projection);
}

void MagnumRenderer::drawEvent()
{
    clearFrame();

    if (canRender())
        draw3DMap();

    swapBuffers();
}

void MagnumRenderer::viewportEvent(ViewportEvent &event)
{
    Magnum::GL::defaultFramebuffer.setViewport({{}, event.framebufferSize()});
}