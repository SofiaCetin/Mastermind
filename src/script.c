// Script fonctionnel du jeu

#include "script.h"

const ColorCode white = {"White", {255, 255, 255, 255}};
const ColorCode light_gray = {"Light-Gray", {230, 230, 230, 255}};
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
    if (new == NULL){
        return;
    }
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
        if (strcmp(current->color.name, element.name) == 0){
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
    printf("] \n");
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
    code->first = NULL;
    code->length = 0;

    ListOfLists* list_tries = malloc(sizeof(ListOfLists));
    if (list_tries == NULL){
        return NULL;
    }
    list_tries->first = NULL;

    Element* first = malloc(sizeof(Element));
    if (first == NULL){
        return NULL;
    }

    first->next = NULL;

    List* ignore_color = malloc(sizeof(List));
    if (ignore_color == NULL){
        return NULL;
    }
    ignore_color->first = NULL;
    ignore_color->length = 0;

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
        next->next = NULL;

        i_range = rand() % colors->length;
        next->color = val_i(colors, i_range);
        while (el_in_list(ignore_color, next->color) != 0){
            i_range = rand() % colors->length;
            next->color = val_i(colors, i_range);
        }
        append(ignore_color, next->color);
        current->next = next;
        current = next;
    }

    res->code = code;
    res->colors = colors;
    res->tries = list_tries;
    res->tries->first = NULL;

    Element* current2 = ignore_color->first;
    while (current2 != NULL){
        Element* delete = current2;
        current2 = current2->next;
        free(delete);
    }

    free(ignore_color);

    return res;
}

void add_new_try(Game* game, List2* new_try){
    if (game == NULL || new_try == NULL || new_try->first == NULL){
        return;
    }
    new_try->next = NULL;

    if (game->tries->first == NULL){
        game->tries->first = new_try;
        return;
    }
    List2* current = game->tries->first;
    while (current->next != NULL){
        current = current->next;
    }
    current->next = new_try;
}

void append_list2(List2* list, Element* element){
    if (list == NULL){
        return;
    }
    element->next = NULL;

    if (list->first == NULL){
        list->first = element;
        return;
    }
    Element* current = list->first;
    while (current->next != NULL){
        current = current->next;
    }
    current->next = element;
}

RoundState check_round(Game* game, List2* current_round){
    if (game == NULL || game->code == NULL || game->code->first == NULL || current_round == NULL || current_round->first == NULL){
        return Invalid;
    }

    List* check_doubles = malloc(sizeof(List));
    if (check_doubles == NULL){
        free(check_doubles);
        return Invalid;
    }
    check_doubles->first = NULL;

    Element* current = current_round->first;
    while (current != NULL){
        if (strcmp(current->color.name, lighter_gray.name) == 0){
            free(check_doubles);
            return MissingPawns;
        }
        if (el_in_list(check_doubles, current->color)){
            free(check_doubles);
            return SameColorPawns;
        }
        append(check_doubles, current->color);
        current = current->next;

    }

    Element* color_code = game->code->first;
    Element* color_round = current_round->first;
    while (color_code != NULL){
        if (strcmp(color_code->color.name, color_round->color.name) != 0){
            free(check_doubles);
            return Valid;
        }
        color_code = color_code->next;
        color_round = color_round->next;
    }
    free(check_doubles);
    return Win;
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

int pos_results(Game* game, List2* current_round){
    if (game == NULL || game->code == NULL || game->code->first == NULL || current_round == NULL || current_round->first == NULL){
        return 0;
    }
    int res = 0;
    Element* round_current = current_round->first;
    Element* game_current = game->code->first;
    while (round_current != NULL && game_current != NULL){

        if (strcmp(round_current->color.name, game_current->color.name) == 0){
            res++;
        }
        round_current = round_current->next;
        game_current = game_current->next;
    }
    return res;
}

int len_listoflists(ListOfLists* list){
    if (list == NULL || list->first == NULL){
        return 0;
    }

    int res = 0;
    List2* current = list->first;
    while (current != NULL){
        res++;
        current = current->next;
    }
    return res;
}