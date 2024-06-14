#include <Mlib/Assert.h>
#include <Mlib/Sdl2.h>
#include <SDL2/SDL_rect.h>

using namespace std;
using namespace Mlib;

const int SCREEN_WIDTH  = 800;
const int SCREEN_HEIGHT = 600;

const char* const& Title = "Test";

Sdl2::Core*      engine = Sdl2::Core::Instance(Title, 800, 600);
Sdl2::KeyObject* key    = Sdl2::KeyObject::Instance();

int
main(int argc, char* argv[])
{
    ASSERT_STATEMENT(SCREEN_WIDTH == 800);
    key->addActionForKey(SDL_SCANCODE_ESCAPE,
                         [&]() -> void
                         {
                             engine->stop();
                         });
    key->addActionForKey(
        SDL_SCANCODE_W,
        [&]() -> void
        {
            for (auto& object : engine->getObjects())
            {
                ((object.state() & Sdl2::State::STATIC) == false) ? object.move({0.0, -object.data.speed}) : void();
            }
        });
    key->addActionForKey(
        SDL_SCANCODE_S,
        [&]() -> void
        {
            for (auto& object : engine->getObjects())
            {
                (object.state() & Sdl2::State::IS_PLAYER) ? object.move({0.0, object.data.speed}) : void();
            }
        });
    key->addActionForKey(
        SDL_SCANCODE_A,
        [&]() -> void
        {
            for (auto& object : engine->getObjects())
            {
                ((object.state() & Sdl2::State::STATIC) == false) ? object.move({-object.data.speed, 0.0}) : void();
            }
        });
    key->addActionForKey(
        SDL_SCANCODE_D,
        [&]() -> void
        {
            for (auto& object : engine->getObjects())
            {
                ((object.state() & Sdl2::State::STATIC) == false) ? object.move({object.data.speed, 0.0}) : void();
            }
        });
    key->addActionForKey(SDL_SCANCODE_SPACE,
                         [&]() -> void
                         {
                             for (auto& object : engine->getObjects())
                             {
                                 // If the object is static, skip
                                 if (object.state() & Sdl2::State::STATIC)
                                 {
                                     continue;
                                 }

                                 // If the object is a player, jump
                                 if (object.state() & Sdl2::State::IS_PLAYER)
                                 {
                                     object.move({0, -10});
                                 }
                             }
                         });

    engine->setMainLoop(
        [&]() -> void
        {
            for (auto& object : engine->getObjects())
            {
                // If the object is static, skip
                if (object.state() & Sdl2::State::STATIC)
                {
                    continue;
                }

                // If the object is a player, apply gravity
                if (object.state() & Sdl2::State::IS_PLAYER)
                {
                    for (auto& other : engine->getObjects())
                    {
                        // If the object is static, skip
                        if (other.state() & Sdl2::State::STATIC)
                        {
                            u32 const X = object.data.position.x;
                            u32 const Y = object.data.position.y;
                            u32 const W = object.data.w;
                            u32 const H = object.data.h;

                            u32 const oX = other.data.position.x;
                            u32 const oY = other.data.position.y;
                            u32 const oW = other.data.w;
                            u32 const oH = other.data.h;

                            if (X <= oX + oW && X + W >= oX && Y <= oY + oH && Y + H >= oY)
                            {
                                object.move({0, -3});
                            }
                        }
                    }
                    object.move({0, 3});
                }
            }
        });

    // Main Player
    engine->createObject({
        {{(SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2)}, 10, 10, 5, Mlib::Sdl2::State::IS_PLAYER}
    });

    // Static Object ( Floor )
    engine->createObject({
        {{SCREEN_WIDTH - 50, SCREEN_HEIGHT / 2}, 50, 2, 5, Mlib::Sdl2::State::STATIC}
    });

    return engine->run();
}
