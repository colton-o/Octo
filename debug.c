#include "debug.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>
SDL_Texture *texture, *text;
TTF_Font *font;
SDL_Color debug_color = {233, 210, 192, 255};
SDL_Rect text_box = {0, 0, 700, 100};
char *d_text;
void debug_start() {
  TTF_Init();
  font = TTF_OpenFont("Light And Airy.ttf", 1000);
  if (font == NULL) {
    printf("no font loaded", TTF_GetError());
  }
}
void debug_update(SDL_Renderer *renderer, tentacle *t) {
  d_text = malloc(100);
  int x = 100;

  sprintf(d_text, "Pos = %d, %d \n Dist = %d \n Pull = %d",
          t[0].tentacle_pos.d.x, t[0].tentacle_pos.d.y, t[0].distance,
          t[0].pulling);

  SDL_Surface *text_surf = TTF_RenderText_Solid(font, d_text, debug_color);

  SDL_Texture *text_tex = SDL_CreateTextureFromSurface(renderer, text_surf);
  SDL_RenderCopy(renderer, text_tex, NULL, &text_box);
  free(d_text);
}
