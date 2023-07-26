#include "bezier.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <math.h>

#define WINX 1000
#define WINY 1000
#define CURVE 50
#define SDL_MAIN_HANDLED
const int SCREEN_FPS = 100;
const int SCREEN_TICK = 1000 / 60;

int speed = 5;

SDL_Point curve_points[CURVE] = {0};

vector_2 velocity;
vector_2 position;

typedef struct {
  vector_2 orig_pos;
  curve tentacle_pos;
  int distance;
} tentacle;

tentacle tentacles[8] = {0};
SDL_Rect *octopus;

void set_tentacle(tentacle *t) {
  t->tentacle_pos.d.x = octopus->x + t->orig_pos.x;
  t->tentacle_pos.d.y = octopus->y + t->orig_pos.y;
  t->tentacle_pos.c.x = (t->tentacle_pos.a.x + t->tentacle_pos.d.x) / 2;
  t->tentacle_pos.c.y= (t->tentacle_pos.a.y + t->tentacle_pos.d.y) / 2;
  t->tentacle_pos.b.x = (t->tentacle_pos.c.x + t->tentacle_pos.d.x) / 2;
  t->tentacle_pos.b.y = (t->tentacle_pos.c.y + t->tentacle_pos.d.y) / 2;
}
void set_tentacle_distance(tentacle *t) {
  t->distance = sqrt(pow(t->tentacle_pos.d.x - (octopus->x), 2) +
                     pow(t->tentacle_pos.d.y - (octopus->y), 2));
}
int main(void) {
  octopus = malloc(sizeof(SDL_Rect));
  octopus->h = 25;
  octopus->w = 25;
  octopus->x = WINX / 2;
  octopus->y = WINY / 2;

  tentacles[0].orig_pos.x = 0;
  tentacles[0].orig_pos.y = -100;

  tentacles[1].orig_pos.x = 100;
  tentacles[1].orig_pos.y = -100;

  tentacles[2].orig_pos.x = 100;
  tentacles[2].orig_pos.y = 0;

  tentacles[3].orig_pos.x = 100;
  tentacles[3].orig_pos.y = 100;

  tentacles[4].orig_pos.x = 0;
  tentacles[4].orig_pos.y = 100;

  tentacles[5].orig_pos.x = -100;
  tentacles[5].orig_pos.y = 100;

  tentacles[6].orig_pos.x = -100;
  tentacles[6].orig_pos.y = 0;

  tentacles[7].orig_pos.x = -100;
  tentacles[7].orig_pos.y = -100;

  uint32_t framestart;

  int frametime;
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window =
      SDL_CreateWindow("mywindow", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, WINX, WINY, SDL_WINDOW_SHOWN);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

  SDL_SetRenderDrawColor(renderer, 70, 70, 70, 255);
  SDL_RenderClear(renderer);
  SDL_Event event;
  while (1) {
    framestart = SDL_GetTicks64();
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_KEYDOWN:
        switch (event.key.keysym.sym) {
        case SDLK_w:
          velocity.y = -1;
          break;
        case SDLK_s:
          velocity.y = 1;
          break;
        case SDLK_a:
          velocity.x = -1;
          break;
        case SDLK_d:
          velocity.x = 1;
          break;
        }
        break;
      case SDL_KEYUP:
        switch (event.key.keysym.sym) {
        case SDLK_w:
          velocity.y = 0;
          break;
        case SDLK_s:
          velocity.y = 0;
          break;
        case SDLK_a:
          velocity.x = 0;
          break;
        case SDLK_d:
          velocity.x = 0;
          break;
        }
        break;
      }
      if (event.type == SDL_QUIT)
        return 0;
    }

    octopus->x += velocity.x * speed;
    octopus->y += velocity.y * speed;

    SDL_SetRenderDrawColor(renderer, 70, 70, 70, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 100);
    SDL_RenderFillRect(renderer, octopus);

    for (int i = 0; i < 8; i++) {
      tentacles[i].tentacle_pos.a.x = octopus->x + (octopus->w / 2);
      tentacles[i].tentacle_pos.a.y = octopus->y + (octopus->h / 2);
      set_tentacle_distance(&tentacles[i]);

      if (tentacles[i].distance > 300 || tentacles[i].distance < 1)
        set_tentacle(&tentacles[i]);

      set_curve(tentacles[i].tentacle_pos, CURVE, curve_points);

      SDL_RenderDrawLines(renderer, curve_points, CURVE);
    }

    SDL_RenderPresent(renderer);
    frametime = SDL_GetTicks64() - framestart;
    if (SCREEN_TICK > frametime) {
      SDL_Delay(SCREEN_TICK - frametime);
    }
  }
  free(octopus);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
