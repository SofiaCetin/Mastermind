#ifndef SCRIPT_H_
#define SCRIPT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL3/SDL.h>

typedef enum Color{
    White,
    Green,
    Blue,
    Yellow,
    Red,
    Pink,
    Gray,
    Orange,
    Black
}Color;

typedef struct ColorCode{
    Color color;
    SDL_Color rgb;
}ColorCode;

extern const ColorCode white;
extern const ColorCode green;
extern const ColorCode blue;
extern const ColorCode yellow;
extern const ColorCode red;
extern const ColorCode pink;
extern const ColorCode gray;
extern const ColorCode orange;
extern const ColorCode black;

typedef struct Element{
    int value;
    struct Element* next;
}Element;

typedef struct List{
    Element* first;
}List;

typedef struct List2{
    List* value;
    struct List2* next;
    struct List2* previous;
}List2;

typedef struct ListOfLists{
    List2* first;
    List2* last;
}ListOfLists;

typedef struct Game{
    List* code;
    ListOfLists* tries;
}Game;

void add_new_try(Game* game, List2* new_try);

#endif