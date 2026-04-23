// Boucle principale du jeu

#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "gui.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 400

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

    float xcenter = SCREEN_WIDTH / 2;
    float ycenter = SCREEN_HEIGHT /2;

    Button* play_btn = create_button((xcenter - (250 / 2)), (ycenter - (80 /2)) + 50, 250, 80, red, "Jouer", false);
    Button* quit_btn = create_button((xcenter - (250 / 2)), (ycenter - (80 /2)) + 150, 250, 80, red, "Quitter", false);

    while (!quit){
        SDL_Event event;
        while (SDL_PollEvent(&event)){
            float xmouse;
            float ymouse;
            SDL_GetMouseState(&xmouse, &ymouse);
            if (mouse_on_btn(xmouse, ymouse, play_btn) || mouse_on_btn(xmouse, ymouse, quit_btn)){
                SDL_SetCursor(hand_cursor);
            }
            else{
                SDL_SetCursor(arrow_cursor);
            }

            if (event.type == SDL_EVENT_QUIT){
                quit = true;
            }

            if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN){
                if (mouse_on_btn(xmouse, ymouse, play_btn)){
                    printf("Play button clicked !");
                }
                else if(mouse_on_btn(xmouse, ymouse, quit_btn)){
                    printf("Quit button clicked !");
                }
            }
        }
        SDL_SetRenderDrawColor(renderer, white.r, white.g, white.b, white.a);
        SDL_RenderClear(renderer);
        draw_button(renderer, play_btn);
        draw_button(renderer, quit_btn);
        SDL_RenderPresent(renderer);
    }

    free(play_btn);
    free(quit_btn);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}