#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>

#include "Renderer.h"

const int WIDTH = 512;
const int HEIGHT = WIDTH;

ChessRenderer *chessRenderer;

int main (){
    const int FPS = 60;
    const int frameDelay = 100 / FPS;

    Uint32 frameStart;
    int frameTime;

    chessRenderer = new ChessRenderer(WIDTH, HEIGHT);

    while (chessRenderer->running){
        frameStart = SDL_GetTicks();
        chessRenderer->Update();
        chessRenderer->Render();
        frameTime = SDL_GetTicks() - frameStart;
        if(frameDelay > frameTime){
            SDL_Delay(frameDelay - frameTime);
        }
    }
    return 0;
}
