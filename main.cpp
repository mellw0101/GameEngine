#include <SDL2/SDL.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int DOT_SIZE = 10;

int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("Move the Dot",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SCREEN_WIDTH,
                                          SCREEN_HEIGHT,
                                          SDL_WINDOW_SHOWN);

    if (!window)
    {
        SDL_Quit();
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    if (!renderer)
    {
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    bool running = true;
    SDL_Event event;
    int dotX = SCREEN_WIDTH / 2;
    int dotY = SCREEN_HEIGHT / 2;
    int speed = 5;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
        }

        // Handle keyboard input
        const Uint8* state = SDL_GetKeyboardState(NULL);
        if (state[SDL_SCANCODE_UP])
        {
            dotY -= speed;
        }
        if (state[SDL_SCANCODE_DOWN])
        {
            dotY += speed;
        }
        if (state[SDL_SCANCODE_LEFT])
        {
            dotX -= speed;
        }
        if (state[SDL_SCANCODE_RIGHT])
        {
            dotX += speed;
        }

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw the dot
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect dotRect = { dotX, dotY, DOT_SIZE, DOT_SIZE };
        SDL_RenderFillRect(renderer, &dotRect);

        // Update screen
        SDL_RenderPresent(renderer);

        SDL_Delay(16); // Approximately 60 frames per second
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}