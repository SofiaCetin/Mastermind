// Script principal du jeu

#include "gui.h"

typedef enum{
    Menu,
    Play,
    Quit
}GameState;

// Fonctions de jeu auxiliaires

bool mouse_on_btn(float mouse_x, float mouse_y, Button* button){
    if(mouse_x >= button->x && mouse_x <= (button->x + button->w) &&
        mouse_y >= button->y && mouse_y <= (button->y + button->h)){
            return 1;
    }
    else{
        return 0;
    }
}

Pawn* mouse_on_spawners(float mouse_x, float mouse_y, PawnList* pawns){
    if (pawns == NULL || pawns->first == NULL){
        return NULL;
    }

    Pawn* current = pawns->first;
    while (current != NULL){
        if(mouse_x >= current->x && mouse_x <= (current->x + current->w) &&
        mouse_y >= current->y && mouse_y <= (current->y + current->h)){
            Pawn* res = copy_create_pawn(current);
            res->type = Moveable;
            res->activated = true;
            return res;
        }
        current = current->next;
    }
    return NULL;
}

bool mouse_on_current_round_pawns(float mouse_x, float mouse_y, PawnList* current_round, float* pawn_x, float* pawn_y){
    if (current_round == NULL || current_round->first == NULL || pawn_x == NULL || pawn_y == NULL){
        return false;
    }
    Pawn* current = current_round->first;
    while (current != NULL){
        if (mouse_x >= current->x && mouse_x <= (current->x + current->w) &&
        mouse_y >= current->y && mouse_y <= (current->y + current->h)){
            *pawn_x = current->x;
            *pawn_y = current->y;
            return true;
        }
        current = current->next;
    }
    return false;
}

void activate_pawns(PawnList* pawns, bool state){
    if (pawns == NULL || pawns->first == NULL){
        return;
    }
    Pawn* current = pawns->first;
    while (current != NULL){
        current->activated = state;
        current = current->next;
    }
}

int check_pawns_state(PawnList* pawns){
    if (pawns == NULL || pawns->first == NULL){
        return 1;
    }
    Pawn* current = pawns->first;
    while (current != NULL){
        if (current->activated == true){
            return 0;
        }
        current = current->next;
    }
    return 1;
}

void free_text(Text* text){
    if (text == NULL || text->texture == NULL){
        return;
    }
    SDL_DestroyTexture(text->texture);
    free(text);
}

void free_button(Button* button){
    if (button == NULL){
        return;
    }

    if (button->text != NULL){
        free_text(button->text);
    }

    free(button);
}

void free_pawns(PawnList* pawns){
    if (pawns == NULL || pawns->first == NULL){
        return;
    }
    Pawn* current = pawns->first;
    while (current != NULL){
        Pawn* delete = current;
        current = current->next;
        free(delete);
    }
    pawns->first = NULL;
}

void free_pawn_list(PawnList* pawns){
    if (pawns == NULL || pawns->first == NULL){
        return;
    }
    Pawn* current = pawns->first;
    while (current != NULL){
        Pawn* delete = current;
        current = current->next;
        free(delete);
    }
    free(pawns);
}

void free_game(Game* game){
    if (game == NULL){
        return;
    }
    if (game->code != NULL){
        Element* current = game->code->first;
        while (current != NULL){
            Element* delete = current;
            current = current->next;
            free(delete);
        }
        free(game->code);
    }
    
    if (game->colors != NULL){
        Element* current = game->colors->first;
        while (current != NULL){
            Element* delete = current;
            current = current->next;
            free(delete);
        }
        free(game->colors);
    }

    if (game->tries != NULL){
        List2* current = game->tries->first;
        while (current != NULL){
            Element* current_el = current->first;
            while (current_el != NULL){
                Element* delete = current_el;
                current_el = current_el->next;
                free(delete);
            }
            List2* delete = current;
            current = current->next;
            free(delete);
        }
        free(game->tries);
    }
    free(game);
}

// Fonction de jeu principale

int main(int argc, char *argv[])
{

    // Initialisation des objets SDL

    SDL_Cursor* arrow_cursor = NULL;
    SDL_Cursor* hand_cursor = NULL;
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    if(SDL_Init(SDL_INIT_VIDEO) == false){
        fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError());
        return 1;
    }

    TTF_Init();

    window = SDL_CreateWindow("Mastermind", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
    if(window == NULL){
        fprintf(stderr, "Erreur SDL_CreateWindow : %s", SDL_GetError());
        return 1;
    }

    renderer = SDL_CreateRenderer(window, NULL);
    if (renderer == NULL){
        fprintf(stderr, "Erreur SDL_CreateRenderer: %s", SDL_GetError());
        return 1;
    }

    SDL_SetRenderVSync(renderer, 1);

    arrow_cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_DEFAULT);
    if (arrow_cursor == NULL){
        fprintf(stderr, "Erreur SDL_CreateSystemCursor: %s", SDL_GetError());
        return 1;
    }

    hand_cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_POINTER);
    if (hand_cursor == NULL){
        fprintf(stderr, "Erreur SDL_CreateSystemCursor: %s", SDL_GetError());
        return 1;
    }

    TTF_Font* button_font = TTF_OpenFont("../assets/Jersey10-Regular.ttf", 47);
    TTF_Font* title_font = TTF_OpenFont("../assets/Jersey10-Regular.ttf", 70);
    if (button_font == NULL || title_font == NULL){
        printf("Erreur d'initialisation de la font");
        return 1;
    }

    // Initialisation des pointeurs et variables nécessaires a la fenêtre menu

    float xcenter = SCREEN_WIDTH / 2;
    float ycenter = SCREEN_HEIGHT /2;

    Text* main_title = create_text(xcenter, ycenter - 100, "Mastermind", white, title_font, renderer);

    Button* play_btn = create_button((xcenter - (250 / 2)), (ycenter - (80 /2)) + 50, 250, 70, lightdark_gray, "Jouer", false, button_font, renderer);
    Button* quit_btn = create_button((xcenter - (250 / 2)), (ycenter - (80 /2)) + 150, 250, 70, lightdark_gray, "Quitter", false, button_font, renderer);

    // Initialisation des pointeurs et variables nécessaires a la fenêtre jeu

    srand(time(NULL));

    List* colors = malloc(sizeof(List));
    if (colors == NULL){
        return 1;
    }

    colors->first = NULL;
    colors->length = 0;
    append(colors, green);
    append(colors, blue);
    append(colors, red);
    append(colors, yellow);
    append(colors, gray);
    append(colors, pink);
    append(colors, orange);
    append(colors, light_gray);

    PawnList* spawners = gen_spawn_pawns(colors);

    Button* valid_btn = create_button(1280 - 150, 720 - 70, 150, 70, green, "Valider", false, button_font, renderer);

    Game* game = init_game(colors);

    PawnList* current_pawns = malloc(sizeof(PawnList));
    if (current_pawns == NULL){
        return 1;
    }
    current_pawns->first = NULL;

    GameState state = Menu;
    bool quit = false;
    Pawn* moving_pawn = NULL;

    // Boucle principale du jeu

    while (!quit){
        SDL_Event event;
        float xmouse;
        float ymouse;
        SDL_GetMouseState(&xmouse, &ymouse);

        // Boucle des interactions/events à la souris

        while (SDL_PollEvent(&event)){

            if (event.type == SDL_EVENT_QUIT){
                quit = true;
            }

            if (state == Menu){
                if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN){
                    if (mouse_on_btn(xmouse, ymouse, play_btn)){
                        state = Play;
                    }
                    else if(mouse_on_btn(xmouse, ymouse, quit_btn)){
                        quit = true;
                    }
                }
            }

            if (state == Play){
                if (event.type == SDL_EVENT_MOUSE_MOTION) {
                    if (moving_pawn) {
                        moving_pawn->x += event.motion.xrel;
                        moving_pawn->y += event.motion.yrel;
                    }
                }

                if (event.type == SDL_EVENT_MOUSE_BUTTON_UP) {
                    if (moving_pawn != NULL && moving_pawn->type == Moveable) {
                        float pos_x, pos_y;
                        if (mouse_on_current_round_pawns(xmouse, ymouse, current_pawns, &pos_x, &pos_y)){
                            modify_current_pawns(current_pawns, pos_x, pos_y, moving_pawn);
                        }
                        activate_pawns(spawners, true);
                        free(moving_pawn);
                        moving_pawn = NULL;
                    }
                }

                if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN){
                    if (moving_pawn == NULL && check_pawns_state(spawners) == 0){
                        float pos_x, pos_y;
                        Pawn* moving = mouse_on_spawners(xmouse, ymouse, spawners);
                        if (moving != NULL){
                            moving_pawn = moving;
                            activate_pawns(spawners, false);
                        }
                        else if (mouse_on_current_round_pawns(xmouse, ymouse, current_pawns, &pos_x, &pos_y)){
                            Pawn* moving = pawn_click(current_pawns, pos_x, pos_y);
                            if (moving != NULL){
                                moving_pawn = moving;
                                activate_pawns(spawners, false); 
                            }
                        }
                    }
                    if (mouse_on_btn(xmouse, ymouse, valid_btn)){
                        List2* converted_round = convert_pawnlist_to_list(current_pawns);
                        RoundState result = check_round(game, converted_round);
                        if (result == Invalid){
                            return 1;
                        }
                        else if (result == MissingPawns){
                            printf("Toutes les cases ne sont pas remplies");
                        }
                        else if (result == SameColorPawns){
                            printf("Il ne peut pas y avoir deux pions de la même couleur");
                        }
                        else if (result == Valid){
                            add_new_try(game, converted_round);
                            free_pawns(current_pawns);
                        }
                    }
                }

            }

        }

        // Instructions pour générer le GUI

        SDL_SetRenderDrawColor(renderer, dark_gray.rgb.r, dark_gray.rgb.g, dark_gray.rgb.b, dark_gray.rgb.a);
        SDL_RenderClear(renderer);

        
        if (state == Menu){
            SDL_GetMouseState(&xmouse, &ymouse);
            if (mouse_on_btn(xmouse, ymouse, play_btn) || mouse_on_btn(xmouse, ymouse, quit_btn)){
                SDL_SetCursor(hand_cursor);
            }
            else{
                SDL_SetCursor(arrow_cursor);
            }
            draw_text(renderer, main_title);
            draw_button(renderer, play_btn);
            draw_button(renderer, quit_btn);
        }

        else if(state == Play){
            SDL_SetCursor(arrow_cursor);
            draw_gameboard(renderer, game, current_pawns);
            draw_pawn_list(renderer, spawners);
            if (moving_pawn != NULL){
                draw_pawn(renderer, moving_pawn);
            }
            SDL_GetMouseState(&xmouse, &ymouse);
            if (mouse_on_btn(xmouse, ymouse, valid_btn)){
                SDL_SetCursor(hand_cursor);
            }
            else{
                SDL_SetCursor(arrow_cursor);
            }
            draw_button(renderer, valid_btn);
        }
    
        SDL_RenderPresent(renderer);
    }

    // Libération de la mémoire

    free_text(main_title);
    free_pawn_list(spawners);
    free_button(play_btn);
    free_button(quit_btn);
    free_button(valid_btn);
    free_game(game);
    free_pawn_list(current_pawns);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}