#include "../include/Engine.hpp"

// /**
//     @class Game
    
//     @brief This class is responsible for managing the game loop and the game objects.
//     */
// class Game
// {
//     private: SDL_Window*             window      = nullptr;
//     private: SDL_Renderer*           renderer    = nullptr;
//     private: bool                    running     = true;
//     private: SDL_Event               event;
//     private: Engine::Object          dot;

//     /* Initialization Subsystems */ #pragma region
//         private: auto init_sdl          ()                                              -> int
//         {
//             if (SDL_Init(SDL_INIT_VIDEO) < 0)
//             {
//                 running = false;
//                 return -1;
//             }
//             return 0;
//         }
//         private: auto create_window     ( const string& title, int width, int height )  -> int
//         {
//             window = SDL_CreateWindow
//             (
//                 title.c_str(),
//                 SDL_WINDOWPOS_UNDEFINED,
//                 SDL_WINDOWPOS_UNDEFINED,
//                 width,
//                 height,
//                 SDL_WINDOW_SHOWN
//             );
//             if (!window)
//             {
//                 SDL_Quit();
//                 return -1;
//             }
//             return 0;
//         }
//         private: auto create_renderer   ()                                              -> int
//         {
//             renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
//             if (!renderer)
//             {
//                 SDL_DestroyWindow(window);
//                 SDL_Quit();
//                 return -1;
//             }
//             return 0;
//         }
//         private: auto create_objects    ()                                              -> void
//         {
//             dot.init({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, DOT_SIZE, DOT_SIZE, 10 });
//         }
//     #pragma endregion
//     private: auto init      ( const string& title, int width, int height )  -> int
//     {
//         init_sdl();
//         create_window(title, width, height);
//         create_renderer();
//         create_objects();
//         return 0;
//     }
//     private: auto cleanup   ()                                              -> void
//     {
//         SDL_DestroyRenderer(renderer);
//         SDL_DestroyWindow(window);
//         SDL_Quit();
//     }
//     private: auto logic     ()                                              -> void
//     {
//         /// Handle keyboard input
//         const Uint8* state = SDL_GetKeyboardState(NULL);
//         if (state[SDL_SCANCODE_ESCAPE])
//         {
//             running = false;
//         }
//         if (state[SDL_SCANCODE_W])
//         {
//             dot.move(Engine::Direction::UP);
//         }
//         if (state[SDL_SCANCODE_S])
//         {
//             dot.move(Engine::Direction::DOWN);
//         }
//         if (state[SDL_SCANCODE_A])
//         {
//             dot.move(Engine::Direction::LEFT);
//         }
//         if (state[SDL_SCANCODE_D])
//         {
//             dot.move(Engine::Direction::RIGHT);
//         }
//     }
//     private: auto clear     ()                                              -> void
//     {
//         SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
//         SDL_RenderClear(renderer);
//     }
//     private: auto draw      ()                                              -> void
//     {
//         dot.draw(renderer);
//     }
//     private: auto update    ()                                              -> void
//     {
//         SDL_RenderPresent(renderer);        // Update screen
//         SDL_Delay(16);                  // Approximately 60 frames per second
//     }

//     public:  Game() {}
//     public:  ~Game() {}
//     public:  auto run () -> int
//     {
//         if (init("Move the Dot", SCREEN_WIDTH, SCREEN_HEIGHT) < 0)
//         {
//             return -1;
//         }

//         while (running)
//         {
//             while (SDL_PollEvent(&event))
//             {
//                 if (event.type == SDL_QUIT)
//                 {
//                     running = false;
//                 }
//             }
        
//             logic();
//             clear();
//             draw();
//             update();
//         }

//         cleanup();
//         return 0;
//     }
// };

int main(int argc, char* argv[])
{
    // Game game;
    // return game.run();

    Engine::Base engine{"Move the Dot", 800, 600};
    engine.createObject({{
        {Engine::SCREEN_WIDTH / 2, Engine::SCREEN_HEIGHT / 2},
        {0, 0},
        10,
        10,
        5,
        0
    }});

    return engine.run();
}