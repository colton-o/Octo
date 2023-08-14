
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <math.h>
#define WINX 1000
#define WINY 1000
#define CURVE 100

const int SCREEN_FPS = 100;
const int SCREEN_TICK = 1000 / 60;

int speed = 5;

SDL_Point curve_points[CURVE] = {0};

#include "bezier.h"

fvector_2 velocity;
vector_2 position;
vector_2 click_pos;
vector_2 direction;
float length;

typedef struct {
  vector_2 orig_pos;
  curve tentacle_pos;
  int distance;
  uint8_t pulling;
} tentacle;

tentacle tentacles[8] = {0};
SDL_Rect *octopus;

void set_tentacle(tentacle *t) {
  // t->tentacle_pos.d.x = octopus->x + t->orig_pos.x;
  // t->tentacle_pos.d.y = octopus->y + t->orig_pos.y;
  t->tentacle_pos.c.x = (t->tentacle_pos.a.x + t->tentacle_pos.d.x) / 2;
  t->tentacle_pos.c.y = (t->tentacle_pos.a.y + t->tentacle_pos.d.y) / 2;
  t->tentacle_pos.b.x = (t->tentacle_pos.c.x + t->tentacle_pos.d.x) / 2;
  t->tentacle_pos.b.y = (t->tentacle_pos.c.y + t->tentacle_pos.d.y) / 2;
}

void set_tentacle_distance(tentacle *t) {
  t->distance = sqrt(pow(t->tentacle_pos.d.x - (octopus->x), 2) +
                     pow(t->tentacle_pos.d.y - (octopus->y), 2));
}

int main(int argc, char *argv[]) {
  octopus = malloc(sizeof(SDL_Rect));
  octopus->h = 25;
  octopus->w = 25;
  octopus->x = WINX / 2;
  octopus->y = WINY / 2;
  click_pos.x = octopus->x;
  click_pos.y = octopus->y;

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

  for (int i = 0; i < 8; i++) {
    tentacles[i].tentacle_pos.d.x = octopus->x + tentacles[i].orig_pos.x;
    tentacles[i].tentacle_pos.d.y = octopus->y + tentacles[i].orig_pos.y;
  }

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

  // debug_start();
  while (1) {
    framestart = SDL_GetTicks64();
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_MOUSEBUTTONDOWN:
          SDL_GetMouseState(&click_pos.x, &click_pos.y);

          break;
      }
      if (event.type == SDL_QUIT) return 0;
    }

    // octopus->x += velocity.x * speed;
    direction.x = click_pos.x - octopus->x;
    direction.y = click_pos.y - octopus->y;
    length = sqrt(pow(direction.x, 2) + pow(direction.y, 2));
    if (direction.x != 0) velocity.x = direction.x / length;

    if (direction.y != 0) velocity.y = direction.y / length;

    if (velocity.y < 0) {
      if (tentacles[0].pulling) {
        octopus->y += velocity.y * speed;
        if (tentacles[4].pulling && tentacles[4].distance > 300) {
          tentacles[4].tentacle_pos.d.y += velocity.y * speed * 2;
        }
      } else
        tentacles[0].tentacle_pos.d.y += velocity.y * speed;
    } else {
      if (tentacles[4].pulling) octopus->y += velocity.y * speed;

      if (tentacles[0].pulling && tentacles[0].distance > 300) {
        tentacles[0].tentacle_pos.d.y += velocity.y * speed * 2;
      } else
        tentacles[4].tentacle_pos.d.y += velocity.y * speed;
    }

    if (velocity.x < 0) {
      if (tentacles[6].pulling)
        octopus->x += velocity.x * speed;
      else
        tentacles[6].tentacle_pos.d.x += velocity.x * speed;
    } else {
      if (tentacles[2].pulling)
        octopus->x += velocity.x * speed;
      else
        tentacles[2].tentacle_pos.d.x += velocity.x * speed;
    }

    SDL_SetRenderDrawColor(renderer, 70, 70, 70, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
    SDL_RenderFillRect(renderer, octopus);

    for (int i = 0; i < 8; i++) {
      tentacles[i].tentacle_pos.a.x = octopus->x + (octopus->w / 2);
      tentacles[i].tentacle_pos.a.y = octopus->y + (octopus->h / 2);
      set_tentacle_distance(&tentacles[i]);
      if (tentacles[i].distance > 200)
        tentacles[i].pulling = 0;
      else if (tentacles[i].distance < 10)
        tentacles[i].pulling = 1;

      set_tentacle(&tentacles[i]);

      SDL_Point curve_points_neg[CURVE] = {0};
      SDL_Point curve_points_pos[CURVE] = {0};

      set_curve(tentacles[i].tentacle_pos, CURVE, curve_points);

      for (int i = 0; i < CURVE; i++) {
        int thick = round((double)i / (double)CURVE);
        thick = (1 - thick);
        curve_points_neg[i].x = curve_points[i].x - thick;
        curve_points_neg[i].y = curve_points[i].y;
        curve_points_pos[i].x = curve_points[i].x + thick;
        curve_points_pos[i].y = curve_points[i].y;
      }

      SDL_RenderDrawLines(renderer, curve_points, CURVE);
      SDL_RenderDrawLines(renderer, curve_points_neg, CURVE);
      SDL_RenderDrawLines(renderer, curve_points_pos, CURVE);
    }

    SDL_RenderPresent(renderer);
    frametime = SDL_GetTicks64() - framestart;
    if (SCREEN_TICK > frametime) {
      SDL_Delay(SCREEN_TICK - frametime);
    }

    // debug_update(renderer, tentacles);
  }
  free(octopus);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
