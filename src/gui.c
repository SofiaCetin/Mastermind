#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "gui.h"

const SDL_Color white = {255, 255, 255, 255};
const SDL_Color black = {0, 0, 0, 255};
const SDL_Color red = {255, 0, 0, 255};

Button* create_button(float x, float y, float w, float h, SDL_Color color, char* text, bool status){
    Button* res = malloc(sizeof(Button));
    if (res == NULL){
        return NULL;
    }
    res->x = x;
    res->y = y;
    res->w = w;
    res->h = h;
    res->color = color;
    res->text = text;
    res->status = status;
    return res;
}

void draw_button(SDL_Renderer* renderer, Button* button){
    SDL_SetRenderDrawColor(renderer, button->color.r, button->color.g, button->color.b, button->color.a);
    SDL_FRect rect;
    rect.x = button->x;
    rect.y = button->y;
    rect.w = button->w;
    rect.h = button->h;
    SDL_RenderFillRect(renderer, &rect);
}

void show_button(Button* button){
    printf("Position: (%f, %f)\nW/H: %f/%f\nColor: (%d, %d, %d)\nText: %s\nStatus: %d"
    , button->x, button->y, button->w, button->h, button->color.r, button->color.g, button->color.b, button->text, button->status);
}

Pawn* create_pawn(float x, float y, float w, float h, SDL_Color color, bool status){
    Pawn* res = malloc(sizeof(Pawn));
    if (res == NULL){
        return NULL;
    }
    res->x = x;
    res->y = y;
    res->w = w;
    res->h = h;
    res->color = color;
    res->status = status;
    return res;
}
