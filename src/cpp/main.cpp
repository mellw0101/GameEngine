#include "../include/Global.hpp"
#include "../include/Engine.hpp"

using namespace std;

const char* const& Title    = "Test";

int main(int argc, char* argv[])
{
    Engine::Base engine{Title, 800, 600};
    
    // Main Player
    engine.createObject({{
        {Engine::SCREEN_WIDTH / 2, Engine::SCREEN_HEIGHT / 2},
        10,
        10,
        5,
        Engine::ObjectState::IS_PLAYER
    }});

    engine.createObject({{
        {Engine::SCREEN_WIDTH / 2, Engine::SCREEN_HEIGHT / 2},
        50,
        50,
        5,
        Engine::ObjectState::STATIC
    }});
    return engine.run();
}