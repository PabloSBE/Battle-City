#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "logica.h"
#include <ctype.h>
#include "audio.h"
#include "inicio.h"

#define filas 13
#define columnas 13
#define tamano 64


void cargar_datos_desde_archivo(Juego *juego, int mapa_est[filas][columnas]) {
    FILE *fp = fopen("mapa.txt", "r");   
    if (fp == NULL) {
        printf("Error al abrir el archivo mapa.txt\n");
        return;
    }

    int c;
    int valores[169];
    int contador = 0;

    // Leer valores
    while ((c = fgetc(fp)) != EOF && contador < 169) {
        if (isdigit(c)){
            valores[contador] = c - '0';
            contador++;
        }
    }
    fclose(fp);

    // 1. Rellenar Mapa Estático (Paredes)
    // El mapa estatico contiene objetos que no cambiaran su estado durante el juego
    contador = 0;
    for(int i = 0; i < filas; i++){
        for(int j = 0; j < columnas; j++){
            // Si es algo movible o destructible, en el estático va VACIO
            if (valores[contador] == DESTRUCTIBLE || valores[contador] == TANQUE1 || 
                valores[contador] == TANQUE2 || valores[contador] == VIDA ||
                valores[contador] >= 5) { // >= 5 son balas
                
                mapa_est[i][j] = VACIO;
            } else {
                mapa_est[i][j] = valores[contador];
            }
            contador++;
        }
    }
    
    // 2. Rellenar Mapa Dinámico (Juego)
    // Este mapa contendra todos los bloques y entidades del juego
    contador = 0;
    for(int i = 0; i < filas; i++){
        for(int j = 0; j < columnas; j++){
            juego->mapa[i][j] = valores[contador];
            contador++;
        }
    }

    // 3. Inicializar posiciones y estados de tanques
    inicializar_juego(juego);
}

int main(int argc, char* args[]) {

    //Inicializar SDL 
    SDL_Window *ventana;
    SDL_Renderer *render;
    TTF_Font *fuente;
    TTF_Font *fuenteBoton;

    if (!iniciarsdl(&ventana,&render,&fuente,&fuenteBoton)){
        return EXIT_FAILURE;
    }

    if (!cargar_audio()){
    return EXIT_FAILURE;
    }

    //Cargar imagenes
    SDL_Texture *imagenes[10];

    //Mapa
    SDL_Surface *surface0=IMG_Load("suelo.jpg");
    SDL_Surface *surface1=IMG_Load("pared.jpg");
    SDL_Surface *surface2=IMG_Load("irrompible.jpg");

    //Entidades
    SDL_Surface *surface3=IMG_Load("tanque1.png");
    SDL_Surface *surface4=IMG_Load("tanque2.png");
    SDL_Surface *surface5=IMG_Load("balaup.png");
    SDL_Surface *surface6=IMG_Load("balaright.png");
    SDL_Surface *surface7=IMG_Load("baladown.png");
    SDL_Surface *surface8=IMG_Load("balaleft.png");
    SDL_Surface *surface9=IMG_Load("vida.png");

    imagenes[0]=SDL_CreateTextureFromSurface(render,surface0);
    imagenes[1]=SDL_CreateTextureFromSurface(render,surface1);
    imagenes[2]=SDL_CreateTextureFromSurface(render,surface2);
    imagenes[3]=SDL_CreateTextureFromSurface(render,surface3);
    imagenes[4]=SDL_CreateTextureFromSurface(render,surface4);
    imagenes[5]=SDL_CreateTextureFromSurface(render,surface5);
    imagenes[6]=SDL_CreateTextureFromSurface(render,surface6);
    imagenes[7]=SDL_CreateTextureFromSurface(render,surface7);
    imagenes[8]=SDL_CreateTextureFromSurface(render,surface8);
    imagenes[9]=SDL_CreateTextureFromSurface(render,surface9);

    SDL_FreeSurface(surface0);
    SDL_FreeSurface(surface1);
    SDL_FreeSurface(surface2);
    SDL_FreeSurface(surface3);
    SDL_FreeSurface(surface4);
    SDL_FreeSurface(surface5);
    SDL_FreeSurface(surface6);
    SDL_FreeSurface(surface7);
    SDL_FreeSurface(surface8);
    SDL_FreeSurface(surface9);


    // Generar nuevo mapa    
    generar_archivo_mapa();

    // =========================//
    // Leer mapa desde archivo  //
    // =========================//

    Juego juego;
    int mapa_est[filas][columnas]; 

    juego.mapa = malloc(sizeof(int*) * ALTO);
    for (int i=0; i<ALTO; i++) {
        juego.mapa[i] = malloc(sizeof(int) * ANCHO);
    }

    cargar_datos_desde_archivo(&juego, mapa_est);

    
    // =============================//
    //      Bucle principal         //
    // =============================//

    // Ajusta las variables de los tanques
    inicializar_juego(&juego);
    int corriendo = 1;
    SDL_Event e;
    double angulo1=0;
    double angulo1ob=0;
    double angulo2=0;
    double angulo2ob=0;

    Uint32 ultimo = 0;
    Uint32 intervalo = 6000;   //10seg
    
    int fin = 0;
    int menu = 1;
    
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

                if(tecla == SDLK_ESCAPE) {
                    corriendo = 0;
                }

                else if(!fin) {
                    switch (tecla) {

                        //Movimientos jugador 1
                        case SDLK_w:
                            juego.jugador1.direccion=0;
                            mover_tanque(&juego, &juego.jugador1, 3);
                            Mix_PlayChannel(-1, snd_mover, 0);
                            angulo1ob=0;
                            break;
                        case SDLK_d:
                            juego.jugador1.direccion=1;
                            mover_tanque(&juego, &juego.jugador1, 3);
                            Mix_PlayChannel(-1, snd_mover, 0);
                            angulo1ob=90;
                            break;
                        case SDLK_s:
                            juego.jugador1.direccion=2;
                            mover_tanque(&juego, &juego.jugador1, 3);
                            Mix_PlayChannel(-1, snd_mover, 0);
                            angulo1ob=180;
                            break;
                        case SDLK_a:
                            juego.jugador1.direccion=3;
                            mover_tanque(&juego, &juego.jugador1, 3);
                            Mix_PlayChannel(-1, snd_mover, 0);
                            angulo1ob=270;
                            break;

                        //Movimientos jugador 2
                        case SDLK_UP:
                            juego.jugador2.direccion=0;
                            mover_tanque(&juego, &juego.jugador2, 4);
                            Mix_PlayChannel(-1, snd_mover, 0);
                            angulo2ob=0;
                            break;
                        case SDLK_RIGHT:
                            juego.jugador2.direccion=1;
                            mover_tanque(&juego, &juego.jugador2, 4);
                            Mix_PlayChannel(-1, snd_mover, 0);
                            angulo2ob=90;
                            break;
                        case SDLK_DOWN:
                            juego.jugador2.direccion=2;
                            mover_tanque(&juego, &juego.jugador2, 4);
                            Mix_PlayChannel(-1, snd_mover, 0);
                            angulo2ob=180;
                            break;
                        case SDLK_LEFT:
                            juego.jugador2.direccion=3;
                            mover_tanque(&juego, &juego.jugador2, 4);
                            Mix_PlayChannel(-1, snd_mover, 0);
                            angulo2ob=270;
                            break;

                        //Entradas de disparos
                        case SDLK_f:
                            disparar(&juego, &juego.jugador1); 
                            break;
                        case SDLK_l:
                            disparar(&juego, &juego.jugador2);
                            
                            break;

                        case SDLK_ESCAPE:
                            corriendo = 0;
                            break;
                        case SDLK_p:
                            generar_guardado_mapa(juego.mapa);
                            break;
                    }
                }
            }
        }
        // Animación suave de rotación
        double velocidadRotacion=30; // grados por frame

        // Ajusta el angulo para una rotacion mas corta
        if(angulo1ob==0 && angulo1>180)angulo1 -= 360;
        if(angulo1ob==270 && angulo1<90)angulo1 += 360;

        //Rotacion Horaria
        if(angulo1<angulo1ob){
            angulo1+=velocidadRotacion;
            if(angulo1>angulo1ob)angulo1=angulo1ob;

        //Rotacion AntiHoraria
        }else if(angulo1>angulo1ob){
            angulo1-=velocidadRotacion;
            if(angulo1<angulo1ob)angulo1=angulo1ob;
        }

        // Ajusta el angulo para una rotacion mas corta
        if(angulo2ob==0 && angulo2>180)angulo2 -= 360;
        if(angulo2ob==270 && angulo2<90)angulo2 += 360;

        //Rotacion Horaria
        if(angulo2<angulo2ob){
            angulo2+=velocidadRotacion;
            if(angulo2>angulo2ob)angulo2=angulo2ob;

        //Rotacion AntiHoraria
        }else if(angulo2>angulo2ob){
            angulo2-=velocidadRotacion;
            if (angulo2<angulo2ob)angulo2=angulo2ob;
        }

        // -------------------------//
        //      Dibujar Menu        //
        // -------------------------//
        SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
        SDL_RenderClear(render);
      
        if(menu) {
            int opcion = dibujar_menu(render, fuente, fuenteBoton, &juego);
        
            if(opcion == 1) {
                generar_archivo_mapa();           
                cargar_datos_desde_archivo(&juego, mapa_est); 
                menu = 0;
            }

            if(opcion == 2) {
                FILE *fCheck = fopen("partida.txt", "r");
                
                if(fCheck == NULL) {
                    printf("No existe partida guardada.\n");
                } else {
                    fclose(fCheck);
                    
                    remove("mapa.txt");
                    rename("partida.txt", "mapa.txt"); 
                    
                    cargar_datos_desde_archivo(&juego, mapa_est);
                    menu = 0;
                }
            }

            else if(opcion == 0) {
                corriendo = 0;
            }
        }

        else if(fin) {
            dibujar_fin(render, fuente, &juego);

            remove("partida.txt");
        }

        // -------------------------//
        //      Dibujar mapa        //
        // -------------------------//
        else {

            /* Primero se lee y dibuja el mapa con bloques estaticos, posteriormente
            se dibujan las entidades, de mode que queden por encima del mapa */

            //Dibuja bloques estaticos
            for(int i=0;i<filas;i++){
                for(int j=0;j<columnas;j++){
                    SDL_Rect destRect={j*tamano,i*tamano,tamano,tamano};
                    switch(mapa_est[i][j]){
                        case 0:
                            SDL_RenderCopy(render,imagenes[0],NULL,&destRect);
                            break;
                        case 2:
                            SDL_RenderCopy(render,imagenes[2],NULL,&destRect);
                            break;
                    }
                }
            } 

            //Dibuja entidades
            for(int i=0;i<filas;i++){
                for(int j=0;j<columnas;j++){
                    SDL_Rect destRect={j*tamano,i*tamano,tamano,tamano};
                    switch(juego.mapa[i][j]){
                        case 1:
                            SDL_RenderCopy(render,imagenes[1],NULL,&destRect);
                            break;
                        case 3:
                            SDL_RenderCopyEx(render,imagenes[3],NULL,&destRect,angulo1,NULL,SDL_FLIP_NONE);
                            break;
                        case 4:
                            SDL_RenderCopyEx(render,imagenes[4],NULL,&destRect,angulo2,NULL,SDL_FLIP_NONE);
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
                        case 9:
                            SDL_RenderCopy(render,imagenes[9],NULL,&destRect);
                            break;
                    }
                }
            }

            // Muestra la cantidad de vidas y disparos de los tanques
            dibujar_hud(render, fuente, &juego);

            SDL_RenderPresent(render);

            // genera vidas cada 6 segundos
            Uint32 ahora = SDL_GetTicks();
            if (ahora - ultimo >= intervalo) {
                vidas(&juego);      
                ultimo = ahora;
            }

            actualizar_estado(&juego);

            if (juego_terminado(&juego) == 1){
                SDL_Delay(1000);
                fin = 1; 
            }
        }

        SDL_RenderPresent(render);
        SDL_Delay(66); //Frecuencia de actualizacion cada 100ms
    }

    //Liberar memoria
    for (int i = 0; i < ALTO; i++){
        free(juego.mapa[i]);
    }

    liberar_audio();     

    free(juego.mapa);

    cerrarsdl(ventana, render, fuente, fuenteBoton);

    return 0;
}
