#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <time.h>
#include <stdlib.h>

// constant declarations
#define WIDTH 1000
#define HEIGHT 800
#define CELL_SIZE 50
#define ROW HEIGHT / CELL_SIZE
#define COLUMN WIDTH / CELL_SIZE
#define ROW_LIM ROW - 2
#define COL_LIM COLUMN - 2
#define MAX_LENGTH 120

struct snake
{
    int xcoord;
    int ycoord;
};

// macros
#define GRID draw_grid(renderer)
#define CELL(x, y, R, G, B) fill_cell(renderer, x, y, R, G, B)
#define X_RNG (rng(COL_LIM, 2) * CELL_SIZE)
#define Y_RNG (rng(ROW_LIM, 2) * CELL_SIZE)
#define FOOD draw_food(renderer, pfood, px_rand, py_rand, blocks, psnake_length)
#define MOVEMENT movement(renderer, blocks, psnake_length, px_temp, py_temp, px_temp1, py_temp1, pbutton)
#define SCOREBOARD draw_scoreboard_ui(renderer, score, psnake_length, font, white, texture, surface)
#define END_SCREEN game_over_screen(renderer, final_score, psnake_length)

void draw_grid(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_FRect rect = {0, 0, WIDTH, 2};
    for (int i = 0; i < ROW; i++)
    {
        SDL_RenderFillRect(renderer, &rect);
        rect.y = rect.y + CELL_SIZE;
    }
    SDL_FRect rect1 = {0, 0, 2, HEIGHT};
    for (int i = 0; i < COLUMN; i++)
    {
        SDL_RenderFillRect(renderer, &rect1);
        rect1.x = rect1.x + CELL_SIZE;
    }
}

void fill_cell(SDL_Renderer *renderer, int x, int y, int R, int G, int B)
{
    SDL_SetRenderDrawColor(renderer, R, G, B, 255);
    SDL_FRect rect = {x, y, CELL_SIZE, CELL_SIZE};
    SDL_RenderFillRect(renderer, &rect);
}

int rng(int rmax, int rmin)
{
    int rnum = (rand() % (rmax - rmin)) + rmin;
    return rnum;
}

void draw_food(SDL_Renderer *renderer, int *food, int *x, int *y, struct snake blocks[], int *psnake_length)
{
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    if (*food == 0)
    {
        *x = X_RNG;
        *y = Y_RNG;
        while (*x == WIDTH / 2 && *y == HEIGHT / 2)
        {
            *x = X_RNG;
            *y = Y_RNG;
        }
        SDL_FRect rect = {*x, *y, CELL_SIZE, CELL_SIZE};
        SDL_RenderFillRect(renderer, &rect);
        *food = 1;
    }

    else if (blocks[0].xcoord == *x && blocks[0].ycoord == *y)
    {
        *x = X_RNG;
        *y = Y_RNG;
        int position = 1;
        while (position)
        {
            position = 0;
            for (int i = 0; i < *psnake_length; i++)
            {
                if (blocks[i].xcoord == *x && blocks[i].ycoord == *y)
                {
                    *x = X_RNG;
                    *y = Y_RNG;
                    position = 1;
                }
            }
        }

        SDL_FRect rect = {*x, *y, CELL_SIZE, CELL_SIZE};
        SDL_RenderFillRect(renderer, &rect);
        if (*psnake_length < MAX_LENGTH)
        {
            (*psnake_length)++;
        }
    }
    else
    {

        SDL_FRect rect = {*x, *y, CELL_SIZE, CELL_SIZE};
        SDL_RenderFillRect(renderer, &rect);
    }
}

void movement(SDL_Renderer *renderer, struct snake blocks[], int *psnake_length, int *x_temp, int *y_temp, int *x_temp1, int *y_temp1, int *pbutton)
{
    switch (*pbutton)
    {
    case 1:
        blocks[0].xcoord = blocks[0].xcoord - CELL_SIZE;
        break;
    case 2:
        blocks[0].ycoord = blocks[0].ycoord - CELL_SIZE;
        break;
    case 3:
        blocks[0].xcoord = blocks[0].xcoord + CELL_SIZE;
        break;
    case 4:
        blocks[0].ycoord = blocks[0].ycoord + CELL_SIZE;
        break;
    }

    for (int i = 1; i <= *psnake_length; i++)
    {
        *x_temp1 = blocks[i].xcoord;
        *y_temp1 = blocks[i].ycoord;

        blocks[i].xcoord = *x_temp;
        blocks[i].ycoord = *y_temp;

        *x_temp = *x_temp1;
        *y_temp = *y_temp1;
    }
}

void draw_scoreboard_ui(SDL_Renderer *renderer, char score[], int *psnake_length, TTF_Font *font, SDL_Color color, SDL_Texture *texture, SDL_Surface *surface)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_FRect scoreboard = {0, 0, WIDTH, CELL_SIZE};
    SDL_RenderFillRect(renderer, &scoreboard);

    sprintf(score, "%d", *psnake_length - 1);
    surface = TTF_RenderText_Solid(font, score, 0, color);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FRect position_rect = {20, (CELL_SIZE - surface->h) / 2, surface->w, surface->h};
    SDL_RenderTexture(renderer, texture, NULL, &position_rect);
}

void game_over_screen(SDL_Renderer *renderer, char final_score[], int *psnake_length)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_FRect box = {275, 370, 450, 100};
    SDL_RenderFillRect(renderer, &box);

    SDL_Color black = {0, 0, 0, 255};

    // game over
    TTF_Font *font_end = TTF_OpenFont("PressStart2P-Regular.ttf", 40);
    SDL_Surface *surface = TTF_RenderText_Solid(font_end, "GAME OVER", 0, black);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FRect position_rect_go_screen = {box.x + (box.w - surface->w) / 2, box.y + ((box.h - surface->h) / 2) - 10, surface->w, surface->h};
    SDL_RenderTexture(renderer, texture, NULL, &position_rect_go_screen);

    // final score
    sprintf(final_score, "Final Score: %d", *psnake_length - 1);
    TTF_Font *font_finalscore = TTF_OpenFont("PressStart2P-Regular.ttf", 20);
    SDL_Surface *surface_final = TTF_RenderText_Solid(font_finalscore, final_score, 0, black);
    SDL_Texture *texture_final = SDL_CreateTextureFromSurface(renderer, surface_final);

    SDL_FRect position_rect_finalscore = {box.x + (box.w - surface_final->w) / 2, box.y + ((box.h - surface_final->h) / 2) + 25, surface_final->w, surface_final->h};
    SDL_RenderTexture(renderer, texture_final, NULL, &position_rect_finalscore);
}

SDL_Event event;
int main()
{
    srand(time(NULL));

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("Failed to initialize SDL");
        return -1;
    }

    if (!TTF_Init())
    {
        SDL_Log("Failed to initialize TTF");
        return -1;
    }

    SDL_Window *window = SDL_CreateWindow("Classic Snake", WIDTH, HEIGHT, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

    TTF_Font *font = TTF_OpenFont("PressStart2P-Regular.ttf", 32);

    SDL_Color white = {255, 255, 255, 255};

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Surface *surface = TTF_RenderText_Solid(font, "0", 0, white);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FRect position_rect = {10, (CELL_SIZE - surface->h) / 2, surface->w, surface->h};
    SDL_RenderTexture(renderer, texture, NULL, &position_rect);

    // variables
    int gx = 350, gy = 250;
    int snake_length = 1;
    int *psnake_length = &snake_length;
    int x_rand, y_rand;
    int *px_rand = &x_rand;
    int *py_rand = &y_rand;

    struct snake blocks[MAX_LENGTH] = {{gx, gy}};

    int food = 0;
    int *pfood = &food;
    // button is current direction
    int button = 3;
    int *pbutton = &button;
    // next_button is pending direction
    int next_button = 3;

    char score[50];
    char final_score[50];

    int collision = 0;

    Uint64 t1 = SDL_GetTicks();

    int running = 1;
    while (running)
    {
        int x_temp = blocks[0].xcoord;
        int *px_temp = &x_temp;
        int y_temp = blocks[0].ycoord;
        int *py_temp = &y_temp;

        int x_temp1;
        int *px_temp1 = &x_temp1;
        int y_temp1;
        int *py_temp1 = &y_temp1;

        if (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                running = 0;
            }
            if (event.type == SDL_EVENT_KEY_DOWN)
            {
                if (event.key.key == SDLK_DOWN)
                {

                    if (button != 4 && button != 2)
                    {
                        next_button = 4;
                    }
                }
                if (event.key.key == SDLK_UP)
                {

                    if (button != 2 && button != 4)
                    {
                        next_button = 2;
                    }
                }
                if (event.key.key == SDLK_LEFT)
                {

                    if (button != 1 && button != 3)
                    {
                        next_button = 1;
                    }
                }
                if (event.key.key == SDLK_RIGHT)
                {

                    if (button != 3 && button != 1)
                    {
                        next_button = 3;
                    }
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // box borders
        for (int i = 0; i < COLUMN; i++)
        {
            CELL(i * CELL_SIZE, CELL_SIZE, 255, 255, 255);
            CELL(i * CELL_SIZE, (ROW - 1) * CELL_SIZE, 255, 255, 255);
        }
        for (int i = 0; i < ROW; i++)
        {
            CELL(0, i * CELL_SIZE, 255, 255, 255);
            CELL((COLUMN - 1) * CELL_SIZE, i * CELL_SIZE, 255, 255, 255);
        }

        Uint64 t2 = SDL_GetTicks();
        int time = (t2 - t1);

        // collision checker
        {
            if (blocks[0].xcoord <= CELL_SIZE || blocks[0].xcoord >= WIDTH - 2 * CELL_SIZE || blocks[0].ycoord <= 2 * CELL_SIZE || blocks[0].ycoord >= HEIGHT - 2 * CELL_SIZE)
            {
                collision = 1;
            }
            else
            {
                for (int i = 1; i < snake_length; i++)
                {
                    if (blocks[0].xcoord == blocks[i].xcoord && blocks[0].ycoord == blocks[i].ycoord)
                    {
                        collision = 1;
                    }
                }
            }
        }

        // movement loop
        if (time >= 100 && collision != 1)
        {

            MOVEMENT;
            button = next_button;
            t1 = t2;
        }

        // drawing out the snake
        for (int i = 0; i < snake_length; i++)
        {
            CELL(blocks[i].xcoord, blocks[i].ycoord, 255, 255, 0);
        }

        GRID;
        FOOD;
        SCOREBOARD;

        if (collision == 1)
        {
            END_SCREEN;
        }

        SDL_Log("%d\n", snake_length);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_DestroySurface(surface);
    SDL_DestroyTexture(texture);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
    return 0;
}