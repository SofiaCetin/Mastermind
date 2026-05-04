#include "gui.h"

Button* create_button(float x, float y, float w, float h, ColorCode color, char* text, bool status, TTF_Font* font, SDL_Renderer* renderer){
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

    SDL_Surface* text_surface = TTF_RenderText_Blended(font, text, strlen(text), white.rgb);
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

    SDL_SetRenderDrawColor(renderer, button->color.rgb.r, button->color.rgb.g, button->color.rgb.b, button->color.rgb.a);

    SDL_RenderFillRect(renderer, &rect);

    SDL_FRect text_rect = {
        button->x + (button->w - button->text_w) / 2,
        button->y + (button->h - button->text_h) / 2,
        button->text_w,
        button->text_h,
    };

    SDL_RenderTexture(renderer, button->text_texture, NULL, &text_rect);
    
}

void draw_pawn(SDL_Renderer* renderer,Pawn* pawn){

    SDL_FRect rect = {pawn->x, pawn->y, pawn->w, pawn->h};

    SDL_SetRenderDrawColor(renderer, pawn->color.rgb.r, pawn->color.rgb.g, pawn->color.rgb.b, pawn->color.rgb.a);

    SDL_RenderFillRect(renderer, &rect);

}

Pawn* create_pawn(float x, float y, float w, float h, ColorCode color, bool activated, PawnType type){
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
    res->type = type;
    return res;
}

Pawn* copy_create_pawn(Pawn* pawn){
    Pawn* res = malloc(sizeof(Pawn));
    if (res == NULL){
        return NULL;
    }
    res->x = pawn->x;
    res->y = pawn->y;
    res->w = pawn->w;
    res->h = pawn->h;
    res->color = pawn->color;
    res->activated = pawn->activated;
    res->type = pawn->type;
    return res;
}

void append_pawn(PawnList* list, Pawn* element){
    if (list == NULL){
        return;
    }

    if (element == NULL){
        return;
    }

    if (list->first == NULL){
        list->first = element;
    } else{
        Pawn* current = list->first;
        while (current->next != NULL){
            current = current->next;
        }
        current->next = element;
        element->next = NULL;
    }
}

PawnList* gen_spawn_pawns(List* colors){
    if (colors == NULL || colors->first == NULL){
        return NULL;
    }

    PawnList* res = malloc(sizeof(PawnList));
    float x = 10;
    float y = 10;
    Element* current = colors->first;
    while (current != NULL){
        Pawn* new = create_pawn(x, y, 40, 40, current->color, true, Spawner);
        append_pawn(res, new);
        current = current->next;
        x += 60;
        if(x > 70){
            x = 10;
            y += 60;
        }
    }

    return res; 

}

void draw_pawn_list(SDL_Renderer* renderer, PawnList* list){
    if (list == NULL || list->first == NULL){
        return;
    }
    Pawn* current = list->first;
    while (current != NULL){

        SDL_FRect rect = {current->x, current->y, current->w, current->h};

        SDL_SetRenderDrawColor(renderer, current->color.rgb.r, current->color.rgb.g, current->color.rgb.b, current->color.rgb.a);

        SDL_RenderFillRect(renderer, &rect);

        current = current->next;
    }
}
