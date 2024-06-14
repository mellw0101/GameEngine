/// FILE: Engine.hpp
#pragma once

#include <SDL2/SDL_rect.h>
#include "Global.hpp"
#include "Structs.h"
#include "Vec2D.hpp"

namespace Engine {
#pragma region Tools
    class Tools
    {
    public:
        // Gravitational constant in pixels per frame squared
        // static constexpr float GRAVITY = 9.8f / 60.0f; // Assuming 60 frames per second

        /**
            @b Function: @c 'random'
            @return @c T
            @brief:
                @note This function returns a random number between the min and max values.]
            */
        template <typename T>
        FORCE_INLINE T
        random(T min, T max)
        {
            return min + (rand() % (max - min + 1));
        }

        /**
            @b Function: @c 'calculateGravity'
            @return @c f32
            @brief:
                @note Function to calculate the velocity change due to gravity
            */
        FORCE_INLINE f32
        calculateGravity(Vec2D initialVelocity)
        {
            return GRAVITY * Tools::square(FRAMETIME);
        }

        FORCE_INLINE auto
        square(f32 x) -> f32
        {
            return x * x;
        }

        FORCE_INLINE auto
        cube(f32 x) -> f32
        {
            return x * x * x;
        }
        FORCE_INLINE auto
        clamp(f32 value, f32 min, f32 max) -> f32
        {
            return (value < min) ? min : (value > max) ? max : value;
        }
        FORCE_INLINE auto
        lerp(f32 a, f32 b, f32 t) -> f32
        {
            return a + t * (b - a);
        }
        FORCE_INLINE auto
        inverseLerp(f32 a, f32 b, f32 value) -> f32
        {
            return (value - a) / (b - a);
        }
        FORCE_INLINE auto
        remap(f32 value, f32 a, f32 b, f32 c, f32 d) -> f32
        {
            return lerp(c, d, inverseLerp(a, b, value));
        }
        FORCE_INLINE auto
        smoothStep(f32 edge0, f32 edge1, f32 x) -> f32
        {
            f32 t = clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
            return t * t * (3.0f - 2.0f * t);
        }
        FORCE_INLINE auto
        smootherStep(f32 edge0, f32 edge1, f32 x) -> f32
        {
            f32 t = clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
            return t * t * t * (t * (t * 6 - 15) + 10);
        }
        FORCE_INLINE auto
        toRadians(f32 degrees) -> f32
        {
            return degrees * (M_PI / 180.0f);
        }
        FORCE_INLINE auto
        toDegrees(f32 radians) -> f32
        {
            return radians * (180.0f / M_PI);
        }
        FORCE_INLINE auto
        isPowerOfTwo(u32 value) -> bool
        {
            return (value != 0) && ((value & (value - 1)) == 0);
        }
        FORCE_INLINE auto
        nextPowerOfTwo(u32 value) -> u32
        {
            value--;
            value |= value >> 1;
            value |= value >> 2;
            value |= value >> 4;
            value |= value >> 8;
            value |= value >> 16;
            value++;
            return value;
        }
        FORCE_INLINE auto
        isEven(u32 value) -> bool
        {
            return (value % 2) == 0;
        }
        FORCE_INLINE auto
        isOdd(u32 value) -> bool
        {
            return (value % 2) != 0;
        }
        FORCE_INLINE auto
        isWithinRange(f32 value, f32 min, f32 max) -> bool
        {
            return (value >= min) && (value <= max);
        }
        FORCE_INLINE auto
        isWithinRange(s32 value, s32 min, s32 max) -> bool
        {
            return (value >= min) && (value <= max);
        }
        FORCE_INLINE auto
        isWithinRange(u32 value, u32 min, u32 max) -> bool
        {
            return (value >= min) && (value <= max);
        }
        FORCE_CONSTEXPR auto
        calculatePerFrameGravity() -> f32
        {
            return GRAVITY / (FPS * FPS);
        }
        FORCE_ACON
        getF32Rect(f32 x, f32 y, f32 w, f32 h)->SDL_FRect
        {
            return {x, y, w, h};
        }
        FORCE_ACON
        getRect(s32 x, s32 y, s32 w, s32 h)->SDL_Rect
        {
            return {x, y, w, h};
        }
        typedef class __RECT_T__
        {
            s32 x, y, w, h;

            auto
            operator==(const __RECT_T__& other) const -> bool
            {
                return (x == other.x && y == other.y && w == other.w && h == other.h);
            }
            auto
            operator!=(const __RECT_T__& other) const -> bool
            {
                return !(*this == other);
            }
            __RECT_T__&
            operator=(const __RECT_T__& other)
            {
                x = other.x;
                y = other.y;
                w = other.w;
                h = other.h;
                return *this;
            }
            __RECT_T__&
            operator+=(const __RECT_T__& other)
            {
                x += other.x;
                y += other.y;
                w += other.w;
                h += other.h;
                return *this;
            }
            __RECT_T__&
            operator-=(const __RECT_T__& other)
            {
                x -= other.x;
                y -= other.y;
                w -= other.w;
                h -= other.h;
                return *this;
            }
            __RECT_T__&
            operator*=(const __RECT_T__& other)
            {
                x *= other.x;
                y *= other.y;
                w *= other.w;
                h *= other.h;
                return *this;
            }
            __RECT_T__&
            operator/=(const __RECT_T__& other)
            {
                x /= other.x;
                y /= other.y;
                w /= other.w;
                h /= other.h;
                return *this;
            }
            __RECT_T__(s32 inX, s32 inY, s32 inW, s32 inH)
                : x(inX)
                , y(inY)
                , w(inW)
                , h(inH)
            {}
        } Rect;
        typedef class __FRECT_T__
        {
            f32 x, y, w, h;
        } FRect;
    };
#pragma endregion
#pragma region Object2D
    /**
        @struct ObjectData
        @brief This struct holds the data of the object Data For 2D.
        */
    typedef struct __OBJECT_DATA_2D_T__
    {
        Vec2D position; // position,  Coordinate Of The Object
        s32   w;
        s32   h;
        f32   speed;

        /**
            @brief The state of the engine (running, paused, etc.) Using Only Bitwise Operations
            @note if (state & (1 << 0)) != 0, then the engine is running
            */
        u32 state;
    } ObjectData2D;

    /**
        @struct Object
        @brief This struct holds the object.
        */
    typedef struct __OBJECT_2D_T__
    {
        ObjectData2D data;

        auto init(const ObjectData2D& data) -> void;
        auto move(s32 Direction, f32 speed_override = 0.0) -> void;
        auto move(Vec2D vel) -> void;
        auto draw(SDL_Renderer* renderer) const -> void;
        auto isStatic() const -> bool;
        auto rect() const -> SDL_Rect;
        auto frect() const -> SDL_FRect;
        auto state() const -> u32;
    } Object;
#pragma endregion
#pragma region KeyObject
    /**
        @brief This map holds the keys and vector of functions.
        */
    typedef umap<u8, vec<func<void()>>> KeyMap;

    /**
        @struct KeyObject
        @brief This struct holds the keymap.
        */
    typedef class __KEY_OBJECT_T__
    {
    private:
        KeyMap keymap;

        /**
            @b Function: @c 'Instance'
            @return @c KeyObject*
            @brief:
                @note This function returns the instance of the KeyObject.
                @note If the instance Does Not Yet Exist, It Creates One.
                @note This Function Is The Only Way To Get The Instance Of The KeyObject.
            */
    public:
        FORCE_INLINE KeyObject*
        Instance()
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
        template <typename F, typename... Args>
        void
        addActionForKey(u8 key, F&& func, Args&&... args)
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
        void
        handleKeyEvent()
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
    } KeyObject;
#pragma endregion
#pragma region Core
    /**
        @class @b 'Base'
        @return @c
        @brief Base class To Control The Engine
        */
    class Core
    {
    private:
    private:
        string window_title;

    private:
        u32 frames;
        /**
            @brief The state of the engine (running, paused, etc.) Using Only Bitwise Operations
            @note if (state & (1 << 0)) != 0, then the engine is running
            */
    private:
        u32 state;

    private:
        SDL_Window* window = nullptr;

    private:
        SDL_Renderer* renderer = nullptr;

    private:
        SDL_Event event;

        auto init() -> int;

    public:
        vec<Object2D> objects;
        bool          running = true;

    public:
        auto run() -> int;
        auto createObject(const Object2D& object) -> void;
        FORCE_INLINE Core*
        Inst(const string& window_title, const int& window_width, const int& window_height)
        {
            if (Engine::CoreInstance == nullptr)
            {
                lock_guard<std::mutex> lock(mutex);
                if (Engine::CoreInstance == nullptr)
                {
                    Engine::CoreInstance = new Core {window_title, window_width, window_height};
                }
            }
            return Engine::CoreInstance;
        }
        void shutdown();

#pragma region 'Sub Functions' run
    private:
        auto cleanup() -> void;
#pragma region 'logic Sub Functions'
    private:
        auto applyPhysics() -> void;
#pragma endregion
    private:
        auto logic() -> void;

    private:
        auto clear() -> void;

    private:
        auto draw() -> void;

    private:
        auto update() -> void;

    private:
        auto pollForEvents() -> void;
#pragma endregion
#pragma region 'Sub Functions' init
    private:
        auto initSDL() -> int;

    private:
        auto createWindow() -> int;

    private:
        auto createRenderer() -> int;

    private:
        auto setupKeys() -> void;
#pragma endregion

    public:
        Core(const string& window_title, const int& window_width, const int& window_height);
    };
#pragma endregion
} // namespace Engine
