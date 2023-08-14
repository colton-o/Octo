#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>

SDL_Texture *texture, *text;
TTF_Font *font;
SDL_Color debug_color = {233, 210, 192, 255};
SDL_Rect text_box = {0, 0, 100, 100};
char *d_text;
void debug_start() {
  TTF_Init();
  font = TTF_OpenFont("Light And Airy.ttf", 1000);
  if (font == NULL) {
    printf("no font loaded", TTF_GetError());
  }
  sprintf(d_text, "testing");
}
void debug_update(SDL_Renderer *renderer) {
  SDL_Surface *text_surf = TTF_RenderText_Solid(font, "dfsdf", debug_color);

  SDL_Texture *text_tex = SDL_CreateTextureFromSurface(renderer, text_surf);
  SDL_RenderCopy(renderer, text_tex, NULL, &text_box);
  printf("rendering");
}
