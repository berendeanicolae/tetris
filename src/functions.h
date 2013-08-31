#include <SDL/SDL.h>

#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

void resize_to(SDL_Surface *src, SDL_Surface *dst);
void read_color(FILE *tin, SDL_Color *color);
void set_rect(SDL_Rect *rect, Sint16 x, Sint16 y, Uint16 w, Uint16 h);

#endif // FUNCTIONS_H_INCLUDED
