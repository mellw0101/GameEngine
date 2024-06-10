#include "../include/Engine.hpp"

using namespace std;

#pragma region Engine::Object
    auto Engine::   Object::   init     ( const ObjectData& data )
    -> void
    {
        this->data = data;
    }
    auto Engine::   Object::   move     ( s32 Direction, f64 speed_override )
    -> void
    {
        s32 direction = Direction;
        s32 speed = 0;

        if (speed_override == 0)
        {
            speed = data.speed;
        }
        else
        {
            speed = speed_override;
        }

        if (Direction & Direction::UP)
        {
            if (data.y > 0)
            {
                data.y -= data.speed;
            }
        }
        if (Direction & Direction::DOWN)
        {
            if (data.y + data.h < SCREEN_HEIGHT)
            {
                data.y += data.speed;
            }
        }
        if (Direction & Direction::LEFT)
        {
            if (data.x > 0)
            {
                data.x -= data.speed;
            }
        }
        if (Direction & Direction::RIGHT)
        {
            if (data.x + data.w < SCREEN_WIDTH)
            {
                data.x += data.speed;
            }
        }
    }
    auto Engine::   Object::   draw     ( SDL_Renderer* renderer ) const
    -> void
    {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect dotRect = { data.x, data.y, data.w, data.h };
        SDL_RenderFillRect(renderer, &dotRect);
    }
    auto Engine::   Object::   isStatic () const
    -> bool
    {
        return data.staticObject;
    }
#pragma endregion