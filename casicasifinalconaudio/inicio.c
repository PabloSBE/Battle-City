#include <stdio.h>
#include "inicio.h"


#define SDL_MAIN_HANDLED

int iniciarsdl(SDL_Window **ventana,
                SDL_Renderer **render,
                TTF_Font **fuente,
                TTF_Font **fuenteBoton)
{
    //SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        printf("Error al iniciar SDL: %s\n", SDL_GetError());
        return 0;
    }

    //SDL_image
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        printf("Error al iniciar SDL_image: %s\n", IMG_GetError());
        return 0;
    }

    //Crear ventana
    *ventana=SDL_CreateWindow("Juego SDL",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,832, 832 + 120,SDL_WINDOW_SHOWN);
    if (!*ventana){
        printf("Error al crear ventana: %s\n",SDL_GetError());
        return 0;
    }

    //Crear renderer
    *render=SDL_CreateRenderer(*ventana,-1,SDL_RENDERER_ACCELERATED);
    if (!*render){
        printf("Error al crear renderer: %s\n", SDL_GetError());
        return 0;
    }

    //SDL_ttf
    if (TTF_Init()==-1){
        printf("Error al iniciar TTF: %s\n", TTF_GetError());
        return 0;
    }

    *fuente=TTF_OpenFont("PressStart2P-Regular.ttf",28);
    *fuenteBoton=TTF_OpenFont("PressStart2P-Regular.ttf",14);

    if (!*fuente || !*fuenteBoton){
        printf("Error cargando fuentes\n");
        return 0;
    }

    return 1;
}

void cerrarsdl(SDL_Window *ventana,SDL_Renderer *render,TTF_Font *fuente,TTF_Font *fuenteBoton){
    if (fuente) TTF_CloseFont(fuente);
    if (fuenteBoton) TTF_CloseFont(fuenteBoton);

    if (render) SDL_DestroyRenderer(render);
    if (ventana) SDL_DestroyWindow(ventana);

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}