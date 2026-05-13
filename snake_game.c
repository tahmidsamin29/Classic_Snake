#include <stdio.h>
#include <SDL3/SDL.h>
#include <time.h>
#include <stdlib.h>

// constant declarations

#define WIDTH 800
#define HEIGHT 600
#define MAX_LENGTH 120
#define CELL_SIZE 50
#define ROW HEIGHT / CELL_SIZE
#define COLUMN WIDTH / CELL_SIZE

// macros for functions

#define GRID draw_grid(renderer)
#define SCOREBOARD draw_scoreboard_ui(renderer)

typedef struct snake
{
    int xcoords;
    int ycoords;
} SNAKE;

void draw_grid(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_FRect row = {CELL_SIZE, CELL_SIZE, WIDTH, 1};
    for (int i = 0; i < ROW - 1; i++)
    {
        SDL_RenderFillRect(renderer, &row);
        row.y = row.y + CELL_SIZE;
    }

    SDL_FRect column = {CELL_SIZE, CELL_SIZE, 1, HEIGHT};
    for (int i = 0; i < COLUMN - 1; i++)
    {
        SDL_RenderFillRect(renderer, &column);
        column.x = column.x + CELL_SIZE;
    }
}

void draw_scoreboard_ui(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_FRect scoreboard = {0, 0, WIDTH, CELL_SIZE};
    SDL_RenderFillRect(renderer, &scoreboard);
}

void fill_cell(SDL_Renderer *renderer, int x, int y, int R, int G, int B)
{
    SDL_SetRenderDrawColor(renderer, R, G, B, 255);
    SDL_FRect cell = {x, y, CELL_SIZE, CELL_SIZE};
    SDL_RenderFillRect(renderer, &cell);
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

    SDL_Window *window = SDL_CreateWindow("Snake", WIDTH, HEIGHT, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

    // variables
    int x = WIDTH / 2;
    int y = HEIGHT / 2;

    int snake_length = 1;
    int *p_snake_length = &snake_length;

    SNAKE snake_body[MAX_LENGTH] = {{x, y}};

    int running = 1;
    while (running)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        if (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                running = 0;
            }
        }

        GRID;
        SCOREBOARD;

        fill_cell(renderer, CELL_SIZE * 2, CELL_SIZE * 6, 255, 255, 255);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}