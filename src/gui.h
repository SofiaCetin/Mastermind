#ifndef GUI_H_
#define GUI_H_

#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "gui.h"

extern const SDL_Color white;
extern const SDL_Color black;
extern const SDL_Color red;

typedef struct Button{
    float x;
    float y;
    float w;
    float h;
    SDL_Color color;
    char* text;
    bool status;
}Button;

typedef struct Pawn{
    float x;
    float y;
    float w;
    float h;
    SDL_Color color;
    bool status;
}Pawn;

Button* create_button(float x, float y, float w, float h, SDL_Color color, char* text, bool status);
void draw_button(SDL_Renderer* renderer, Button* button);
void show_button(Button* button);


#endif