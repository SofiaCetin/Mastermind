#ifndef GUI_H_
#define GUI_H_

#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gui.h"

extern const SDL_Color white;
extern const SDL_Color black;
extern const SDL_Color red;

typedef enum{
    Spawner,
    Moveable
}PawnType;

typedef struct Button{
    float x;
    float y;
    float w;
    float h;
    SDL_Color color;
    char* text;
    bool status;
    float text_w;
    float text_h;
    SDL_Texture* text_texture;

}Button;

typedef struct Pawn{
    float x;
    float y;
    float w;
    float h;
    SDL_Color color;
    bool activated;
    PawnType type;
}Pawn;


Button* create_button(float x, float y, float w, float h, SDL_Color color, char* text, bool status, TTF_Font* font, SDL_Renderer* renderer);
void draw_button(SDL_Renderer* renderer, Button* button);
Pawn* create_pawn(float x, float y, float w, float h, SDL_Color color, bool activated, PawnType type);
void draw_pawn(SDL_Renderer* renderer, Pawn* pawn);

#endif