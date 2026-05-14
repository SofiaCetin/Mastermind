// Script fonctionnel du jeu

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
const ColorCode lighter_gray = {"Lighter Gray", {80, 80, 80, 255}};
const ColorCode darker_gray = {"Darker Gray", {25, 25, 25, 255}};

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
        return 0;
    }
    Element* current = list->first;
    while (current != NULL){
        if (current->color.name == element.name){
            return 1;
        }
        current = current->next;
    }
    return 0;
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

    List* current_round = malloc(sizeof(List));
    if (current_round == NULL){
        return NULL;
    }

    ListOfLists* list_tries = malloc(sizeof(ListOfLists));
    if (list_tries == NULL){
        return NULL;
    }

    Element* first = malloc(sizeof(Element));
    if (first == NULL){
        return NULL;
    }

    List* ignore_color = malloc(sizeof(List));
    if (ignore_color == NULL){
        return NULL;
    }

    int i_range = rand() % colors->length;
    first->color = val_i(colors, i_range);
    code->first = first;
    append(ignore_color, first->color);
    Element* current = first;
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

    for (int i = 0; i < 4; i++){
        append(current_round, white);
    }

    res->code = code;
    res->colors = colors;
    res->tries = list_tries;
    res->tries->first = NULL;
    res->tries->next = NULL;
    return res;
}

void add_new_try(Game* game, List2* new_try){
    if (game == NULL || new_try == NULL || new_try->first == NULL){
        return;
    }
    if (game->tries->first == NULL){
        game->tries->first = new_try;
        return;
    }
    List2* current = game->tries->first;
    while (current->next != NULL){
        current = current->next;
    }
    current->next = new_try;
    new_try->next = NULL;

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

RoundState check_round(Game* game, List2* current_round){
    if (game == NULL || game->code == NULL || game->code->first == NULL || current_round == NULL || current_round->first == NULL){
        return Invalid;
    }

    List* check_doubles = malloc(sizeof(List));
    if (check_doubles == NULL){
        return Invalid;
    }
    check_doubles->first = NULL;

    Element* current = current_round->first;
    while (current != NULL){
        if (current->color.name == lighter_gray.name){
            return MissingPawns;
        }
        if (el_in_list(check_doubles, current->color)){
            return SameColorPawns;
        }
        append(check_doubles, current->color);
        current = current->next;

    }
    return Valid;
}

int colors_results(Game* game, List2* current_round){
    if (game == NULL || game->code == NULL || game->code->first == NULL || current_round == NULL || current_round->first == NULL){
        return 0;
    }
    int res = 0;
    Element* current = current_round->first;
    while (current != NULL){
        if (el_in_list(game->code, current->color)){
            res++;
        }
        current = current->next;
    }
    return res;
}

/*
int pos_results(Game* game, List2* current_round){
    if (game == NULL || game->code == NULL || game->code->first == NULL || current_round == NULL || current_round->first == NULL){
        return 0;
    }
    
}
*/

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
