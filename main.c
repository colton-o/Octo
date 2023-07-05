#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <math.h>
#define WINX 1000
#define WINY 1000

const int SCREEN_FPS = 100;
const int SCREEN_TICK = 1000 / 60;

int speed = 5;

typedef struct {
  int x;
  int y;
} vector_2;

vector_2 velocity;
vector_2 position;
typedef struct {
  vector_2 cur_pos;
  vector_2 orig_pos;
  int distance;
} tentacle;

tentacle tentacles[8] = {0};
SDL_Rect *octopus;

void set_tentacle(tentacle *t) {
  t->cur_pos.x = octopus->x + t->orig_pos.x;
  t->cur_pos.y = octopus->y + t->orig_pos.y;
}
void set_tentacle_distance(tentacle *t) {
  t->distance = sqrt(pow(t->cur_pos.x - (t->orig_pos.x + octopus->x), 2) +
                     pow(t->cur_pos.y - (t->orig_pos.y + octopus->y), 2));
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
  tentacles[5].orig_pos.y = 50;

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
      set_tentacle_distance(&tentacles[i]);

      if (tentacles[i].distance > 350)
        set_tentacle(&tentacles[i]);

      SDL_RenderDrawLine(renderer, octopus->x + (octopus->w / 2),
                         octopus->y + (octopus->h / 2), tentacles[i].cur_pos.x,
                         tentacles[i].cur_pos.y);
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
