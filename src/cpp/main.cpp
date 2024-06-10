#include "../include/Global.hpp"
#include "../include/Engine.hpp"

using namespace std;

const char* const& Title    = "Test";

Engine::Core *engine = Engine::Core::Inst(Title, 800, 600);

int main(int argc, char* argv[])
{
    Log::Inst()->log("Starting engine...");
    // Main Player
    engine->createObject({{
        {Engine::SCREEN_WIDTH / 2, Engine::SCREEN_HEIGHT / 2},
        10,
        10,
        5,
        Engine::ObjectState::IS_PLAYER
    }});

    engine->createObject({{
        {Engine::SCREEN_WIDTH / 2, Engine::SCREEN_HEIGHT / 2},
        50,
        50,
        5,
        Engine::ObjectState::STATIC
    }});

    return engine->run();
}