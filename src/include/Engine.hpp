/// FILE: Engine.hpp
#pragma once

#include "Global.hpp"
#include "Vec2D.hpp"

namespace Engine
{
#pragma region 'ObjectData'
    /**
        @struct ObjectData
        @brief This struct holds the data of the object.
        */
    typedef struct __2D_OBJECT_DATA_T__
    {
        Vec2D position;         // position,  Coordinate Of The Object
        Vec2D velocity;         // velocity,  Speed Of The Object In Pixels Per Frame

        s32 w;                  //  width,  Coordinate Of The Object
        s32 h;                  // height,  Coordinate Of The Object
        f32 speed;              //  speed,  if ' staticObject ' Is False, Speed Of The Object`s Acceleration In Pixels Per Frame ( 60 Frames Per Second )
        u32 id;                 //     id,  Unique Identifier For The Object   
        string name;            //   Name,  String That Holds Name of the object For Debugging Purposes
        
        bool staticObject;      // If true, the object will be fixed to the 'World'
    }
    ObjectData2D;
#pragma endregion
#pragma region 'Object'
    /**
        @struct Object
        @brief This struct holds the object.
        */
    typedef struct __SDL2_OBJECT_T__
    {
        ObjectData2D data;

        auto init ( const ObjectData2D& data )
        -> void;
        
        auto move ( s32 Direction, f32 speed_override = 0.0)
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
            /**
                @b Function: @c 'run'
                    @return int
                    @brief:
                        @note This Function Is The
                        @note Main Loop For The Engine,
                */
            public : auto run ()    -> int;
            #pragma region 'Sub Functions' run
                private: auto cleanup       ()  ->  void;
                #pragma region 'logic Sub Functions' 
                    private: auto applyPhysics () -> void;
                #pragma endregion
                private: auto logic         ()  ->  void;
                private: auto clear         ()  ->  void;
                private: auto draw          ()  ->  void;
                private: auto update        ()  ->  void;
                private: auto pollForEvents ()  ->  void;
            #pragma endregion
            
            private: auto init () -> int;
            #pragma region 'Sub Functions' init
                private : auto initSDL              ()  ->  int;
                private : auto createWindow         ()  ->  int;
                private : auto createRenderer       ()  ->  int;
                private : auto setupMovementKeys    ()  ->  void;
            #pragma endregion
            
            public : auto createObject  ( const Object& object ) -> void;
        #pragma endregion
        Base (const string& window_title, int window_width, int window_height);
    };
#pragma endregion
}