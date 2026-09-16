#include <DeferredRenderer.hpp>
#include <RendererFactory.hpp>
#include <engine/util/Errors.hpp>

namespace app {
std::unique_ptr<IRenderer> RendererFactory::create(RendererType type) {
    switch (type) {
        case RendererType::Deferred: return std::make_unique<DeferredRenderer>();
        case RendererType::ForwardPlus: return std::make_unique<ForwardPlusRenderer>();
        default: RG_SHOULD_NOT_REACH_HERE("Unrecognized RendererType");
    }
}
}// namespace app
