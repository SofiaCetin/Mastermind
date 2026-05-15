// Script contenant des fonctions pour créer le GUI sans surcharger le main

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
    res->status = status;

    Text* button_text = create_text(x + w / 2, y + h / 2, text, white, font, renderer);
    if (button_text == NULL){
        return NULL;
    }
    res->text = button_text;

    return res;
}

Text* create_text(float x, float y, char* text, ColorCode color, TTF_Font* font, SDL_Renderer* renderer){
    Text* res = malloc(sizeof(Text));
    if (res == NULL){
        return NULL;
    }
    res->x = x;
    res->y = y;
    res->text = text;
    res->color = color;

    SDL_Surface* text_surface = TTF_RenderText_Blended(font, text, strlen(text), color.rgb);
    if (text_surface == NULL){
        printf("Erreur de surface: %s", SDL_GetError());
        return NULL;
    }

    res->texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    res->w = text_surface->w;
    res->h = text_surface->h;

    SDL_DestroySurface(text_surface);

    res->next = NULL;

    return res;

}

void draw_text(SDL_Renderer* renderer, Text* text){
    if (text == NULL || text->texture == NULL){
        return;
    }

    SDL_FRect text_rect = {text->x - text->w / 2, text->y - text->h /2, text->w, text->h};

    SDL_RenderTexture(renderer, text->texture, NULL, &text_rect);
}

void draw_button(SDL_Renderer* renderer, Button* button){

    SDL_FRect rect = {button->x, button->y, button->w, button->h};

    SDL_SetRenderDrawColor(renderer, button->color.rgb.r, button->color.rgb.g, button->color.rgb.b, button->color.rgb.a);

    SDL_RenderFillRect(renderer, &rect);

    if (button->text != NULL){
        draw_text(renderer, button->text);
    }
    
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
    res->next = NULL;
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
    res->next = NULL;
    return res;
}

void append_pawn(PawnList* list, Pawn* element){
    if (list == NULL){
        return;
    }

    if (element == NULL){
        return;
    }

    element->next = NULL;

    if (list->first == NULL){
        list->first = element;
    } else{
        Pawn* current = list->first;
        while (current->next != NULL){
            current = current->next;
        }
        current->next = element;
    }
}

PawnList* gen_spawn_pawns(List* colors){
    if (colors == NULL || colors->first == NULL){
        return NULL;
    }

    PawnList* res = malloc(sizeof(PawnList));
    if (res == NULL){
        return NULL;
    }
    res->first = NULL;
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

void draw_gameboard(SDL_Renderer* renderer, Game* game, PawnList* current_pawns, TextList* colors_valid, TextList* pos_valid, TTF_Font* font){
    float rect_x = (SCREEN_WIDTH / 2) - 150;
    float rect_y = 0;
    float rect_w = 300;
    float rect_h = SCREEN_HEIGHT;
    SDL_FRect rect = {rect_x, rect_y, rect_w, rect_h};

    SDL_SetRenderDrawColor(renderer, darker_gray.rgb.r, darker_gray.rgb.g, darker_gray.rgb.b, darker_gray.rgb.a);

    SDL_RenderFillRect(renderer, &rect);

    if (game == NULL || game->code == NULL || game->colors == NULL || game->tries == NULL || current_pawns == NULL){
        return;
    }

    float try_h = rect_h / 9;
    float pawns_w = 40;
    float pawns_h = 40;
    float pawns_separation = (rect_w - (4*pawns_w)) / 5;
    float pawns_x = rect_x + pawns_separation;
    rect_y = SCREEN_HEIGHT - try_h;
    float pawns_y = rect_y + ((try_h / 2) - 20);

    if (game->tries->first != NULL){
        List2* current = game->tries->first;
        while (current != NULL){
            SDL_FRect rect_2 = {rect_x, rect_y, rect_w, try_h};

            SDL_SetRenderDrawColor(renderer, lightdark_gray.rgb.r, lightdark_gray.rgb.g, lightdark_gray.rgb.b, lightdark_gray.rgb.a);

            SDL_RenderFillRect(renderer, &rect_2);

            gen_numbers(renderer, colors_valid, font, current->color_result, rect_x - 20, rect_y + (try_h / 2), Left);
            gen_numbers(renderer, pos_valid, font, current->pos_result, rect_x + rect_w + 20, rect_y + (try_h / 2), Right);

            Element* current_el = current->first;
            while (current_el != NULL){
                Pawn new;
                new.x = pawns_x;
                new.y = pawns_y;
                new.w = pawns_w;
                new.h = pawns_h;
                new.color = current_el->color;

                draw_pawn(renderer, &new);
                pawns_x += pawns_separation + pawns_w;
                current_el = current_el->next;      
            }
            rect_y -= try_h;
            pawns_y = rect_y + ((try_h / 2) - 20);
            current = current->next;
            pawns_x = rect_x + pawns_separation;
        }
    }

    SDL_FRect rect_current = {rect_x, rect_y, rect_w, try_h};

    SDL_SetRenderDrawColor(renderer, lightdark_gray.rgb.r, lightdark_gray.rgb.g, lightdark_gray.rgb.b, lightdark_gray.rgb.a);

    SDL_RenderFillRect(renderer, &rect_current);

    if (current_pawns->first == NULL){
        for (int i = 0; i < 4; i++){
            Pawn* new = create_pawn(pawns_x, pawns_y, pawns_w, pawns_h, lighter_gray, false, Idle);
            append_pawn(current_pawns, new);
            pawns_x += pawns_separation + pawns_w;
        }
    }

    Pawn* current = current_pawns->first;
    while (current != NULL){
        draw_pawn(renderer, current);
        current = current->next;
    }
}

void modify_current_pawns(PawnList* current_pawns, float x, float y, Pawn* pawn){
    if (current_pawns == NULL || current_pawns->first == NULL || pawn == NULL){
        return;
    }
    Pawn* current = current_pawns->first;
    while (current != NULL){
        if (current->x == x && current->y == y){
            if (strcmp(current->color.name, lighter_gray.name) == 0){
                current->color = pawn->color;
            }
        }
        current = current->next;
    }
}

Pawn* pawn_click(PawnList* current_pawns, float x, float y){
    if (current_pawns == NULL || current_pawns->first == NULL){
        return NULL;
    }

    Pawn* current = current_pawns->first;
    while (current != NULL){
        if (current->x == x && current->y == y){
            if(strcmp(current->color.name,lighter_gray.name) != 0){
                Pawn* res = copy_create_pawn(current);
                res->type = Moveable;
                current->color = lighter_gray;
                return res;
            }
        }
        current = current->next;
    }
    return NULL;
}

List2* convert_pawnlist_to_list(PawnList* pawns){
    if (pawns == NULL || pawns->first == NULL){
        return NULL;
    }

    List2* res = malloc(sizeof(List2));
    if (res == NULL){
        return NULL;
    }
    res->first = NULL;
    res->next = NULL;

    Pawn* current = pawns->first;
    while (current != NULL){
        Element* element = malloc(sizeof(Element));
        if (element == NULL){
            return NULL;
        }
        element->color = current->color;
        element->next = NULL;
        append_list2(res, element);
        current = current->next;
    }

    return res;
}

void append_textlist(TextList* list, Text* text){
    if (list == NULL || text == NULL){
        return;
    }

    text->next = NULL;

    if (list->first == NULL){
        list->first = text;
        return;
    }

    Text* current = list->first;
    while (current->next != NULL){
        current = current->next;
    }
    current->next = text;
}

void gen_numbers(SDL_Renderer* renderer, TextList* text_list, TTF_Font* font, int number, float starting_x, float starting_y, NumberSide side){
    if (text_list == NULL ||  number > 4 || number <= 0){
        return;
    }

    char text_number[] = "1";

    int text_width = 20;
    int text_separation = 10;

    int total_width = (number * text_width) + ((number - 1) * text_separation);

    if (side == Left){
        starting_x -= total_width;
    }
    else if (side == Right){
        starting_x += text_separation;
    }

    for (int i = 0; i < number; i++){
        Text* text = create_text(starting_x, starting_y, text_number, white, font, renderer);
        if (text == NULL){
            return;
        }
        append_textlist(text_list, text);
        draw_text(renderer, text);
        text_number[0]++;
        starting_x += 20 + text_separation;
    }
}

void draw_result(SDL_Renderer* renderer, List* code, float starting_x, float starting_y){

    int pawn_w = 40;
    int pawn_h = 40;
    int pawn_separation = pawn_w;
    starting_x -= (pawn_separation / 2) + (pawn_w * 2) + pawn_separation;

    Element* current = code->first;
    for (int i = 0; i < 4; i++){
        if (current != NULL){
            SDL_FRect rect = {starting_x, starting_y, pawn_w, pawn_h};

            SDL_SetRenderDrawColor(renderer, current->color.rgb.r, current->color.rgb.g, current->color.rgb.b, current->color.rgb.a);

            SDL_RenderFillRect(renderer, &rect);

            current = current->next;
            starting_x += pawn_separation + pawn_w;
        }
    }

}