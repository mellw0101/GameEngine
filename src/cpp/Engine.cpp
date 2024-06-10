#include "../include/Engine.hpp"

using namespace std;

#pragma region 'Engine::Object'
    auto Engine::   Object::   init     ( const ObjectData2D& data )
    -> void
    {
        this->data = data;
    }
    auto Engine::   Object::   move     ( s32 Direction, f32 speed_override )
    -> void
    {
        s32 speed = (speed_override == 0) ? data.speed : speed_override;

        Vec2D movement(0, 0);

        if (Direction & Direction::UP)
        {
            if (data.position.y > 0)
            {
                movement.y -= speed;
            }
        }
        if (Direction & Direction::DOWN)
        {
            if (data.position.y + data.h < SCREEN_HEIGHT)
            {
                movement.y += speed;
            }
        }
        if (Direction & Direction::LEFT)
        {
            if (data.position.x > 0)
            {
                movement.x -= speed;
            }
        }
        if (Direction & Direction::RIGHT)
        {
            if (data.position.x + data.w < SCREEN_WIDTH)
            {
                movement.x += speed;
            }
        }

        // Update the position using the calculated movement vector
        data.position += movement;
    }
    auto Engine::   Object::   draw     ( SDL_Renderer* renderer )              const
    -> void
    {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect dotRect = { data.position.getXInt(), data.position.getYInt(), data.w, data.h };
        SDL_RenderFillRect(renderer, &dotRect);
    }
    auto Engine::   Object::   isStatic ()                                      const
    -> bool
    {
        return data.staticObject;
    }
#pragma endregion
#pragma region 'Engine::Base'
    auto Engine::   Base::  run             ()
    -> int
    {
        init();
        while (running)
        {
            pollForEvents();
            logic();
            clear();
            draw();
            update();
        }
        cleanup();
        return 0;
    }
    #pragma region 'Sub Functions' run
        auto Engine::   Base::  cleanup ()
        -> void
        {
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
        }
        #pragma region logic 'Sub Functions' 
            auto Engine::   Base::  applyPhysics ()
            -> void
            {
                /// NOTE: This Is The Gravity Function
                for (auto& object : objects)
                {
                    if (object.isStatic() == false)
                    {
                        object.move(Direction::DOWN, 3);
                    }
                }
            }
        #pragma endregion
        auto Engine::   Base::  logic           ()
        -> void
        {
            applyPhysics();
            KeyObject::Instance()->handleKeyEvent();
        }
        auto Engine::   Base::  clear           ()
        -> void
        {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
        }
        auto Engine::   Base::  draw            ()
        -> void
        {
            for (auto const& object : objects)
            {
                object.draw(renderer);
            }
        }
        auto Engine::   Base::  update          ()
        -> void
        {
            SDL_RenderPresent(renderer);        // Update screen
            SDL_Delay(16);                  // Approximately 60 frames per second
        }
        auto Engine::   Base::  pollForEvents   ()
        -> void
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
    auto Engine::   Base::  init            ()
    -> int
    {
        initSDL();
        createWindow();
        createRenderer();
        setupMovementKeys();
        return 0;
    }
    #pragma region 'Sub Functions' init
        auto Engine::   Base::  initSDL              ()
        -> int
        {
            if (SDL_Init(SDL_INIT_VIDEO) < 0)
            {
                running = false;
                return -1;
            }
            return 0;
        }
        auto Engine::   Base::  createWindow         ()
        -> int
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
        auto Engine::   Base::  createRenderer       ()
        -> int
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
        auto Engine::   Base::  setupMovementKeys    ()
        -> void
        {
            KeyObject::Instance()->addActionForKey(SDL_SCANCODE_W, [&]()
            -> void
            {
                for (auto& object : objects)
                {
                    if (object.isStatic() == false)
                    {
                        object.move(Direction::UP);
                    }
                }
            });
            KeyObject::Instance()->addActionForKey(SDL_SCANCODE_S, [&]() 
            -> void
            {
                for (auto& object : objects)
                {
                    if (object.isStatic() == false)
                    {
                        object.move(Direction::DOWN);
                    }
                }
            });
            KeyObject::Instance()->addActionForKey(SDL_SCANCODE_A, [&]()
            -> void
            {
                for (auto& object : objects)
                {
                    if (object.isStatic() == false)
                    {
                        object.move(Direction::LEFT);
                    }
                }
            });
            KeyObject::Instance()->addActionForKey(SDL_SCANCODE_D, [&]()
            -> void
            {
                for (auto& object : objects)
                {
                    if (object.isStatic() == false)
                    {
                        object.move(Direction::RIGHT);
                    }
                }
            });
            KeyObject::Instance()->addActionForKey(SDL_SCANCODE_ESCAPE, [&]()
            -> void
            {
                running = false;
            });
        }
    #pragma endregion
    auto Engine::   Base::  createObject    ( const Object& object )
    -> void
    {
        objects.emplace_back(object);
    }

    Engine::Base::Base (const string& window_title, int window_width, int window_height)
    : window_title(window_title), SCREEN_WIDTH(window_width), SCREEN_HEIGHT(window_height)
    {
        Engine::SCREEN_WIDTH    = SCREEN_WIDTH;
        Engine::SCREEN_HEIGHT   = SCREEN_HEIGHT;
    }
#pragma endregion