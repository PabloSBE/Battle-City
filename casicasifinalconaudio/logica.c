#include "logica.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include "audio.h"

void dibujar_texto(SDL_Renderer* r, TTF_Font* f, const char* msg, int x, int y){

    SDL_Color blanco = {255, 255, 255};

    SDL_Surface* s = TTF_RenderText_Solid(f, msg, blanco);
    SDL_Texture* t = SDL_CreateTextureFromSurface(r, s);

    SDL_Rect rect = {x, y, s->w, s->h};
    SDL_RenderCopy(r, t, NULL, &rect);

    SDL_FreeSurface(s);
    SDL_DestroyTexture(t);
}

void dibujar_boton(SDL_Renderer *render, TTF_Font *fuente, Boton *boton) {
    // 1. Dibujar el rectángulo del fondo
    SDL_SetRenderDrawColor(render, boton->color.r, boton->color.g, boton->color.b, boton->color.a);
    SDL_RenderFillRect(render, &boton->rect);

    // 2. Dibujar el borde (opcional, para que se vea mejor)
    SDL_SetRenderDrawColor(render, 255, 255, 255, 255); // Borde blanco
    SDL_RenderDrawRect(render, &boton->rect);

    // 3. Calcular posición para centrar el texto
    int texto_w, texto_h;
    TTF_SizeText(fuente, boton->texto, &texto_w, &texto_h);
    
    int texto_x = boton->rect.x + (boton->rect.w - texto_w) / 2;
    int texto_y = boton->rect.y + (boton->rect.h - texto_h) / 2;

    // 4. Usar tu función existente para dibujar el texto encima
    // Nota: Necesitas modificar ligeramente tu dibujar_texto o usar esta lógica directa:
    dibujar_texto(render, fuente, boton->texto, texto_x, texto_y);
}

void dibujar_hud(SDL_Renderer *render, TTF_Font *fuente, Juego *juego){

    int hudY = 832;
    int hudH = 120;

    SDL_Rect hudRect = {0, hudY, 832, hudH};
    SDL_SetRenderDrawColor(render, 20, 20, 20, 255);
    SDL_RenderFillRect(render, &hudRect);

    char texto[64];

    //tanque 1
    sprintf(texto, "T1 Vidas: %d", juego->jugador1.vida);
    dibujar_texto(render, fuente, texto, 20, hudY + 20);

    sprintf(texto, "T1 Balas: %d", juego->jugador1.disparos);
    dibujar_texto(render, fuente, texto, 20, hudY + 60);

    //tanque 2
    sprintf(texto, "T2 Vidas: %d", juego->jugador2.vida);
    dibujar_texto(render, fuente, texto, 832 - 340, hudY + 20);

    sprintf(texto, "T2 Balas: %d", juego->jugador2.disparos);
    dibujar_texto(render, fuente, texto, 832 - 340, hudY + 60);
}

int dibujar_menu(SDL_Renderer *render, TTF_Font *fuente, TTF_Font *fuenteBoton, Juego *juego) {
  SDL_Event e;

  Boton btnNuevo;
  btnNuevo.rect = (SDL_Rect) {290, 225, 250, 75};
  btnNuevo.color = (SDL_Color) {0, 150, 0, 255};
  sprintf(btnNuevo.texto, "Nueva Partida");

  Boton btnCargar;
  btnCargar.rect = (SDL_Rect) {290, 350, 250, 75};
  btnCargar.color = (SDL_Color) {0, 0, 150, 255};
  sprintf(btnCargar.texto, "Cargar Partida");

  Boton btnSalir;
  btnSalir.rect = (SDL_Rect) {290, 475, 250, 75};
  btnSalir.color = (SDL_Color) {150, 0, 0, 255};
  sprintf(btnSalir.texto, "Salir");

  while(1) {
      while(SDL_PollEvent(&e)) {
        if(e.type == SDL_MOUSEBUTTONDOWN) {
          if(e.button.button == SDL_BUTTON_LEFT) {
            int x = e.button.x;
            int y = e.button.y;

            if(click_boton(&btnNuevo, x, y)) {
              return 1;
            }
            
            if(click_boton(&btnCargar, x, y)) {
              return 2; 
            }

            if(click_boton(&btnSalir, x, y)) {
              return 0;
            }
          }
        }
      }

    SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
    SDL_RenderClear(render);
    
    dibujar_texto(render, fuente, "Battle City", 270, 100);

    dibujar_boton(render, fuenteBoton, &btnNuevo);
    dibujar_boton(render, fuenteBoton, &btnCargar);
    dibujar_boton(render, fuenteBoton, &btnSalir);

    SDL_RenderPresent(render);
  }
}
void dibujar_fin(SDL_Renderer *render, TTF_Font *fuente, Juego *juego) {
  SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
  SDL_RenderClear(render);

  char mensaje[50];

  if(juego->jugador1.vida <= 0)
    sprintf(mensaje, "Jugador 2 Gana");
  else 
    sprintf(mensaje, "Jugador 1 Gana");

  dibujar_texto(render, fuente, "Fin Del Juego", 235, 125);
  dibujar_texto(render, fuente, mensaje, 210, 350);
  dibujar_texto(render, fuente, "Presione ESC para salir", 100, 455);
}

int click_boton(Boton *boton, int x, int y) {
  if(x >= boton->rect.x && x <= (boton->rect.x + boton->rect.w) && y >= boton->rect.y && y <= (boton->rect.y + boton->rect.h)) {
      return 1;
  }

  return 0;
}

// Lee mapa, busca posiciones de tanques
void inicializar_juego(Juego *juego) {

    //Busca los tanques
    for(int i = 0; i < ALTO; i++) {
        for(int j = 0; j < ANCHO; j++) {

            //Guarda la ubicacion del jugador 1 en el struct
            if(juego->mapa[i][j] == 3) {
                juego->jugador1.fila = i;
                juego->jugador1.columna = j;
                juego->jugador1.vida = 3;
                juego->jugador1.disparos = 0;
                juego->jugador1.direccion = 0;
            }

            //Guarda la ubicacion del jugador 2 en el struct
            if(juego->mapa[i][j] == 4) {
                juego->jugador2.fila = i;
                juego->jugador2.columna = j;
                juego->jugador2.vida = 3;
                juego->jugador2.disparos = 0;
                juego->jugador2.direccion = 0;
            }
        }
    }
}

void vidas(Juego *juego){

    int x = rand() % 13;
    int y = rand() % 13;
  
    if(juego->mapa[x][y] != 2 && juego->mapa[x][y] != 3 && juego->mapa[x][y] != 4)
      juego->mapa[x][y] = 9;
}


// Actualiza balas
void actualizar_estado(Juego *juego) {
    actualizar_balas(juego);
}

// Movimiento del tanque
int mover_tanque(Juego *juego, Tanque *t, int jugador) {

    //Arriba
    if (t->direccion==0 && juego->mapa[(t->fila)-1][t->columna]==VACIO){
        juego->mapa[t->fila][t->columna]=VACIO;
        t->fila -= 1;
        juego->mapa[t->fila][t->columna]=jugador;
        return 1;

    }else if (t->direccion==0 && juego->mapa[(t->fila)-1][t->columna]==VIDA){
        Mix_PlayChannel(-1, snd_vida, 0);
        juego->mapa[t->fila][t->columna]=VACIO;
        t->fila -= 1;
        juego->mapa[t->fila][t->columna]=jugador;
        t->vida+=1;
        return 1;
    } 

    //Derecha
    else if (t->direccion==1 && juego->mapa[t->fila][(t->columna)+1]==VACIO){
        juego->mapa[t->fila][t->columna]=VACIO;
        t->columna += 1;
        juego->mapa[t->fila][t->columna]=jugador;
        return 1;

    }else if (t->direccion==1 && juego->mapa[t->fila][(t->columna)+1]==VIDA){
        Mix_PlayChannel(-1, snd_vida, 0);
        juego->mapa[t->fila][t->columna]=VACIO;
        t->columna += 1;
        juego->mapa[t->fila][t->columna]=jugador;
        t->vida+=1;
        return 1;
    }

    //Abajo
    else if (t->direccion==2 && juego->mapa[(t->fila)+1][t->columna]==VACIO){
        juego->mapa[t->fila][t->columna]=VACIO;
        t->fila += 1;
        juego->mapa[t->fila][t->columna]=jugador;
        return 1;

    }else if (t->direccion==2 && juego->mapa[(t->fila)+1][t->columna]==VIDA){
        Mix_PlayChannel(-1, snd_vida, 0);
        juego->mapa[t->fila][t->columna]=VACIO;
        t->fila += 1;
        juego->mapa[t->fila][t->columna]=jugador;
        t->vida+=1;
        return 1;
    }

    //Izquierda
    else if (t->direccion==3  && juego->mapa[t->fila][(t->columna)-1]==VACIO){
        juego->mapa[t->fila][t->columna]=VACIO;
        t->columna -= 1;
        juego->mapa[t->fila][t->columna]=jugador;
        return 1;

    }else if (t->direccion==3  && juego->mapa[t->fila][(t->columna)-1]==VIDA){
        Mix_PlayChannel(-1, snd_vida, 0);
        juego->mapa[t->fila][t->columna]=VACIO;
        t->columna -= 1;
        juego->mapa[t->fila][t->columna]=jugador;
        t->vida+=1;
        return 1;
    }

    else{
        return 0;  
    }

}

// Crea bala desde la posición del tanque
void disparar(Juego *juego, Tanque *t) {

    t->disparos+=1;


    Mix_PlayChannel(-1, snd_disparo, 0);
    //disparo hacia arriba
    if(t->direccion==0 && juego->mapa[(t->fila)-1][t->columna]==VACIO){
        juego->mapa[(t->fila)-1][t->columna]=BALA_ARRIBA;
    }
    else if(t->direccion==0 && juego->mapa[(t->fila)-1][t->columna]==DESTRUCTIBLE || t->direccion==0 && juego->mapa[(t->fila)-1][t->columna]==VIDA){
        Mix_PlayChannel(-1, snd_explosion, 0);
        juego->mapa[(t->fila)-1][t->columna]=VACIO;
    }
    else if(t->direccion==0 && juego->mapa[(t->fila)-1][t->columna]==3){
        Mix_PlayChannel(-1, snd_disparo, 0);
        Mix_PlayChannel(-1, snd_daño, 0);
        juego->jugador1.vida-=1;
    }
    else if(t->direccion==0 && juego->mapa[(t->fila)-1][t->columna]==4){
        Mix_PlayChannel(-1, snd_disparo, 0);
        Mix_PlayChannel(-1, snd_daño, 0);
        juego->jugador2.vida-=1;
    }

    //disparo hacia la derecha
    else if(t->direccion==1 && juego->mapa[t->fila][(t->columna)+1]==VACIO){
        juego->mapa[t->fila][(t->columna)+1]=BALA_DERECHA;
    }
    else if(t->direccion==1 && juego->mapa[t->fila][(t->columna)+1]==DESTRUCTIBLE || t->direccion==1 && juego->mapa[t->fila][(t->columna)+1]==VIDA){
        Mix_PlayChannel(-1, snd_explosion, 0);
        juego->mapa[t->fila][(t->columna)+1]=VACIO;
    }
    else if(t->direccion==1 && juego->mapa[t->fila][(t->columna)+1]==3){
        Mix_PlayChannel(-1, snd_disparo, 0);
        Mix_PlayChannel(-1, snd_daño, 0);
        juego->jugador1.vida-=1;
    }
    else if(t->direccion==1 && juego->mapa[t->fila][(t->columna)+1]==4){
        Mix_PlayChannel(-1, snd_disparo, 0);
        Mix_PlayChannel(-1, snd_daño, 0);
        juego->jugador2.vida-=1;
    }

    //disparo hacia abajo
    else if(t->direccion==2 && juego->mapa[(t->fila)+1][t->columna]==VACIO){
        juego->mapa[(t->fila)+1][t->columna]=BALA_ABAJO;
    }
    else if(t->direccion==2 && juego->mapa[(t->fila)+1][t->columna]==DESTRUCTIBLE || t->direccion==2 && juego->mapa[(t->fila)+1][t->columna]==VIDA){
        Mix_PlayChannel(-1, snd_explosion, 0);
        juego->mapa[(t->fila)+1][t->columna]=VACIO;
    }
    else if(t->direccion==2 && juego->mapa[(t->fila)+1][t->columna]==3){
        Mix_PlayChannel(-1, snd_disparo, 0);
        Mix_PlayChannel(-1, snd_daño, 0);
        juego->jugador1.vida-=1;
    }
    else if(t->direccion==2 && juego->mapa[(t->fila)+1][t->columna]==4){
        Mix_PlayChannel(-1, snd_disparo, 0);
        Mix_PlayChannel(-1, snd_daño, 0);
        juego->jugador2.vida-=1;
    }

    //disparo hacia la izquierda
    else if(t->direccion==3 && juego->mapa[t->fila][(t->columna)-1]==VACIO){
        juego->mapa[t->fila][(t->columna)-1]=BALA_IZQUIERDA;
    }
    else if(t->direccion==3 && juego->mapa[t->fila][(t->columna)-1]==DESTRUCTIBLE || t->direccion==3 && juego->mapa[t->fila][(t->columna)-1]==VIDA){
        Mix_PlayChannel(-1, snd_explosion, 0);
        juego->mapa[t->fila][(t->columna)-1]=VACIO;
    }
    else if(t->direccion==3 && juego->mapa[t->fila][(t->columna)-1]==3){
        Mix_PlayChannel(-1, snd_disparo, 0);
        Mix_PlayChannel(-1, snd_daño, 0);
        juego->jugador1.vida-=1;
    }
    else if(t->direccion==3 && juego->mapa[t->fila][(t->columna)-1]==4){
        Mix_PlayChannel(-1, snd_disparo, 0);
        Mix_PlayChannel(-1, snd_daño, 0);
        juego->jugador2.vida-=1;
    }

    else{
        t->disparos-=1;
    }

}

// Actualiza posición de balas e interacciones
void actualizar_balas(Juego *juego) {

    int mapa_nuevo[ALTO][ANCHO];

    //Copia el mapa
    for(int i = 0; i < ALTO; i++) {
        for(int j = 0; j < ANCHO; j++) {
            mapa_nuevo[i][j] = juego->mapa[i][j];
        }
    }

    for(int i = 0; i < ALTO; i++) {
        for(int j = 0; j < ANCHO; j++) {

            //Colision ante cada tipo de elemento
            if(juego->mapa[i][j]==BALA_ARRIBA){
                if(juego->mapa[(i)-1][j]==VACIO){
                    mapa_nuevo[i][j]=VACIO;
                    mapa_nuevo[(i)-1][j]=BALA_ARRIBA;
                }
                else if(juego->mapa[(i)-1][j]==INDESTRUCTIBLE){
                    mapa_nuevo[i][j]=VACIO;
                }
                else if(juego->mapa[(i)-1][j]==DESTRUCTIBLE){
                    Mix_PlayChannel(-1, snd_explosion, 0);
                    mapa_nuevo[i][j]=VACIO;
                    mapa_nuevo[(i)-1][j]=VACIO;
                }
                else if(juego->mapa[(i)-1][j]==TANQUE1){
                    Mix_PlayChannel(-1, snd_daño, 0);
                    juego->jugador1.vida-=1;
                    mapa_nuevo[i][j]=VACIO;
                }
                else if(juego->mapa[(i)-1][j]==TANQUE2){
                    Mix_PlayChannel(-1, snd_daño, 0);
                    juego->jugador2.vida-=1;
                    mapa_nuevo[i][j]=VACIO;
                }
		        else{
		            mapa_nuevo[i][j]=VACIO;
                    mapa_nuevo[(i)-1][j]=VACIO;
		        }
		        if(i-2>=0){
		            if (juego->mapa[(i)-2][j]==BALA_ABAJO){
                    Mix_PlayChannel(-1, snd_explosion, 0);
		                mapa_nuevo[i-1][j]=VACIO;
		                mapa_nuevo[(i)-2][j]=VACIO;
		            }
		        }

            }

            if(juego->mapa[i][j]==BALA_DERECHA){
                if(juego->mapa[i][j+1]==VACIO){
                    mapa_nuevo[i][j]=VACIO;
                    mapa_nuevo[i][j+1]=BALA_DERECHA;
                }
                else if(juego->mapa[i][j+1]==INDESTRUCTIBLE){
                    mapa_nuevo[i][j]=VACIO;
                }
                else if(juego->mapa[i][j+1]==DESTRUCTIBLE){
                    mapa_nuevo[i][j]=VACIO;
                    mapa_nuevo[i][j+1]=VACIO;
                }
                else if(juego->mapa[i][j+1]==TANQUE1){
                    Mix_PlayChannel(-1, snd_daño, 0);
                    juego->jugador1.vida-=1;
                    mapa_nuevo[i][j]=VACIO;
                }
                else if(juego->mapa[i][j+1]==TANQUE2){
                    Mix_PlayChannel(-1, snd_daño, 0);
                    juego->jugador2.vida-=1;
                    mapa_nuevo[i][j]=VACIO;
                }
		        else{
                    mapa_nuevo[i][j]=VACIO;
                    mapa_nuevo[i][j+1]=VACIO;
		        }
		
		        if(j+2<=12){
		          if(juego->mapa[i][j+2]==BALA_IZQUIERDA){
                    Mix_PlayChannel(-1, snd_explosion, 0);
                    mapa_nuevo[i][j+1]=VACIO;
		                mapa_nuevo[i][j+2]=VACIO;
		          }
		        }

            } 
             
            if(juego->mapa[i][j]==BALA_ABAJO){
                if(juego->mapa[(i)+1][j]==VACIO){
                    mapa_nuevo[i][j]=VACIO;
                    mapa_nuevo[(i)+1][j]=BALA_ABAJO;
                }
                else if(juego->mapa[(i)+1][j]==INDESTRUCTIBLE){
                    mapa_nuevo[i][j]=VACIO;
                }
                else if(juego->mapa[(i)+1][j]==DESTRUCTIBLE){
                    mapa_nuevo[i][j]=VACIO;
                    mapa_nuevo[(i)+1][j]=VACIO;
                }
                else if(juego->mapa[(i)+1][j]==TANQUE1){
                    Mix_PlayChannel(-1, snd_daño, 0);
                    juego->jugador1.vida-=1;
                    mapa_nuevo[i][j]=VACIO;
                }
                else if(juego->mapa[(i)+1][j]==TANQUE2){
                    Mix_PlayChannel(-1, snd_daño, 0);
                    juego->jugador2.vida-=1;
                    mapa_nuevo[i][j]=VACIO;
                }
		        else{
                    mapa_nuevo[i][j]=VACIO;
                    mapa_nuevo[(i)+1][j]=VACIO;
		        }
		
		        if (i+2<=12){
		          if (juego->mapa[(i)+2][j]==BALA_ARRIBA){
                    Mix_PlayChannel(-1, snd_explosion, 0);
                    mapa_nuevo[i+1][j]=VACIO;
		            mapa_nuevo[(i)+2][j]=VACIO;
		            }
		        }

            }

            if(juego->mapa[i][j]==BALA_IZQUIERDA){
                if(juego->mapa[i][j-1]==VACIO){
                    mapa_nuevo[i][j]=VACIO;
                    mapa_nuevo[i][j-1]=BALA_IZQUIERDA;
                }
                else if(juego->mapa[i][j-1]==INDESTRUCTIBLE){
                    mapa_nuevo[i][j]=VACIO;
                }
                else if(juego->mapa[i][j-1]==DESTRUCTIBLE){
                    mapa_nuevo[i][j]=VACIO;
                    mapa_nuevo[i][j-1]=VACIO;
                }
                else if(juego->mapa[i][j-1]==TANQUE1){
                    Mix_PlayChannel(-1, snd_daño, 0);
                    juego->jugador1.vida-=1;
                    mapa_nuevo[i][j]=VACIO;
                }
                else if(juego->mapa[i][j-1]==TANQUE2){
                    Mix_PlayChannel(-1, snd_daño, 0);
                    juego->jugador2.vida-=1;
                    mapa_nuevo[i][j]=VACIO;
                }
		        else{
                    mapa_nuevo[i][j]=VACIO;
                    mapa_nuevo[i][j-1]=VACIO;
		        }
		
		        if(j-2>=0){
		          if(juego->mapa[i][j-2]==BALA_DERECHA){
                    Mix_PlayChannel(-1, snd_explosion, 0);
                    mapa_nuevo[i][j-1]=VACIO;
		            mapa_nuevo[i][j-2]=VACIO;
		          }
		        }

            } 
        }
    }

    for(int i = 0; i < ALTO; i++) {
        for(int j = 0; j < ANCHO; j++) {
            juego->mapa[i][j] = mapa_nuevo[i][j];
        }
    }
}


// Retorna 1 si alguien ganó 
int juego_terminado(Juego *juego) {

    if(juego->jugador1.vida<=0){
        return 1;

    }

    if(juego->jugador2.vida<=0){
        return 1;
    }

    return 0;
}

#define LARGO 13
#define ANCHO 13

void liberar_memoria(int **mapa) {
  if(mapa == NULL)
    return;

  // Liberar memoria de cada fila
  for(int i = 0; i < ANCHO; i++) {
    free(mapa[i]);
  }

  free(mapa);
}

void dfs(int x, int y, int **mapa, int visitado[ANCHO][LARGO]) {
  if(x < 0 || x >= ANCHO || y < 0 || y >= LARGO)
    return;

  if(visitado[x][y])
    return;

  if(mapa[x][y] == 2)
    return;

  visitado[x][y] = 1;

  dfs(x + 1, y, mapa, visitado);
  dfs(x - 1, y, mapa, visitado);
  dfs(x, y + 1, mapa, visitado);
  dfs(x, y - 1, mapa, visitado);
}

int conectados(int **mapa) {
  int visitado[ANCHO][LARGO] = {0};

  int x = -1, y = -1;

  for(int i = 1; i < ANCHO - 1; i++) {
    for(int j = 1; j < LARGO - 1; j++) {
      if(mapa[i][j] != 2) {
        x = i;
        y = j;
        break;
      }
    }

    if(x != -1)
      break;
  }

  if(x == -1)
    return 0;

  dfs(x, y, mapa, visitado);

  for(int i = 1; i < ANCHO - 1; i++) {
    for(int j = 1; j < LARGO - 1; j++) {
      if(mapa[i][j] != 2 && !visitado[i][j]) {
        return 0;
      }
    }
  }

  return 1;
}

int** generar_mapa() {
  srand(time(NULL));

  int **mapa = (int**)malloc(ANCHO * sizeof(int*));

  if(mapa == NULL) {
    printf("No se ha podido asignar memoria para las filas del mapa \n");
    return NULL;
  }

  for(int i = 0; i < ANCHO; i++) {
    mapa[i] = (int*)malloc(LARGO * sizeof(int));

    if(mapa[i] == NULL) {
      printf("No se ha podido asignar memoria para las columnas del mapa \n");
      return NULL;
    }
  }

  // 0 = Espacio vacios
  // 1 = Bloque destructible
  // 2 = Bloque indestructible
  // 3 = Tanque 1
  // 4 = Tanque 2

  // Generamos un mapa vacio (celdas 0) y con un borde indestructible (celdas 2) 
  for(int i = 0; i < ANCHO; i++) {
    for(int j = 0; j < LARGO; j++) {
      if(i == 0 || i == ANCHO - 1|| j == 0 || j == LARGO - 1) { 
        mapa[i][j] = 2;
      }
      else {
        mapa[i][j] = 0;
      }
    }
  }
    
  for(int i = 1; i < LARGO - 1; i++) {
    for(int j = 1; j < ANCHO - 1; j++) {
      int probabilidad = (rand() % (10 - 1 + 1) + 1);


      if(probabilidad > 6 && probabilidad <= 8) {
        mapa[i][j] = 2;
      }

      else if(probabilidad > 8) {
        mapa[i][j] = 1;
      }
    }
  }

  return mapa;
}

void colocar_tanques(int **mapa) {
  int tanque1_x, tanque1_y, tanque2_x, tanque2_y;

  while(1) {
    tanque1_x = (rand() % (12 - 1 + 1) + 1);
    tanque1_y = (rand() % (12 - 1 + 1) + 1);

    if(mapa[tanque1_x][tanque1_y] != 2){
      mapa[tanque1_x][tanque1_y] = 3;
      break;
    }
  }

  while(1) {
    tanque2_x = (rand() % (12 - 1 + 1) + 1);
    tanque2_y = (rand() % (12 - 1 + 1) + 1);

    if(mapa[tanque2_x][tanque2_y] != 2 && (abs(tanque2_x - tanque1_x) + abs(tanque2_y - tanque1_y)) >= 7){
      mapa[tanque2_x][tanque2_y] = 4;
      break;
    }
  }
}

void generar_archivo_mapa() {

  int **mapa;
  int cantidad_mapas = 0;

  FILE *fp;

  do {
    if(cantidad_mapas == 1)
      liberar_memoria(mapa);

    mapa = generar_mapa();
    cantidad_mapas = 1;

    if(mapa == NULL) {
      return;
    }

    if(conectados(mapa)) {
      colocar_tanques(mapa);
      break;
    }

  } while(1); 

  for(int i = 0; i < LARGO; i++) {
    for(int j = 0; j < ANCHO; j++) {
      printf("%d ", mapa[i][j]);
    }

    printf("\n");
  }

  fp = fopen("mapa.txt", "w");

  if(fp == NULL) {
    printf("Ha habido un problema al abrir el archivo");
    return;
  }

  for(int i = 0; i < LARGO; i++) {
    for(int j = 0; j < ANCHO; j++) {
      fprintf(fp, "%d ", mapa[i][j]);
    }

    fprintf(fp, "\n");
  }

  fclose(fp);
  liberar_memoria(mapa);
}

void generar_guardado_mapa(int **mapa) {
  FILE *fp;

  fp = fopen("partida.txt", "w");

  if(fp == NULL) {
    printf("Ha habido nu problema al guardar el mapa");
    return;
  }

  for(int i = 0; i < LARGO; i++) {
    for(int j = 0; j < ANCHO; j++) {
      fprintf(fp, "%d ", mapa[i][j]);
    }

    fprintf(fp, "\n");
  }

  fclose(fp);
}
