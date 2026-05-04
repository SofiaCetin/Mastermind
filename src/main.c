// Boucle principale du jeu

#include "gui.h"

typedef enum{
    Menu,
    Play,
    Quit
}GameState;

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
            return res;
        }
        current = current->next;
    }
    return NULL;
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

void free_spawners(PawnList* pawns){
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
}

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

    // Création des boutons / objets nécessaires au fenêtres menu, play...

    // Fenêtre du menu principal

    float xcenter = SCREEN_WIDTH / 2;
    float ycenter = SCREEN_HEIGHT /2;

    char* title = "Mastermind";
    SDL_Surface* title_surface = TTF_RenderText_Blended(title_font, title, strlen(title), black.rgb);
    if (title_surface == NULL){
        printf("Erreur de surface: %s", SDL_GetError());
        return 1;
    }
    SDL_Texture* title_texture = SDL_CreateTextureFromSurface(renderer, title_surface);

    float text_w = title_surface->w;
    float text_h = title_surface->h;
    SDL_DestroySurface(title_surface);

    SDL_FRect title_rect = {xcenter - text_w / 2, (ycenter - 100) - text_h / 2, text_w, text_h};

    Button* play_btn = create_button((xcenter - (250 / 2)), (ycenter - (80 /2)) + 50, 250, 70, red, "Jouer", false, button_font, renderer);
    Button* quit_btn = create_button((xcenter - (250 / 2)), (ycenter - (80 /2)) + 150, 250, 70, red, "Quitter", false, button_font, renderer);
    Button* valid_btn = create_button(1280 - 150, 720 - 70, 150, 70, green, "Valider", false, button_font, renderer);

    // Fenêtre du jeu

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

    List2* test_try = malloc(sizeof(List2));
    if (test_try == NULL){
        return 1;
    }

    test_try->next = NULL;

    Element* test_el = malloc(sizeof(Element));
    if (test_el == NULL){
        return 1;
    }
    test_el->color = red;
    test_el->next = NULL;

    append_list2(test_try, test_el);

    // Boucle principale de l'application

    Game* game = init_game(colors);
    game->tries->first = test_try;
    GameState state = Menu;
    bool quit = false;
    Pawn* moving_pawn = NULL;

    while (!quit){
        SDL_Event event;
        float xmouse;
        float ymouse;
        SDL_GetMouseState(&xmouse, &ymouse);

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
                    if (moving_pawn != NULL) {
                        activate_pawns(spawners, true);
                        free(moving_pawn);
                        moving_pawn = NULL;
                    }
                }
                if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN){
                    if (moving_pawn == NULL && check_pawns_state(spawners) == 0){
                        Pawn* moving = mouse_on_spawners(xmouse, ymouse, spawners);
                        if (moving != NULL){
                            moving_pawn = moving;
                            activate_pawns(spawners, false);
                        }
                    }
                    if (mouse_on_btn(xmouse, ymouse, valid_btn)){
                        printf("Valider");
                    }
                }

            }

        }

        // Instructions non dépendantes aux events

        SDL_SetRenderDrawColor(renderer, white.rgb.r, white.rgb.g, white.rgb.b, white.rgb.a);
        SDL_RenderClear(renderer);

        
        if (state == Menu){
            SDL_GetMouseState(&xmouse, &ymouse);
            if (mouse_on_btn(xmouse, ymouse, play_btn) || mouse_on_btn(xmouse, ymouse, quit_btn)){
                SDL_SetCursor(hand_cursor);
            }
            else{
                SDL_SetCursor(arrow_cursor);
            }
            SDL_RenderTexture(renderer, title_texture, NULL, &title_rect);
            draw_button(renderer, play_btn);
            draw_button(renderer, quit_btn);
        }

        else if(state == Play){
            SDL_SetCursor(arrow_cursor);
            draw_gameboard(renderer, game);
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

    SDL_DestroyTexture(play_btn->text_texture);
    SDL_DestroyTexture(quit_btn->text_texture);
    free_spawners(spawners);
    free(play_btn);
    free(quit_btn);
    free(valid_btn);
    free_game(game);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}