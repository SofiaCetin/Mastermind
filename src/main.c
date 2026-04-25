// Boucle principale du jeu

#include "gui.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 400

typedef enum{
    Menu,
    Game,
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

int main(int argc, char *argv[])
{
    SDL_Cursor* arrow_cursor = NULL;
    SDL_Cursor* hand_cursor = NULL;
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    bool quit = false;

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

    float xcenter = SCREEN_WIDTH / 2;
    float ycenter = SCREEN_HEIGHT /2;

    char* title = "Mastermind";
    SDL_Surface* title_surface = TTF_RenderText_Blended(title_font, title, strlen(title), black);
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

    GameState state = Menu;

    while (!quit){
        SDL_Event event;
        while (SDL_PollEvent(&event)){

            if (event.type == SDL_EVENT_QUIT){
                quit = true;
            }

            if (state == Menu){
                float xmouse;
                float ymouse;
                SDL_GetMouseState(&xmouse, &ymouse);
                if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN){
                    if (mouse_on_btn(xmouse, ymouse, play_btn)){
                        state = Game;
                    }
                    else if(mouse_on_btn(xmouse, ymouse, quit_btn)){
                        quit = true;
                    }
                }
            }

        }
        SDL_SetRenderDrawColor(renderer, white.r, white.g, white.b, white.a);
        SDL_RenderClear(renderer);

        if (state == Menu){
            float xmouse;
            float ymouse;
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
        else if(state == Game){
            SDL_SetCursor(arrow_cursor);
            draw_button(renderer, play_btn);
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(play_btn->text_texture);
    SDL_DestroyTexture(quit_btn->text_texture);
    free(play_btn);
    free(quit_btn);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}