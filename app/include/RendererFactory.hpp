#ifndef RENDERERFACTORY_HPP
#define RENDERERFACTORY_HPP
#include <IRenderer.hpp>
#include <memory>

namespace app {
class RendererFactory {
public:
    static std::unique_ptr<IRenderer> create(RendererType type);
};
}// namespace app
#endif//RENDERERFACTORY_HPP
