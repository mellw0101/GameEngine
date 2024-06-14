#include "../include/Engine.hpp"
#include <cstdint>
#include "../include/Global.hpp"

using namespace std;

#pragma region Object2D
auto
Object2D::init(const ObjectData2D& data) -> void
{
    this->data = data;
}

auto
Object2D::move(Vector2D vel) -> void
{
    vel.x = (data.position.x + data.w + vel.x >= SCREEN_WIDTH) ? SCREEN_WIDTH - data.position.x - data.w : vel.x;
    vel.y = (data.position.y + data.h + vel.y >= SCREEN_HEIGHT) ? SCREEN_HEIGHT - data.position.y - data.h : vel.y;

    vel.x = (data.position.x + vel.x <= 0) ? -data.position.x : vel.x;
    vel.y = (data.position.y + vel.y <= 0) ? -data.position.y : vel.y;

    data.position += vel;
}

auto
Object2D::draw(SDL_Renderer* renderer) const -> void
{
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect currentRect = rect();
    SDL_RenderFillRect(renderer, &currentRect);
}

auto
Object2D::isStatic() const -> bool
{
    return (data.state & (State::STATIC));
}

auto
Object2D::rect() const -> SDL_Rect
{
    SDL_Rect rect = {data.position.getXInt(), data.position.getYInt(), data.w, data.h};
    return rect;
}

auto
Object2D::frect() const -> SDL_FRect
{
    return {data.position.x, data.position.y, static_cast<f32>(data.w), static_cast<f32>(data.h)};
}

auto
Object2D::state() const -> u32
{
    return data.state;
}

#pragma endregion
#pragma region 'Core'
auto
Engine::Core::createObject(const Object2D& object) -> void
{
    objects.emplace_back(object);
}

int
Engine::Core::init()
{
    initSDL();
    createWindow();
    createRenderer();
    setupKeys();
    return 0;
}

int
Engine::Core::initSDL()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        running = false;
        return -1;
    }
    return 0;
}

auto
Engine::Core::createWindow() -> int
{
    window = SDL_CreateWindow(window_title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                              SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window)
    {
        SDL_Quit();
        return -1;
    }
    return 0;
}

auto
Engine::Core::createRenderer() -> int
{
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }
    return 0;
}

auto
Engine::Core::setupKeys() -> void
{
    KeyObject::Instance()->addActionForKey(
        SDL_SCANCODE_W,
        [&]() -> void
        {
            for (auto& object : objects)
            {
                ((object.state() & State::STATIC) == false) ? object.move({0.0, -object.data.speed}) : void();
            }
        });

    KeyObject::Instance()->addActionForKey(
        SDL_SCANCODE_S,
        [&]() -> void
        {
            for (auto& object : objects)
            {
                ((object.state() & State::STATIC) == false) ? object.move({0.0, object.data.speed}) : void();
            }
        });
    KeyObject::Instance()->addActionForKey(
        SDL_SCANCODE_A,
        [&]() -> void
        {
            for (auto& object : objects)
            {
                ((object.state() & State::STATIC) == false) ? object.move({-object.data.speed, 0.0}) : void();
            }
        });
    KeyObject::Instance()->addActionForKey(
        SDL_SCANCODE_D,
        [&]() -> void
        {
            for (auto& object : objects)
            {
                !(object.state() & State::STATIC) ? object.move({object.data.speed, 0.0}) : void();
            }
        });
    KeyObject::Instance()->addActionForKey(SDL_SCANCODE_ESCAPE, [&]() -> void { running = false; });
    KeyObject::Instance()->addActionForKey(SDL_SCANCODE_SPACE,
                                           [&]() -> void
                                           {
                                               for (auto& object : objects)
                                               {
                                                   // If the object is static, skip
                                                   if (object.state() & State::STATIC)
                                                   {
                                                       continue;
                                                   }

                                                   // If the object is a player, jump
                                                   if (object.state() & State::IS_PLAYER)
                                                   {
                                                       object.move({0, -10});
                                                   }
                                               }
                                           });
}

auto
Engine::Core::run() -> int
{
    init();
    while (running)
    {
        if (DEBUG)
        {
            cout << "FRAMECOUNT: " << frames << '\n';
        }
        frames++;
        pollForEvents();
        applyPhysics();
        logic();
        clear();
        draw();
        update();
    }
    cleanup();
    return 0;
}

auto
Engine::Core::cleanup() -> void
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

auto
Engine::Core::logic() -> void
{
    KeyObject::Instance()->handleKeyEvent();
}

auto
Engine::Core::applyPhysics() -> void
{
    for (auto& object : objects)
    {
        // Only apply gravity if the object is not static
        if (object.state() & State::STATIC)
        {
            continue;
        }
        else
        {
            // Apply Gravity
            object.move({0.0f, GRAVITY});

            // Check if the object is on the ground
            if (object.data.position.y + (f32)object.data.h == (f32)SCREEN_HEIGHT)
            {
                object.data.state |= State::ON_GROUND;
            }
            else
            {
                object.data.state &= ~State::ON_GROUND;
            }

            for (auto& other : objects)
            {
                if (&object == &other)
                {
                    continue;
                }
                // if (object.data.position.x < other.data.position.x + other.data.w &&
                //     object.data.position.x + object.data.w > other.data.position.x &&
                //     object.data.position.y < other.data.position.y + other.data.h &&
                //     object.data.position.y + object.data.h > other.data.position.y)
                // {
                //     cout << "Collision" << '\n';
                // }
                if (obj2DPointInObj2DRect(object, other))
                {
                    cout << "Collision" << '\n';
                }
            }
        }
    }
}

auto
Engine::Core::clear() -> void
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

auto
Engine::Core::draw() -> void
{
    for (auto const& object : objects)
    {
        object.draw(renderer);
    }
}

auto
Engine::Core::update() -> void
{
    SDL_RenderPresent(renderer); // Update screen
    SDL_Delay(8);                // Approximately 60 frames per second
}

auto
Engine::Core::pollForEvents() -> void
{
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            running = false;
        }
    }
}

Engine::Core::Core(const string& window_title, const int& window_width, const int& window_height)
    : window_title(window_title)
{
    SCREEN_WIDTH  = window_width;
    SCREEN_HEIGHT = window_height;
}
