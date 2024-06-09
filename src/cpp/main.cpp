#include <SDL2/SDL.h>
#include <iostream>
#include <string>

using namespace std;

#define CHECK_KEY(key, statement) if (state[SDL_SCANCODE_##key]) statement 

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int DOT_SIZE = 10;


struct Dot
{
    int x       = SCREEN_WIDTH / 2,
        y       = SCREEN_HEIGHT / 2,
        speed   = 5,
        size    = 10;
};

class Game
{
private:
    SDL_Window*     window      = nullptr;
    SDL_Renderer*   renderer    = nullptr;
    bool            running     = true;
    SDL_Event       event;
    Dot             dot;

    auto init (const string& title, int width, int height) -> int
    {
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            running = false;
        }

        window = SDL_CreateWindow
        (
            title.c_str(),
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            width,
            height,
            SDL_WINDOW_SHOWN
        );

        if (!window)
        {
            SDL_Quit();
            return -1;
        }

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
        if (!renderer)
        {
            SDL_DestroyWindow(window);
            SDL_Quit();
            return -1;
        }

        return 0;
    }

    auto cleanup () -> void
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    auto logic () -> void
    {
        /// Handle keyboard input
        const Uint8* state = SDL_GetKeyboardState(NULL);
        if (state[SDL_SCANCODE_ESCAPE])
        {
            running = false;
        }
        if (state[SDL_SCANCODE_W])
        {
            if (dot.y > 0)
            {
                dot.y -= dot.speed;
            }
        }
        if (state[SDL_SCANCODE_S])
        {
            if (dot.y + DOT_SIZE < SCREEN_HEIGHT)
            {
                dot.y += dot.speed;
            }
        }
        if (state[SDL_SCANCODE_A])
        {
            if (dot.x > 0)
            {
                dot.x -= dot.speed;
            }
        }
        if (state[SDL_SCANCODE_D])
        {
            if (dot.x + DOT_SIZE < SCREEN_WIDTH)
            {
                dot.x += dot.speed;
            }
        }
    }

    auto clear () -> void
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
    }

    auto draw () -> void
    {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect dotRect = { dot.x, dot.y, DOT_SIZE, DOT_SIZE };
        SDL_RenderFillRect(renderer, &dotRect);
    }

    auto update () -> void
    {
        SDL_RenderPresent(renderer);        // Update screen
        SDL_Delay(16);                  // Approximately 60 frames per second
    }

public:
    Game() {}
    ~Game() {}

    auto run () -> int
    {
        if (init("Move the Dot", SCREEN_WIDTH, SCREEN_HEIGHT) < 0)
        {
            return -1;
        }

        while (running)
        {
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT)
                {
                    running = false;
                }
            }
        
            logic();
            clear();
            draw();
            update();
        }

        cleanup();
        return 0;
    }
};

int main(int argc, char* argv[])
{
    Game game;
    return game.run();
}