#include <Mlib/Assert.h>
#include <Mlib/Math.h>
#include <Mlib/Profile.h>
#include <Mlib/Sdl2.h>

using namespace std;
using namespace Mlib;
using namespace Sdl2;
using namespace Math;

const int   WIDTH  = 800;
const int   HEIGHT = 600;
const char* Title  = "Test";

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
    Core::Instance()->setTitle(Title);

    ASSERT_STATEMENT(WIDTH == 800);
    ASSERT_STATEMENT(HEIGHT == 600);
    SCREEN_WIDTH = WIDTH;
    ASSERT_STATEMENT(SCREEN_WIDTH == WIDTH);
    SCREEN_HEIGHT = HEIGHT;
    ASSERT_STATEMENT(SCREEN_HEIGHT == HEIGHT);

    KeyObject::Instance()->addActionForKey(SDL_SCANCODE_ESCAPE,
                                           [&]() -> void
                                           {
                                               Core::Instance()->stop();
                                           });
    KeyObject::Instance()->addActionForKey(SDL_SCANCODE_W,
                                           [&]() -> void
                                           {
                                               Profile::AutoTimer timer("key->addActionForKey(SDL_SCANCODE_W)");
                                               for (auto const& obj : Core::Instance()->getObjects())
                                               {
                                                   if (obj->data.velocity.y < 0.0)
                                                   {
                                                       continue;
                                                   }
                                                   Vec2D nVec = {0.0, -obj->data.speed};
                                                   obj->state(State::IS_PLAYER) ? obj->move(nVec.normalized()) : void();
                                               }
                                           });
    KeyObject::Instance()->addActionForKey(SDL_SCANCODE_S,
                                           [&]() -> void
                                           {
                                               Profile::AutoTimer timer("key->addActionForKey(SDL_SCANCODE_S)");
                                               for (auto const& obj : Core::Instance()->getObjects())
                                               {
                                                   if (obj->state(State::IS_PLAYER))
                                                   {
                                                       if (obj->data.velocity.y > 0.0)
                                                       {
                                                           continue;
                                                       }
                                                       Vec2D nVec = {0.0, obj->data.speed};
                                                       obj->state(State::ON_GROUND) ? void()
                                                                                    : obj->move(nVec.normalized());
                                                   }
                                               }
                                           });
    KeyObject::Instance()->addActionForKey(SDL_SCANCODE_A,
                                           [&]() -> void
                                           {
                                               Profile::AutoTimer timer("key->addActionForKey(SDL_SCANCODE_A)");
                                               for (auto const& obj : Core::Instance()->getObjects())
                                               {
                                                   Vec2D nVec = {-obj->data.speed, 0.0};
                                                   (obj->state() & Sdl2::State::IS_PLAYER)
                                                       ? obj->move(nVec.normalized())
                                                       : void();
                                               }
                                           });
    KeyObject::Instance()->addActionForKey(
        SDL_SCANCODE_D,
        [&]() -> void
        {
            Profile::AutoTimer timer("key->addActionForKey(SDL_SCANCODE_D)");
            for (auto const& obj : Core::Instance()->getObjects())
            {
                obj->state(State::IS_PLAYER) ? obj->move({obj->data.speed, 0.0}) : void();
            }
        });
    KeyObject::Instance()->addActionForKey(SDL_SCANCODE_SPACE,
                                           [&]() -> void
                                           {
                                               Profile::AutoTimer timer("key->addActionForKey(SDL_SCANCODE_SPACE)");
                                               for (auto const& obj : Core::Instance()->getObjects())
                                               {
                                                   // If the object is static, skip
                                                   if (obj->state() & State::STATIC)
                                                   {
                                                       continue;
                                                   }

                                                   // If the object is a player, jump
                                                   if (obj->state() & State::IS_PLAYER)
                                                   {
                                                       if (obj->data.velocity.y == 0.0)
                                                       {
                                                           //    object->data.position += {0.0, -10.0};
                                                           obj->data.velocity.y += 6.0;
                                                       }
                                                   }
                                               }
                                           });

    Core::Instance()->setMainLoop(
        [&]() -> void
        {
            Profile::AutoTimer timer("engine->setMainLoop()");
            for (auto const& obj : Core::Instance()->getObjects())
            {
                // If the object is static, skip
                if (obj->state() & State::STATIC)
                {
                    continue;
                }

                // If the object is a player, apply gravity
                if (obj->state() & State::IS_PLAYER)
                {
                    for (auto const& other : Core::Instance()->getObjects())
                    {
                        if (other->state() & Sdl2::State::STATIC)
                        {
                            f64 const X = obj->data.position.x;
                            f64 const Y = obj->data.position.y;
                            f64 const W = obj->data.w;
                            f64 const H = obj->data.h;

                            f64 const oX = other->data.position.x;
                            f64 const oY = other->data.position.y;
                            f64 const oW = other->data.w;
                            f64 const oH = other->data.h;

                            if (X < oX + oW && X + W > oX && Y < oY + oH && Y + H >= oY)
                            {
                                cout << "velocity: " << obj->data.velocity.y << "\n";
                                cout << "X = " << X << "    Y = " << Y << "     W = " << W << "     H = " << H
                                     << "     oX = " << oX << "       oY = " << oY << "    oW = " << oW
                                     << "     oH = " << oH << "     Sdl2::velocityChange.y = " << Sdl2::velocityChange.y
                                     << '\n';

                                if (obj->data.velocity.y == 0.0)
                                {
                                    obj->data.state |= State::ON_GROUND;
                                }
                                else
                                {
                                    obj->data.state &= ~State::ON_GROUND;
                                }

                                if (obj->data.velocity.y < 0.1)
                                {
                                    obj->data.velocity = {0, 0};
                                }
                                else if (obj->data.velocity.y < 0.2)
                                {
                                    obj->data.velocity = {0.0, calculateBounceVelocity(obj->data.velocity.y, 0.50)};
                                }
                                else if (obj->data.velocity.y < 0.3)
                                {
                                    obj->data.velocity = {0.0, calculateBounceVelocity(obj->data.velocity.y, 0.60)};
                                }
                                else if (obj->data.velocity.y < 0.5)
                                {
                                    obj->data.velocity = {0.0, calculateBounceVelocity(obj->data.velocity.y, 0.70)};
                                }
                                else if (obj->data.velocity.y < 1.0)
                                {
                                    obj->data.velocity = {0.0, calculateBounceVelocity(obj->data.velocity.y, 0.75)};
                                }
                                else if (obj->data.velocity.y < 2.0)
                                {
                                    obj->data.velocity = {0.0, calculateBounceVelocity(obj->data.velocity.y, 0.80)};
                                }
                                else if (obj->data.velocity.y < 3.0)
                                {
                                    obj->data.velocity = {0.0, calculateBounceVelocity(obj->data.velocity.y, 0.83)};
                                }
                                else
                                {
                                    obj->data.velocity = {0.0, calculateBounceVelocity(obj->data.velocity.y, 0.85)};
                                }
                                obj->data.velocity -= Sdl2::velocityChange;
                                obj->data.position.y = oY - H;
                            }
                        }
                    }
                    obj->data.velocity += Sdl2::velocityChange;
                    obj->move(obj->data.velocity);
                }
            }
        });

    // Main Player
    Core::Instance()->createObject({
        {{((f64)SCREEN_WIDTH / 2), ((f64)SCREEN_HEIGHT / 2)}, {}, 10, 10, 20, Sdl2::State::IS_PLAYER}
    });

    // Static Object ( Floor )
    Core::Instance()->createObject({
        {{0, static_cast<f64>(SCREEN_HEIGHT - 20)}, {}, SCREEN_WIDTH, 20, 0, Sdl2::State::STATIC}
    });

    return Core::Instance()->run();
}
