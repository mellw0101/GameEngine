/// FILE: Engine.hpp
#pragma once

#include "Global.hpp"

namespace Engine
{
#pragma region 'ObjectData'
    /**
        @struct ObjectData
        @brief This struct holds the data of the object.
        */
    typedef struct __SDL2_OBJECT_DATA_T__
    {
        s32 x;                  //      x,  Coordinate Of The Object
        s32 y;                  //      y,  Coordinate Of The Object
        s32 w;                  //  width,  Coordinate Of The Object
        s32 h;                  // height,  Coordinate Of The Object
        f64 speed;              //  speed,  if ' staticObject ' Is False, Speed Of The Object`s Acceleration In Pixels Per Frame ( 60 Frames Per Second )
        Uint32 id;              //     id,  Unique Identifier For The Object   
        string name;            //   Name,  String That Holds Name of the object For Debugging Purposes
        
        bool staticObject;      // If true, the object will be fixed to the 'World'
    }
    ObjectData;
#pragma endregion
#pragma region 'Object'
    /**
        @struct Object
        @brief This struct holds the object.
        */
    typedef struct __SDL2_OBJECT_T__
    {
        ObjectData data;

        auto init ( const ObjectData& data )
        -> void;
        
        auto move ( int Direction, f64 speed_override = 0.0)
        -> void;
        
        auto draw ( SDL_Renderer* renderer ) const
        -> void;
        
        auto isStatic () const
        -> bool;
    }
    Object;
#pragma endregion
#pragma region 'KeyObject'
    /**
        @brief This map holds the keys and vector of functions.
        */
    typedef umap<u8, vec<func<void()>>> KeyMap;

    /**
        @struct KeyObject
        @brief This struct holds the keymap.
        */
    typedef class __SDL2_KEY_OBJECT_T__
    {
        private : KeyMap keymap;

        /**
            @b Function: @c 'Instance'
            @return @c KeyObject*
            @brief:
                @note This function returns the instance of the KeyObject.
                @note If the instance Does Not Yet Exist, It Creates One.
                @note This Function Is The Only Way To Get The Instance Of The KeyObject.
            */
        public : static auto Instance()
        -> KeyObject*
        {
            if (!KeyObjectInstance)
            {
                KeyObjectInstance = new KeyObject();
            }
            return KeyObjectInstance;
        }

        /** 
            @b Function: @c 'addActionForKey'
                @return void
                @brief:
                    @note This function adds an action for a key.
                    @note The function is called when the key is pressed.
            */
        public: template <typename F, typename... Args> auto addActionForKey(u8 key, F&& func, Args&&... args)
        -> void
        {
            auto boundFunc = std::bind(std::forward<F>(func), std::forward<Args>(args)...);
            keymap[key].emplace_back([boundFunc]() { boundFunc(); });
        }        
        /**
            @b Function: @c 'handleKeyEvent'
                @return void
                @brief:
                    @note This function handles the key event.
                    @note Only Called From Func 'run' In 'Engine::Base' Class
                    @note This Function Is Called Every Frame
            */
        public: auto
        handleKeyEvent ( void )
        -> void
        {
            const u8* state = SDL_GetKeyboardState(nullptr);
            for (auto const& [key, funcs] : keymap)
            {
                if (state[key])
                {
                    for (auto const& func : funcs)
                    {
                        func();
                    }
                }
            }
        }
    }
    KeyObject;
#pragma endregion
#pragma region 'Base'
    /**
        @class @b 'Base'
        @return @c 
        @brief Base class To Control The Engine 
        */
    class Base
    {
        #pragma region 'Variabels'
            private: s32                    SCREEN_WIDTH;
            private: s32                    SCREEN_HEIGHT;
            private: string                 window_title;

            private: SDL_Window*            window      = nullptr;
            private: SDL_Renderer*          renderer    = nullptr;
            private: bool                   running     = true;
            private: SDL_Event              event;
            private: vec<Engine::Object>    objects;
        #pragma endregion
        #pragma region 'Functions'
            #pragma region run 'Sub Functions'
                private: auto cleanup ()
                -> void
                {
                    SDL_DestroyRenderer(renderer);
                    SDL_DestroyWindow(window);
                    SDL_Quit();
                }
                #pragma region logic 'Sub Functions' 
                    private: auto applyPhysics ()
                    -> void
                    {
                        for (auto& object : objects)
                        {
                            if (object.isStatic() == false)
                            {
                                object.move(Direction::DOWN, 0.1);
                            }
                        }
                    }
                #pragma endregion
                private: auto logic ()
                -> void
                {
                    applyPhysics();
                    KeyObject::Instance()->handleKeyEvent();
                }
                private: auto clear ()
                -> void
                {
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_RenderClear(renderer);
                }
                private: auto draw ()
                -> void
                {
                    for (auto const& object : objects)
                    {
                        object.draw(renderer);
                    }
                }
                private: auto update ()
                -> void
                {
                    SDL_RenderPresent(renderer);        // Update screen
                    SDL_Delay(16);                  // Approximately 60 frames per second
                }
                private: auto pollForEvents ()
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
            /**
                @b Function: @c 'run'
                    @return void
                    @brief:
                        @note This Function Is The
                        @note Main Loop For The Engine,
                */
            public : auto run ()
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
            #pragma region init 'Sub Functions'
                private : auto initSDL              ()
                -> int
                {
                    if (SDL_Init(SDL_INIT_VIDEO) < 0)
                    {
                        running = false;
                        return -1;
                    }
                    return 0;
                }
                private : auto createWindow         ()
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
                private : auto createRenderer       ()
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
                private : auto setupMovementKeys    ()
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
            private: auto init ()
            -> int
            {
                initSDL();
                createWindow();
                createRenderer();
                setupMovementKeys();
                return 0;
            }
            public : auto createObject  ( const Object& object )
            -> void
            {
                objects.emplace_back(object);
            }
        #pragma endregion
        Base (const string& window_title, int window_width, int window_height)
        : window_title(window_title), SCREEN_WIDTH(window_width), SCREEN_HEIGHT(window_height)
        {
            Engine::SCREEN_WIDTH    = SCREEN_WIDTH;
            Engine::SCREEN_HEIGHT   = SCREEN_HEIGHT;
        }
    };

#pragma endregion
}