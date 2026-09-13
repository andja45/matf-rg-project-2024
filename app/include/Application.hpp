#ifndef APPLICATION_HPP
#define APPLICATION_HPP
#include <engine/core/App.hpp>

namespace app {
class Application final : public engine::core::App {
    void app_setup() override;
};
}// namespace app
#endif//APPLICATION_HPP
