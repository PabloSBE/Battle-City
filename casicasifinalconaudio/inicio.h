#ifndef INICIO_H
#define INICIO_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

int iniciarsdl(SDL_Window **ventana,SDL_Renderer **render,TTF_Font **fuente,TTF_Font **fuenteBoton);

void cerrarsdl(SDL_Window *ventana,SDL_Renderer *render,TTF_Font *fuente,TTF_Font *fuenteBoton);
#endif