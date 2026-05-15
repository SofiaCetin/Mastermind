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

typedef enum PawnType{
    Spawner,
    Moveable,
    Idle
}PawnType;

typedef enum NumberSide{
    Left,
    Right
}NumberSide;

typedef struct Text{
    float x;
    float y;
    float w;
    float h;
    struct Text* next;
    char* text;
    ColorCode color;
    SDL_Texture* texture;
}Text;

typedef struct TextList{
    Text* first;
}TextList;

typedef struct Button{
    float x;
    float y;
    float w;
    float h;
    ColorCode color;
    bool status;
    Text* text;

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
void draw_gameboard(SDL_Renderer* renderer, Game* game, PawnList* current_pawns, TextList* colors_valid, TextList* pos_valid, TTF_Font* font);
void modify_current_pawns(PawnList* current_pawns, float x, float y, Pawn* pawn);
Pawn* pawn_click(PawnList* current_pawns, float x, float y);
List2* convert_pawnlist_to_list(PawnList* pawns);
Text* create_text(float x, float y, char* text, ColorCode color, TTF_Font* font, SDL_Renderer* renderer);
void draw_text(SDL_Renderer* renderer, Text* text);
void gen_numbers(SDL_Renderer* renderer, TextList* text_list, TTF_Font* font, int number, float starting_x, float starting_y, NumberSide side);
void append_textlist(TextList* list, Text* text);
void draw_result(SDL_Renderer* renderer, List* code, float starting_x, float starting_y);

#endif