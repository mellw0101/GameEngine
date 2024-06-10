#include "../include/Engine.hpp"
//#include <algorithm>

using namespace std;

#pragma region 'Tools'
    Engine::Time* Engine::Time::instance = nullptr;
    mutex Engine::Time::mutex;
    
//    void setBit     ( u32 &num, u32 bit ) // Set the bit at position `bit` to 1
//    {
//        num |= (1u << bit);
//    }
//    void clearBit   ( u32 &num, u32 bit ) // Clear the bit at position `bit` to 0
//    {
//        num &= ~(1u << bit);
//    }
#pragma endregion
#pragma region Object2D
    auto Engine::   Object::    init        ( const ObjectData2D& data )                ->  void
    {
        this->data = data;
    }
    auto Engine::   Object::    move        ( Vec2D vel )                               ->  void
    {
        vel.x = (data.position.x + data.w + vel.x >= SCREEN_WIDTH ) ? SCREEN_WIDTH  - data.position.x - data.w : vel.x;
        vel.y = (data.position.y + data.h + vel.y >= SCREEN_HEIGHT) ? SCREEN_HEIGHT - data.position.y - data.h : vel.y;
        
        vel.x = (data.position.x + vel.x <= 0) ? -data.position.x : vel.x;
        vel.y = (data.position.y + vel.y <= 0) ? -data.position.y : vel.y;

        data.position += vel;
    }
    auto Engine::   Object::    draw        ( SDL_Renderer* renderer )            const ->  void
    {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect currentRect = rect();
        SDL_RenderFillRect(renderer, &currentRect);
    }
    auto Engine::   Object::    isStatic    ()                                    const ->  bool
    {
        return (data.state & (ObjectState::STATIC));
    }
    auto Engine::   Object::    rect        ()                                    const ->  SDL_Rect
    {
        SDL_Rect rect = { data.position.getXInt(), data.position.getYInt(), data.w, data.h };
        return rect;
    }
    auto Engine::   Object::    frect       ()                                    const ->  SDL_FRect
    {
        return { data.position.x, data.position.y, static_cast<f32>(data.w), static_cast<f32>(data.h) };
    }
    auto Engine::   Object::    state       ()                                    const ->  u32
    {
        return data.state;
    }
#pragma endregion
#pragma region 'Core'
    auto Engine::Core::  run             ()                        ->  int
    {
        init();
        while (running)
        {
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
    auto Engine::Core::  createObject    ( const Object& object )  ->  void
    {
        objects.emplace_back(object);
    }
    auto Engine::Core::  init            ()                        ->  int
    {
        initSDL();
        createWindow();
        createRenderer();
        setupKeys();
        return 0;
    }
    #pragma region 'Sub Functions' run
        auto Engine::Core::  cleanup         ()  ->  void
        {
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
        }
        auto Engine::Core::  logic           ()  ->  void
        {
            KeyObject::Instance()->handleKeyEvent();
        }
        auto Engine::Core::  applyPhysics    ()  ->  void
        {
            for (auto& object : objects)
            {
                // Only apply gravity if the object is not static
                if (object.state() & ObjectState::STATIC)
                {
                    continue;
                }

                // Apply Gravity
                object.move({0.0f, GRAVITY});

                // Check if the object is on the ground
                if (object.data.position.y + (f32)object.data.h == (f32)SCREEN_HEIGHT)
                {
                    object.data.state |= ObjectState::ON_GROUND;
                }
                else
                {
                    object.data.state &= ~ObjectState::ON_GROUND;
                }

                for (auto& other : objects)
                {
                    if (&object == &other)
                    {
                        continue;
                    }
                }
            }
        }     
        auto Engine::Core::  clear           ()  ->  void
        {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
        }
        auto Engine::Core::  draw            ()  ->  void
        {
            for (auto const& object : objects)
            {
                object.draw(renderer);
            }
        }
        auto Engine::Core::  update          ()  ->  void
        {
            SDL_RenderPresent(renderer);        // Update screen
            SDL_Delay(8);                  // Approximately 60 frames per second
        }
        auto Engine::Core::  pollForEvents   ()  ->  void
        {
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT)
                {
                    running = false;
                }
            }
        }
    #pragma endregion
    #pragma region 'Sub Functions' init
        auto Engine::Core::  initSDL         ()  ->  int
        {
            if (SDL_Init(SDL_INIT_VIDEO) < 0)
            {
                running = false;
                return -1;
            }
            return 0;
        }
        auto Engine::Core::  createWindow    ()  ->  int
        {
            window = SDL_CreateWindow
            (
                window_title.c_str(),
                SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED,
                SCREEN_WIDTH,
                SCREEN_HEIGHT,
                SDL_WINDOW_SHOWN
            );
            if (!window)
            {
                SDL_Quit();
                return -1;
            }
            return 0;
        }
        auto Engine::Core::  createRenderer  ()  ->  int
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
        auto Engine::Core::  setupKeys       ()  ->  void
        {
            KeyObject::Instance()->addActionForKey(SDL_SCANCODE_W,      [&]()  ->  void
            {
                for (auto& object : objects)
                {
                    ((object.state() & ObjectState::STATIC) == false) ? object.move({0.0, -object.data.speed}) : void();
                }
            });
            KeyObject::Instance()->addActionForKey(SDL_SCANCODE_S,      [&]()  ->  void
            {
                for (auto& object : objects)
                {
                    ((object.state() & ObjectState::STATIC) == false) ? object.move({0.0, object.data.speed}) : void();
                }
            });
            KeyObject::Instance()->addActionForKey(SDL_SCANCODE_A,      [&]()  ->  void
            {
                for (auto& object : objects)
                {
                    ((object.state() & ObjectState::STATIC) == false) ? object.move({-object.data.speed, 0.0}) : void();
                }
            });
            KeyObject::Instance()->addActionForKey(SDL_SCANCODE_D,      [&]()  ->  void
            {
                for (auto& object : objects)
                {
                    ! (object.state() & ObjectState::STATIC) ? object.move({object.data.speed, 0.0}) : void();
                }
            });
            KeyObject::Instance()->addActionForKey(SDL_SCANCODE_ESCAPE, [&]()  ->  void
            {
                running = false;
            });
            KeyObject::Instance()->addActionForKey(SDL_SCANCODE_SPACE,  [&]()  ->  void
            {
                for (auto& object : objects)
                {
                    if (object.state() & ObjectState::STATIC)       // If the object is static, skip
                    {
                        continue;
                    }
                    if (object.state() & ObjectState::IS_PLAYER)    // If the object is a player, jump
                    {
                        // if (object.state() & ObjectState::ON_GROUND) // If the player is on the ground, jump
                        // {
                        // }
                        object.move({0, -10});
                    }
                }
            });
        }
    #pragma endregion
    
    Engine::Core::          Core            ( const string& window_title, int window_width, int window_height )
    : window_title(window_title), SCREEN_WIDTH(window_width), SCREEN_HEIGHT(window_height)
    {
        Engine::SCREEN_WIDTH    = SCREEN_WIDTH;
        Engine::SCREEN_HEIGHT   = SCREEN_HEIGHT;
    }
#pragma endregion