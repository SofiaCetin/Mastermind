// Script de base du jeu

#include "script.h"

const ColorCode white = {"White", {255, 255, 255, 255}};
const ColorCode light_gray = {"Light Gray", {230, 230, 230, 255}};
const ColorCode green = {"Green", {0, 255, 0, 255}};
const ColorCode blue = {"Blue", {0, 0, 255, 255}};
const ColorCode yellow = {"Yellow", {255, 255, 0, 255}};
const ColorCode red = {"Red", {255, 0, 0, 255}};
const ColorCode pink = {"Pink", {255, 0, 165, 255}};
const ColorCode gray = {"Gray", {170, 170, 170, 255}};
const ColorCode orange = {"Orange", {255, 128, 0, 255}};
const ColorCode black = {"Black", {0, 0, 0, 255}};
const ColorCode dark_gray = {"Dark Gray", {30, 30, 30, 255}};
const ColorCode lightdark_gray = {"Light Dark Gray", {50, 50, 50, 255}};

void append(List* colors, ColorCode element){
    if (colors == NULL){
        return;
    }
    Element* new = malloc(sizeof(Element));
    new->color = element;
    new->next = NULL;
    if (colors->first == NULL){
        colors->first = new;
    } else{
        Element* actuel = colors->first;
        while (actuel->next != NULL){
            actuel = actuel->next;
        }
        actuel->next = new;
    }
    colors->length += 1;
}

int el_in_list(List* list, ColorCode element){
    if (list == NULL || list->first == NULL){
        return 1;
    }
    Element* current = list->first;
    while (current != NULL){
        if (current->color.name == element.name){
            return 0;
        }
        current = current->next;
    }
    return 1;
}

void show_elements(List* colors){
    if (colors == NULL || colors->first == NULL){
        return;
    }
    Element* actuel = colors->first;
    printf("[");
    while (actuel != NULL){
        printf("%s", actuel->color.name);
        actuel = actuel->next;
        printf(" ");
    }
    printf("]");
}

ColorCode val_i(List* list, int i){
    if (i >= list->length || i < 0 || list == NULL || list->first == NULL){
        exit(1);
    }
    Element* current = list->first;
    while (i != 0){
        current = current->next;
        i--;
    }
    return current->color;
}

Game* init_game(List* colors){
    Game* res = malloc(sizeof(Game));
    if (res == NULL){
        return NULL;
    }

    List* code = malloc(sizeof(List));
    if (code == NULL){
        return NULL;
    }

    Element* premier = malloc(sizeof(Element));
    if (premier == NULL){
        return NULL;
    }

    List* ignore_color = malloc(sizeof(List));
    if (ignore_color == NULL){
        return NULL;
    }

    int i_range = rand() % colors->length;
    premier->color = val_i(colors, i_range);
    code->first = premier;
    append(ignore_color, premier->color);
    Element* current = premier;
    for (int i = 0; i < 3; i++){
        
        Element* next = malloc(sizeof(Element));
        if (next == NULL){
            return NULL;
        }

        i_range = rand() % colors->length;
        next->color = val_i(colors, i_range);
        while (el_in_list(ignore_color, next->color) == 0){
            i_range = rand() % colors->length;
            next->color = val_i(colors, i_range);
        }
        append(ignore_color, next->color);
        current->next = next;
        current = next;
    }

    res->code = code;
    res->colors = colors;
    ListOfLists* list_tries = malloc(sizeof(ListOfLists));
    res->tries = list_tries;
    res->tries->first = NULL;
    res->tries->next = NULL;
    return res;
}

void add_new_try(Game* game, List2* new_try){
    if (game == NULL || new_try == NULL || new_try->first == NULL){
        return;
    }
    if (game->tries == NULL){
        game->tries->first = new_try;
    }

    new_try->next = game->tries->first;

}

void append_list2(List2* list, Element* element){
    if (list == NULL){
        return;
    }
    if (list->first == NULL){
        list->first = element;
        element->next = NULL;
        return;
    }
    Element* current = list->first;
    while (current->next != NULL){
        current = current->next;
    }
    current->next = element;
    element->next = NULL;
}

/*
int main(void){

    srand(time(NULL));

    List* colors = malloc(sizeof(List));
    colors->first = NULL;
    colors->length = 0;
    append(colors, white);
    append(colors, green);
    append(colors, blue);
    append(colors, red);
    append(colors, yellow);
    append(colors, black);
    append(colors, gray);
    append(colors, pink);
    append(colors, orange);
    show_elements(colors);
    Game* game = init_game(colors);
    show_elements(game->code);

}
*/
