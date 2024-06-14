#include <Mlib/Assert.h>
#include <Mlib/Profile.h>
#include <Mlib/Sdl2.h>

using namespace std;
using namespace Mlib;

const int SCREEN_WIDTH  = 800;
const int SCREEN_HEIGHT = 600;

const char* const& Title = "Test";

Sdl2::Core*      engine = Sdl2::Core::Instance(Title, 800, 600);
Sdl2::KeyObject* key    = Sdl2::KeyObject::Instance();

static f32 const         FPS          = 120.0f;
static f32 const         timePerFrame = 1.0f / FPS;
static Sdl2::Vec2D const acceleration(0.0f, 9.81f);

void
calculate_gravity_effect(double time_ms)
{
    double g      = 9.81;
    double time_s = time_ms / 1000.0;

    double velocity = g * time_s;
    double distance = 0.5 * g * time_s * time_s;

    std::cout << "Velocity: " << to_string(velocity) << " m/s" << std::endl;
    std::cout << "Distance: " << to_string(distance) << " m" << std::endl;
}

int
main(int argc, char* argv[])
{
    Profile::setupReportGeneration();

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
            Profile::AutoTimer timer("key->addActionForKey(SDL_SCANCODE_W)");
            for (auto const& object : engine->getObjects())
            {
                ((object->state() & Sdl2::State::STATIC) == false) ? object->move({0.0, -object->data.speed}) : void();
            }
        });
    key->addActionForKey(
        SDL_SCANCODE_S,
        [&]() -> void
        {
            Profile::AutoTimer timer("key->addActionForKey(SDL_SCANCODE_S)");
            for (auto& object : engine->getObjects())
            {
                (object->state() & Sdl2::State::IS_PLAYER) ? object->move({0.0, object->data.speed}) : void();
            }
        });
    key->addActionForKey(
        SDL_SCANCODE_A,
        [&]() -> void
        {
            Profile::AutoTimer timer("key->addActionForKey(SDL_SCANCODE_A)");
            for (auto& object : engine->getObjects())
            {
                (object->state() & Sdl2::State::IS_PLAYER) ? object->move({-object->data.speed, 0.0}) : void();
            }
        });
    key->addActionForKey(
        SDL_SCANCODE_D,
        [&]() -> void
        {
            Profile::AutoTimer timer("key->addActionForKey(SDL_SCANCODE_D)");
            for (auto& object : engine->getObjects())
            {
                ((object->state() & Sdl2::State::STATIC) == false) ? object->move({object->data.speed, 0.0}) : void();
            }
        });
    key->addActionForKey(SDL_SCANCODE_SPACE,
                         [&]() -> void
                         {
                             Profile::AutoTimer timer("key->addActionForKey(SDL_SCANCODE_SPACE)");
                             for (auto const& object : engine->getObjects())
                             {
                                 // If the object is static, skip
                                 if (object->state() & Sdl2::State::STATIC)
                                 {
                                     continue;
                                 }

                                 // If the object is a player, jump
                                 if (object->state() & Sdl2::State::IS_PLAYER)
                                 {
                                     object->data.position += {0.0, -object->data.speed};
                                 }
                             }
                         });

    engine->setMainLoop(
        [&]() -> void
        {
            Profile::AutoTimer timer("engine->setMainLoop()");
            for (auto& object : engine->getObjects())
            {
                // If the object is static, skip
                if (object->state() & Sdl2::State::STATIC)
                {
                    continue;
                }

                // If the object is a player, apply gravity
                if (object->state() & Sdl2::State::IS_PLAYER)
                {
                    for (auto const& other : engine->getObjects())
                    {
                        if (other->state() & Sdl2::State::STATIC)
                        {
                            f64 const X = object->data.position.x;
                            f64 const Y = object->data.position.y;
                            f64 const W = object->data.w;
                            f64 const H = object->data.h;

                            f64 const oX = other->data.position.x;
                            f64 const oY = other->data.position.y;
                            f64 const oW = other->data.w;
                            f64 const oH = other->data.h;

                            if (X < oX + oW && X + W > oX && Y < oY + oH && Y + H > oY)
                            {
                                object->data.velocity   = {0.0, 0.0};
                                object->data.position.y = oY - H;
                                object->data.velocity -= Sdl2::velocityChange;
                            }
                        }
                    }
                    object->data.velocity += Sdl2::velocityChange;
                    object->move(object->data.velocity);
                }
            }
        });

    // Main Player
    engine->createObject({
        {{((f64)SCREEN_WIDTH / 2), ((f64)SCREEN_HEIGHT / 2)}, {}, 10, 10, 5, Sdl2::State::IS_PLAYER}
    });

    // Static Object ( Floor )
    engine->createObject({
        {{0, SCREEN_HEIGHT - 20}, {}, SCREEN_WIDTH, 20, 0, Sdl2::State::STATIC}
    });

    return engine->run();
}
