#include <SDL/SDL.h>

#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

void resize_to(SDL_Surface *src, SDL_Surface *dst);
void read_color(FILE *tin, SDL_Color *color);
void set_rect(SDL_Rect *rect, int x, int y, int w, int h);

#endif // FUNCTIONS_H_INCLUDED
