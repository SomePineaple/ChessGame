#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL_rect.h>

class Engine;
struct SDL_Window;

struct SDL_Renderer;

class ChessRenderer{
public:
    ChessRenderer(int width, int height);
    ~ChessRenderer();

    void Render();
    void Update();
    void KeyHandler();
    int Quit();

    bool running;

private:
    SDL_Window *window;
    SDL_Renderer *renderer;

    Engine *engine;

    void RenderPieces();
    void RenderBoard();
    void RenderCheckMateBoard();

    const int DIMENTION = 8;
    int sqSize;
    SDL_Rect sqRect;

    int prevClickX = 0;
    int prevClickY = 0;
    int clickX = 0;
    int clickY = 0;
};


#endif /* RENDERER_H_ */
