#include "renderer.hpp"
#include "functions.hpp"
#include "text.hpp"
#include <iostream>
#include <vector>
#include <cstdlib>  
#include <ctime>   
#include <cmath>
#include <algorithm>
#include <iomanip>

int main(int argc, char* argv[]) {

    const int TARGET_FPS = 60;
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;

    const int TARGET_FRAME_TIME = 1000 / TARGET_FPS;
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    if (!initialize(window, renderer, SCREEN_WIDTH, SCREEN_HEIGHT)) {

        return 1;
    }
    
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    bool running = true;
    SDL_Event e;
    Uint32 currentFrameTime = SDL_GetTicks();
    Uint32 lastFrameTime = 0;
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    const Uint8* keystate = SDL_GetKeyboardState(NULL);
    int mouse_x, mouse_y;
    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = false;
            } 
        }
        Uint32 mouseState = SDL_GetMouseState(&mouse_x, &mouse_y);
        lastFrameTime = currentFrameTime;
        currentFrameTime = SDL_GetTicks();
        Uint32 deltaTime = currentFrameTime - lastFrameTime;
        std::cout << "FPS: " << 1000/deltaTime << std::endl;
        Uint32 frameTime = SDL_GetTicks() - currentFrameTime;
        if (frameTime < TARGET_FRAME_TIME) {
            SDL_Delay(TARGET_FRAME_TIME - frameTime); 
        }
        if (keystate[SDL_SCANCODE_UP]) {
            std::cout << "UP pressed\n";
        }
        if (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) {
            std::cout << mouse_x << " " << mouse_y << "\n";
        }
        drawScreen(renderer, { 0,0,0,10 });
        drawThickLine(renderer, 100, 100, 200, 300, 10, { 255,0,0,10 });

        drawRotatedRectangle(renderer, 300, 300, 100, 150, 30, { 255,0,0,255 });
        SDL_RenderPresent(renderer);
    }

    cleanup(window, renderer);
    return 0;
}

