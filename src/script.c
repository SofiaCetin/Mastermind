// Script de base du jeu

#include "script.h"

const ColorCode white = {White, {255, 255, 255, 255}};
const ColorCode green = {Green, {0, 255, 0, 255}};
const ColorCode blue = {Blue, {0, 0, 255, 255}};
const ColorCode yellow = {Yellow, {255, 255, 0, 255}};
const ColorCode red = {Red, {255, 0, 0, 255}};
const ColorCode pink = {Pink, {255, 0, 165, 255}};
const ColorCode gray = {Gray, {187, 187, 187, 255}};
const ColorCode orange = {Orange, {255, 128, 0, 255}};
const ColorCode black = {Black, {0, 0, 0, 255}};

Game* create_game(List* code){
    Game* game = malloc(sizeof(Game));
    game->code = code;
    game->tries = NULL;
    return game;
}

void add_new_try(Game* game, List2* new_try){
    if (game == NULL || new_try == NULL || new_try->value == NULL || new_try->value->first == NULL){
        return;
    }
    if (game->tries == NULL){
        game->tries->first = new_try;
    }

    new_try->previous = game->tries->last;
    new_try->next = game->tries->first;
    game->tries->first->previous = new_try;
    game->tries->last->next = new_try;
    game->tries->last = new_try;

}