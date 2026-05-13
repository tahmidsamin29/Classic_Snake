#include <stdio.h>
#include <SDL3/SDL.h>
#include <time.h>
#include <stdlib.h>

// constant declarations
#define WIDTH 800
#define HEIGHT 600
#define CELL_SIZE 50
#define ROW HEIGHT / CELL_SIZE
#define COLUMN WIDTH / CELL_SIZE
#define ROW_LIM ROW - 1
#define COL_LIM COLUMN - 1
#define MAX_LENGTH 120

struct snake
{
    int xcoord;
    int ycoord;
};

// macros
#define GRID draw_grid(renderer)
#define CELL(x, y) fill_cell(renderer, x, y)
#define X_RNG (rng(COL_LIM, 1) * CELL_SIZE)
#define Y_RNG (rng(ROW_LIM, 1) * CELL_SIZE)
#define FOOD draw_food(renderer, pfood, px_rand, py_rand, blocks, psnake_length)
#define MOVEMENT movement(renderer, blocks, psnake_length, px_temp, py_temp, px_temp1, py_temp1, pbutton)
#define SCOREBOARD draw_scoreboard_ui(renderer)

void draw_grid(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_FRect rect = {0, 0, WIDTH, 1};
    for (int i = 0; i < ROW; i++)
    {
        SDL_RenderFillRect(renderer, &rect);
        rect.y = rect.y + CELL_SIZE;
    }
    SDL_FRect rect1 = {0, 0, 1, HEIGHT};
    for (int i = 0; i < COLUMN; i++)
    {
        SDL_RenderFillRect(renderer, &rect1);
        rect1.x = rect1.x + CELL_SIZE;
    }
}

void fill_cell(SDL_Renderer *renderer, int x, int y)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
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

void draw_scoreboard_ui(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_FRect scoreboard = {0, 0, WIDTH, CELL_SIZE};
    SDL_RenderFillRect(renderer, &scoreboard);
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
    SDL_Window *window = SDL_CreateWindow("Classic Snake", WIDTH, HEIGHT, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

    // variables
    int gx = 350, gy = 250;
    int snake_length = 1;
    int *psnake_length = &snake_length;
    int x_rand, y_rand;
    int *px_rand = &x_rand;
    int *py_rand = &y_rand;

    struct snake blocks[MAX_LENGTH] = {{gx, gy}};

    int running = 1;
    int food = 0;
    int *pfood = &food;
    // button is current direction
    int button = 3;
    int *pbutton = &button;
    // next_button is pending direction
    int next_button = 3;

    Uint64 t1 = SDL_GetTicks();

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

        FOOD;

        for (int i = 0; i < snake_length; i++)
        {
            CELL(blocks[i].xcoord, blocks[i].ycoord);
        }

        Uint64 t2 = SDL_GetTicks();
        int time = (t2 - t1);

        if (time >= 100)
        {

            MOVEMENT;
            button = next_button;
            t1 = t2;
        }

        GRID;
        SCOREBOARD;
        SDL_Log("%d\n", snake_length);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}