#ifndef GUI_H_
#define GUI_H_

#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "script.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

typedef enum{
    Spawner,
    Moveable,
    Idle
}PawnType;

typedef struct Button{
    float x;
    float y;
    float w;
    float h;
    ColorCode color;
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
    ColorCode color;
    bool activated;
    PawnType type;
    struct Pawn* next;
}Pawn;

typedef struct PawnList{
    Pawn* first;
}PawnList;


Button* create_button(float x, float y, float w, float h, ColorCode color, char* text, bool status, TTF_Font* font, SDL_Renderer* renderer);
void draw_button(SDL_Renderer* renderer, Button* button);
Pawn* create_pawn(float x, float y, float w, float h, ColorCode color, bool activated, PawnType type);
Pawn* copy_create_pawn(Pawn* pawn);
void draw_pawn(SDL_Renderer* renderer, Pawn* pawn);
void append_pawn(PawnList* list, Pawn* element);
PawnList* gen_spawn_pawns(List* colors);
void draw_pawn_list(SDL_Renderer* renderer, PawnList* list);
void draw_gameboard(SDL_Renderer* renderer, Game* game, PawnList* current_pawns);
void modify_current_pawns(PawnList* current_pawns, float x, float y, Pawn* pawn);
Pawn* pawn_click(PawnList* current_pawns, float x, float y);
List2* convert_pawnlist_to_list(PawnList* pawns);

#endif