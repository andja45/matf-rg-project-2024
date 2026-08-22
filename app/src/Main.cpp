#include <Application.hpp>
#include <engine/core/Engine.hpp>

int main(int argc, char **argv) {
    auto app = std::make_unique<app::Application>();
    return app->run(argc, argv);
}
