#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <vector>

using namespace std;

/* Constants */
#pragma region Constants
    const int SCREEN_WIDTH  = 800;
    const int SCREEN_HEIGHT = 600;
    const int DOT_SIZE      = 10;
#pragma endregion

/* Macros */
#pragma region Macros
    #define SDL2_ERROR_CHECK( x ) if (x < 0) { return -1; }
#pragma endregion

namespace Engine
{
    /* Enums */
    #pragma region Enums
        class  Direction { public: enum
        {
            UP      = 1 << 0,
            DOWN    = 1 << 1,
            LEFT    = 1 << 2,
            RIGHT   = 1 << 3,
        };};
    #pragma endregion
}