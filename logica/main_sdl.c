#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <stdio.h>
#include "logica.h"

#define ALTO 13
#define ANCHO 13

void mostrar_mapa(Juego *j) {
    for(int i = 0; i < ALTO; i++) {
        for(int k = 0; k < ANCHO; k++) {
            printf("%d ", j->mapa[i][k]);
        }
        printf("\n");
    }
    printf("\n");
}

int main(int argc, char* args[]) {

    // =============================
    // Inicializar SDL
    // =============================
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Error al inicializar SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* ventana = SDL_CreateWindow(
        "Battle City - Test",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        640, 480,
        SDL_WINDOW_SHOWN
    );

    if (!ventana) {
        printf("No se pudo crear ventana: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }


    // =============================
    // Leer mapa desde archivo
    // =============================

    Juego juego;

    //Abrir archivo
    FILE *fp = fopen("mapa.txt", "r");   
    if (fp == NULL) {
        printf("Error al abrir el archivo");
        return 1;
    }

    int buffer[169];
    int c;

    //anade los valores del mapa a un arreglo
    int valores[169], contador=0;
    while ((c=fgetc(fp)) != EOF) {
        if (isdigit(c)){
            valores[contador]=c-'0';
            contador++;
            }
        }

    fclose(fp);
    
    //anade los valores del arreglo a la matriz mapa
    contador=0;
    for(int i=0; i<ALTO; i++){
        for(int j=0; j<ANCHO; j++){
            juego.mapa[i][j]=valores[contador];
            contador++;
        }
    }



    // =============================
    // Inicializar lógica del juego
    // =============================
    inicializar_juego(&juego);

    printf("Mapa inicial:\n");
    mostrar_mapa(&juego);


    // =============================
    // Bucle principal
    // =============================
    int corriendo = 1;
    SDL_Event e;

    printf("Controles: W A S D\n");
    printf("Salir: ESC\n\n");

    while (corriendo) {

        // -------------------------
        // Procesar eventos SDL
        // -------------------------
        while (SDL_PollEvent(&e)) {

            if (e.type == SDL_QUIT) {
                corriendo = 0;
            }

            if (e.type == SDL_KEYDOWN) {

                SDL_Keycode tecla = e.key.keysym.sym;

                switch (tecla) {
                    case SDLK_w:
                        juego.jugador1.direccion=0;
                        mover_tanque(&juego, &juego.jugador1, 3);
                        break;
                    case SDLK_d:
                        juego.jugador1.direccion=1;
                        mover_tanque(&juego, &juego.jugador1, 3);
                        break;
                    case SDLK_s:
                        juego.jugador1.direccion=2;
                        mover_tanque(&juego, &juego.jugador1, 3);
                        break;
                    case SDLK_a:
                        juego.jugador1.direccion=3;
                        mover_tanque(&juego, &juego.jugador1, 3);
                        break;

                    case SDLK_ESCAPE:
                        corriendo = 0;
                        break;

                    case SDLK_f:
                        disparar(&juego, &juego.jugador1);
                }
            }
        }

        // -------------------------
        // Lógica automática
        // -------------------------
        actualizar_estado(&juego);

        // -------------------------
        // Mostrar estado (solo textual)
        // -------------------------
        mostrar_mapa(&juego);

        SDL_Delay(200); // Para que no vaya tan rápido
    }


    // =============================
    // Salida limpia
    // =============================
    SDL_DestroyWindow(ventana);
    SDL_Quit();

    return 0;
}
