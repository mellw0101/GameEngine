/// FILE: Engine.hpp
#pragma once

#include "Global.hpp"

namespace Engine
{
    typedef struct __SDL2_OBJECT_DATA_T__
    {
        int x;              //      x,  Coordinate Of The Object
        int y;              //      y,  Coordinate Of The Object
        int w;              //  width,  Coordinate Of The Object
        int h;              // height,  Coordinate Of The Object
        int speed;          //  speed,  if ' staticObject ' Is False, Speed Of The Object`s Acceleration In Pixels Per Frame ( 60 Frames Per Second )
        Uint32 id;          //     id,  Unique Identifier For The Object   
        string name;        //   Name,  String That Holds Name of the object For Debugging Purposes
        bool staticObject;  // If true, the object will be fixed to the 'World'

    } ObjectData;

    typedef struct __SDL2_OBJECT_T__
    {
        ObjectData data;

        auto init ( const ObjectData& data ) -> void;

        auto move ( int Direction ) -> void
        {
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

        auto draw ( SDL_Renderer* renderer ) -> void
        {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_Rect dotRect = { data.x, data.y, data.w, data.h };
            SDL_RenderFillRect(renderer, &dotRect);
        }

    } Object;
}