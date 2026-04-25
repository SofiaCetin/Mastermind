#include "gui.h"

const SDL_Color white = {255, 255, 255, 255};
const SDL_Color black = {0, 0, 0, 255};
const SDL_Color red = {255, 0, 0, 255};

Button* create_button(float x, float y, float w, float h, SDL_Color color, char* text, bool status, TTF_Font* font, SDL_Renderer* renderer){
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

    SDL_Surface* text_surface = TTF_RenderText_Blended(font, text, strlen(text), white);
    if (text_surface == NULL){
        printf("Erreur de surface: %s", SDL_GetError());
        return NULL;
    }

    res->text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    res->text_w = text_surface->w;
    res->text_h = text_surface->h;

    SDL_DestroySurface(text_surface);

    return res;
}

void draw_button(SDL_Renderer* renderer, Button* button){

    SDL_FRect rect = {button->x, button->y, button->w, button->h};

    SDL_SetRenderDrawColor(renderer, button->color.r, button->color.g, button->color.b, button->color.a);

    SDL_RenderFillRect(renderer, &rect);

    SDL_FRect text_rect = {
        button->x + (button->w - button->text_w) / 2,
        button->y + (button->h - button->text_h) / 2,
        button->text_w,
        button->text_h,
    };

    SDL_RenderTexture(renderer, button->text_texture, NULL, &text_rect);
    
}

void show_button(Button* button){
    printf("Position: (%f, %f)\nW/H: %f/%f\nColor: (%d, %d, %d)\nText: %s\nStatus: %d"
    , button->x, button->y, button->w, button->h, button->color.r, button->color.g, button->color.b, button->text, button->status);
}

Pawn* create_pawn(float x, float y, float w, float h, SDL_Color color, bool activated, PawnType type){
    Pawn* res = malloc(sizeof(Pawn));
    if (res == NULL){
        return NULL;
    }
    res->x = x;
    res->y = y;
    res->w = w;
    res->h = h;
    res->color = color;
    res->activated = activated;
    return res;
}
