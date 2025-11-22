#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "logica.h"

#define filas 13
#define columnas 13
#define tamano 64

int main(int argc, char* args[]) {

    // ===================//
    //  Inicializar SDL   //
    // ===================//

    if(SDL_Init(SDL_INIT_VIDEO)!=0){
        printf("Error al inicializar SDL: %s\n",SDL_GetError());
        return EXIT_FAILURE;
    }
    //Inicializar SDL_Image
    if(!(IMG_Init(IMG_INIT_PNG)&IMG_INIT_PNG)){
        printf("Error al inicializar SDL_Image: %s\n",IMG_GetError());
        return EXIT_FAILURE;
    }
    //Crear ventana
    SDL_Window *ventana=SDL_CreateWindow("Juego SDL",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,832,832,SDL_WINDOW_SHOWN);
    if(ventana==NULL){
        printf("Error al inicializar SDL_CreateWindow: %s\n",SDL_GetError);
        return EXIT_FAILURE;
    }   
    //Crear render
    SDL_Renderer *render=SDL_CreateRenderer(ventana,-1,SDL_RENDERER_ACCELERATED);
    if(render==NULL){
        printf("Error al inicializar SDL_CreateRenderer: %s\n",SDL_GetError);
        return EXIT_FAILURE;
    }


    //Cargar imagenes

    SDL_Texture *imagenes[9];
    SDL_Surface *surface0=IMG_Load("suelo.jpg");
    SDL_Surface *surface1=IMG_Load("pared.jpg");
    SDL_Surface *surface2=IMG_Load("irrompible.jpg");
    SDL_Surface *surface3=IMG_Load("tanque.png");
    SDL_Surface *surface4=IMG_Load("tanque.png");
    SDL_Surface *surface5=IMG_Load("balaup.png");
    SDL_Surface *surface6=IMG_Load("balaright.png");
    SDL_Surface *surface7=IMG_Load("baladown.png");
    SDL_Surface *surface8=IMG_Load("balaleft.png");

    imagenes[0]=SDL_CreateTextureFromSurface(render,surface0);
    imagenes[1]=SDL_CreateTextureFromSurface(render,surface1);
    imagenes[2]=SDL_CreateTextureFromSurface(render,surface2);
    imagenes[3]=SDL_CreateTextureFromSurface(render,surface3);
    imagenes[4]=SDL_CreateTextureFromSurface(render,surface4);
    imagenes[5]=SDL_CreateTextureFromSurface(render,surface5);
    imagenes[6]=SDL_CreateTextureFromSurface(render,surface6);
    imagenes[7]=SDL_CreateTextureFromSurface(render,surface7);
    imagenes[8]=SDL_CreateTextureFromSurface(render,surface8);

    SDL_FreeSurface(surface0);
    SDL_FreeSurface(surface1);
    SDL_FreeSurface(surface2);
    SDL_FreeSurface(surface3);
    SDL_FreeSurface(surface4);
    SDL_FreeSurface(surface5);
    SDL_FreeSurface(surface6);
    SDL_FreeSurface(surface7);
    SDL_FreeSurface(surface8);





    // =========================//
    // Leer mapa desde archivo  //
    // =========================//

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
    
    // reservar memoria dinámica
    juego.mapa = malloc(sizeof(int*) * ALTO);
    for (int i=0; i<ALTO; i++) {
        juego.mapa[i] = malloc(sizeof(int) * ANCHO);
    }

    //anade los valores del arreglo a la matriz mapa
    contador=0;
    for(int i=0; i<ALTO; i++){
        for(int j=0; j<ANCHO; j++){
            juego.mapa[i][j]=valores[contador];
            contador++;
        }
    }






    // =============================//
    //      Bucle principal         //
    // =============================//

    inicializar_juego(&juego);
    int corriendo = 1;
    SDL_Event e;

    while (corriendo) {

        // -------------------------//
        //     Entradas SDL         //
        // -------------------------//
        while (SDL_PollEvent(&e)) {

            if (e.type == SDL_QUIT) {
                corriendo = 0;
            }
            if (e.type == SDL_KEYDOWN) {
                SDL_Keycode tecla = e.key.keysym.sym;

                switch (tecla) {

                    //Movimientos jugador 1
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

                    //Movimientos jugador 2
                    case SDLK_UP:
                        juego.jugador2.direccion=0;
                        mover_tanque(&juego, &juego.jugador2, 4);
                        break;
                    case SDLK_RIGHT:
                        juego.jugador2.direccion=1;
                        mover_tanque(&juego, &juego.jugador2, 4);
                        break;
                    case SDLK_DOWN:
                        juego.jugador2.direccion=2;
                        mover_tanque(&juego, &juego.jugador2, 4);
                        break;
                    case SDLK_LEFT:
                        juego.jugador2.direccion=3;
                        mover_tanque(&juego, &juego.jugador2, 4);
                        break;



                    case SDLK_ESCAPE:
                        corriendo = 0;
                        break;

                    case SDLK_f:
                        disparar(&juego, &juego.jugador1);
                        break;
                    case SDLK_l:
                        disparar(&juego, &juego.jugador2);
                        break;
                }
            }
        }

        actualizar_estado(&juego);


        

        // -------------------------//
        //      Dibujar mapa        //
        // -------------------------//

        SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
        SDL_RenderClear(render);

        for(int i=0;i<filas;i++){
            for(int j=0;j<columnas;j++){
                SDL_Rect destRect={j*tamano,i*tamano,tamano,tamano};
                switch(juego.mapa[i][j]){
                    case 0:
                        SDL_RenderCopy(render,imagenes[0],NULL,&destRect);
                        break;
                    case 1:
                        SDL_RenderCopy(render,imagenes[1],NULL,&destRect);
                        break;
                    case 2:
                        SDL_RenderCopy(render,imagenes[2],NULL,&destRect);
                        break;
                    case 3:
                        SDL_RenderCopy(render,imagenes[3],NULL,&destRect);
                        break;
                    case 4:
                        SDL_RenderCopy(render,imagenes[4],NULL,&destRect);
                        break;
                    case 5:
                        SDL_RenderCopy(render,imagenes[5],NULL,&destRect);
                        break;
                    case 6:
                        SDL_RenderCopy(render,imagenes[6],NULL,&destRect);
                        break;
                    case 7:
                        SDL_RenderCopy(render,imagenes[7],NULL,&destRect);
                        break;
                    case 8:
                        SDL_RenderCopy(render,imagenes[8],NULL,&destRect);
                        break;
                }

            }
        }
        SDL_RenderPresent(render);

        juego_terminado(&juego);
        if (juego_terminado(&juego) == 1){
            SDL_Delay(3000);
            break;
        }

        SDL_Delay(100);     //Frecuencia de actualizacion cada 100ms
    }



    //Liberar memoria
    for (int i = 0; i < ALTO; i++){
        free(juego.mapa[i]);
        free(juego.mapa);
    }

    //Cierre de ventana
    SDL_DestroyWindow(ventana);
    SDL_Quit();

    return 0;
}

