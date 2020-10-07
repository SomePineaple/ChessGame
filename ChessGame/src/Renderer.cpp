#include "Renderer.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <iostream>
#include <string>

#include "Engine.h"

ChessRenderer::ChessRenderer(int width, int height){
    running = true;
    sqSize = width / DIMENTION;
    //Init SDL, the window and the renderer
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0){
        std::cout << "SDL Initialized\n";
        window = SDL_CreateWindow("Simple chess game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
        if (window){
            std::cout << "Created window\n";
        }
        renderer = SDL_CreateRenderer(window, -1, 0);

        if (renderer){
            std::cout << "Created renderer\n";
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        }

        engine = new Engine();
        if(engine){
            std::cout << "Initialized engine\n";
        }

    sqRect.h = sqSize;
    sqRect.w = sqSize;
    }
}

// Keeps track of all the key presses we need to listen for, also handles mouse
// clicks and any other events, could be called EventHandler
void ChessRenderer::KeyHandler(){
    SDL_Event event;
    SDL_PollEvent(&event);
    switch(event.type){
        case SDL_QUIT:
            running = false;
            break;
        case SDL_MOUSEBUTTONDOWN:
            int xmouse;
            int ymouse;
            SDL_GetMouseState(&xmouse, &ymouse);
            ymouse = ymouse / sqSize + 1;
            xmouse = xmouse / sqSize + 1;
            if(clickX == xmouse && clickY == ymouse){
                clickX = 0;
                clickY = 0;
                std::cout << "Unselected square\n";
                break;
            } else{
                clickX = xmouse;
                clickY = ymouse;
            }
            if(prevClickX == 0){
                prevClickX = clickX;
                prevClickY = clickY;
                break;
            } else{
                engine->MakeMove(prevClickX - 1, prevClickY - 1, clickX - 1, clickY - 1);
                prevClickX = 0;
                prevClickY = 0;
                clickX = 0;
                clickY = 0;
                break;
            }
            break;
        case SDL_KEYDOWN:
            switch(event.key.keysym.sym){
                case SDLK_ESCAPE:
                    running = false;
                    break;
                case SDLK_z:
                	engine->UndoMove();
                	break;
                default:
                    break;
                }
                break;
        default:
                break;
    }
}

// Updates everything that needs to be done, will call methods to move pieces if need be
void ChessRenderer::Update(){
    this->KeyHandler();
    if (engine->whiteMove) {
    	SDL_SetWindowTitle(window, "White move");
    } else {
    	SDL_SetWindowTitle(window, "Black move");
    }
}

// Renders everything to the window
void ChessRenderer::Render(){
    SDL_RenderClear(renderer);
    this->RenderBoard();
    this->RenderPieces();
    SDL_RenderPresent(renderer);
}

// Renders the squares on the board
void ChessRenderer::RenderBoard(){
    for (int r = 0; r < DIMENTION; r++){
        for (int c = 0; c < DIMENTION; c++){
            sqRect.x = c * sqSize;
            sqRect.y = r * sqSize;
            // Changes the color for every other square
            if((r + c) % 2 == 1){
                SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
            } else{
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            }
            SDL_RenderFillRect(renderer, &sqRect);
        }
    }
}

// Renders the pieces
void ChessRenderer::RenderPieces(){
    SDL_Rect destRect;
    std::string path = "assets/";
    destRect.h = sqSize;
    destRect.w = sqSize;
    for (int r = 0; r < DIMENTION; r++){
        for (int c = 0; c < DIMENTION; c++){
            if(engine->GetPiece(c, r) == "--"){
                continue;
            }
            std::string fullPath = path + engine->GetPiece(c, r) + ".png";
            SDL_Texture *pieceImg = IMG_LoadTexture(renderer, fullPath.c_str());
            if(pieceImg == nullptr){
                std::cout << "Unable to load image at " << fullPath << ", ABORTING\n";
                running = false;
            }
            destRect.x = c * sqSize;
            destRect.y = r * sqSize;
            SDL_RenderCopy(renderer, pieceImg, NULL, &destRect);
            SDL_DestroyTexture(pieceImg);
        }
    }
}
