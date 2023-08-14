#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

SDL_Texture *texture, *text;
TTF_Font *font;
SDL_Color debug_color = {233, 210, 192, 255};
SDL_Rect text_box = {0, 0, 100, 100};
void debug_start() { font = TTF_OpenFont("Light and Airy.ttf", 30); }
void debug_update(SDL_Renderer *renderer) {
  SDL_Surface *text_surf =
      TTF_RenderText_Solid(font, "Hello World", debug_color);

  SDL_Texture *text_tex = SDL_CreateTextureFromSurface(renderer, text_surf);
  SDL_RenderCopy(renderer, text_tex, NULL, &text_box);
}
