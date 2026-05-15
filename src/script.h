#ifndef SCRIPT_H_
#define SCRIPT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <SDL3/SDL.h>

typedef enum RoundState{
    Valid,
    MissingPawns,
    SameColorPawns,
    Invalid
}RoundState;

typedef struct ColorCode{
    char* name;
    SDL_Color rgb;
}ColorCode;

extern const ColorCode white;
extern const ColorCode light_gray;
extern const ColorCode green;
extern const ColorCode blue;
extern const ColorCode yellow;
extern const ColorCode red;
extern const ColorCode pink;
extern const ColorCode gray;
extern const ColorCode orange;
extern const ColorCode black;
extern const ColorCode dark_gray;
extern const ColorCode lightdark_gray;
extern const ColorCode darker_gray;
extern const ColorCode lighter_gray;

typedef struct Element{
    ColorCode color;
    struct Element* next;
}Element;

typedef struct List{
    int length;
    Element* first;
}List;

typedef struct List2{
    Element* first;
    int color_result;
    int pos_result;
    struct List2* next;
}List2;

typedef struct ListOfLists{
    List2* first;
}ListOfLists;

typedef struct Game{
    List* code;
    List* colors;
    ListOfLists* tries;
}Game;

Game* init_game(List* colors);
void add_new_try(Game* game, List2* new_try);
void append(List* colors, ColorCode element);
void show_elements(List* colors);
int el_in_list(List* list, ColorCode element);
ColorCode val_i(List* list, int i);
void append_list2(List2* list, Element* element);
RoundState check_round(Game* game, List2* current_round);
int colors_results(Game* game, List2* current_round);


#endif