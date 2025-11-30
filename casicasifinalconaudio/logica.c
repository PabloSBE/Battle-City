#include "logica.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include "audio.h"

#define LARGO 13
#define ANCHO 13

// Dibujar textos
void dibujar_texto(SDL_Renderer* r, TTF_Font* f, const char* msg, int x, int y){

    SDL_Color blanco = {255, 255, 255};

    SDL_Surface* s = TTF_RenderText_Solid(f, msg, blanco);
    SDL_Texture* t = SDL_CreateTextureFromSurface(r, s);

    SDL_Rect rect = {x, y, s->w, s->h};
    SDL_RenderCopy(r, t, NULL, &rect);

    SDL_FreeSurface(s);
    SDL_DestroyTexture(t);
}

// Dibujar botones
void dibujar_boton(SDL_Renderer *render, TTF_Font *fuente, Boton *boton) {
    // Generar el rectangulo del boton
    SDL_SetRenderDrawColor(render, boton->color.r, boton->color.g, boton->color.b, boton->color.a);
    SDL_RenderFillRect(render, &boton->rect);

    // Generar el borde del boton
    SDL_SetRenderDrawColor(render, 255, 255, 255, 255); // Borde blanco
    SDL_RenderDrawRect(render, &boton->rect);

    // Posicionar el texto en el boton
    int texto_w, texto_h;
    TTF_SizeText(fuente, boton->texto, &texto_w, &texto_h);
    
    int texto_x = boton->rect.x + (boton->rect.w - texto_w) / 2;
    int texto_y = boton->rect.y + (boton->rect.h - texto_h) / 2;

    // Utilizar la funcion dibujar_texto para colocar el texto en el boton
    dibujar_texto(render, fuente, boton->texto, texto_x, texto_y);
}

// Dibujar hud
void dibujar_hud(SDL_Renderer *render, TTF_Font *fuente, Juego *juego){

    int hudY = 832;
    int hudH = 120;

	  //Rellena el fondo del hud
    SDL_Rect hudRect = {0, hudY, 832, hudH};
    SDL_SetRenderDrawColor(render, 20, 20, 20, 255);
    SDL_RenderFillRect(render, &hudRect);

    char texto[64];

    //Muestra las vidas y disparos del tanque 1
    sprintf(texto, "T1 Vidas: %d", juego->jugador1.vida);
    dibujar_texto(render, fuente, texto, 20, hudY + 20);

    sprintf(texto, "T1 Balas: %d", juego->jugador1.disparos);
    dibujar_texto(render, fuente, texto, 20, hudY + 60);

    //Muestra las vidas y disparos del tanque 2
    sprintf(texto, "T2 Vidas: %d", juego->jugador2.vida);
    dibujar_texto(render, fuente, texto, 832 - 340, hudY + 20);

    sprintf(texto, "T2 Balas: %d", juego->jugador2.disparos);
    dibujar_texto(render, fuente, texto, 832 - 340, hudY + 60);
}

// Dibujar menu de inicio
int dibujar_menu(SDL_Renderer *render, TTF_Font *fuente, TTF_Font *fuenteBoton, Juego *juego) {
  SDL_Event e;

  // Crear el boton de iniciar nueva partida con sus caracteristicas
  Boton btnNuevo;
  btnNuevo.rect = (SDL_Rect) {290, 225, 250, 75}; 
  btnNuevo.color = (SDL_Color) {0, 150, 0, 255};
  sprintf(btnNuevo.texto, "Nueva Partida");

  // Crear el boton de cargar partida con sus caracteristicas
  Boton btnCargar;
  btnCargar.rect = (SDL_Rect) {290, 350, 250, 75};
  btnCargar.color = (SDL_Color) {0, 0, 150, 255};
  sprintf(btnCargar.texto, "Cargar Partida");

  // Crear el boton de salir con sus caracteristicas
  Boton btnSalir;
  btnSalir.rect = (SDL_Rect) {290, 475, 250, 75};
  btnSalir.color = (SDL_Color) {150, 0, 0, 255};
  sprintf(btnSalir.texto, "Salir");

  // El programa queda en un bucle hasta que el usuario eliga alguna opcion
  while(1) {
      while(SDL_PollEvent(&e)) {

        // Detectar click
        if(e.type == SDL_MOUSEBUTTONDOWN) {
          // Detectar solo click izquierda
          if(e.button.button == SDL_BUTTON_LEFT) {
            // Obtener coordenadas x, y del mouse
            int x = e.button.x; 
            int y = e.button.y;

            // Verificar si el click ocurrio en las coordenadas del boton de nueva partida
            if(click_boton(&btnNuevo, x, y)) {
              return 1;
            }

            // Verificar si el click ocurrio en las coordenadas del boton de cargar
            if(click_boton(&btnCargar, x, y)) {
              return 2; 
            }

            // Verificar si el click ocurrio en las coordenadas del boton de sakur
            if(click_boton(&btnSalir, x, y)) {
              return 0;
            }
          }
        }
      }

    // Renderizar un fondo negro
    SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
    SDL_RenderClear(render);
    
    // Renderizar el texto "Battle City"
    dibujar_texto(render, fuente, "Battle City", 270, 100);

    // Renderizar los botones con sus respectivas fuentes
    dibujar_boton(render, fuenteBoton, &btnNuevo);
    dibujar_boton(render, fuenteBoton, &btnCargar);
    dibujar_boton(render, fuenteBoton, &btnSalir);

    // Actualizar la ventana
    SDL_RenderPresent(render);
  }
}

// Dibujar pantalla de fin
void dibujar_fin(SDL_Renderer *render, TTF_Font *fuente, Juego *juego) {
  // Renderizar un fondo negro
  SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
  SDL_RenderClear(render);

  // Mensaje de maximo 50 caracteres
  char mensaje[30];

  // Dependiendo de que jugador gana, el mensaje es uno otro
  if(juego->jugador1.vida <= 0)
    sprintf(mensaje, "Jugador 2 Gana");
  else 
    sprintf(mensaje, "Jugador 1 Gana");

  // Renderizar los textos de la pantalla de fin
  dibujar_texto(render, fuente, "Fin Del Juego", 235, 125);
  dibujar_texto(render, fuente, mensaje, 210, 350);
  dibujar_texto(render, fuente, "Presione ESC para salir", 100, 455);
}

// Verificacion coordenadas de boton
int click_boton(Boton *boton, int x, int y) {
  // Verificar que el click se haya hecho dentro del boton
  // Se verifica que las coordenadas del click esten dentro de las coordenadas del boton
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

	//crea una posicion aleatoria
    int x = rand() % 13;
    int y = rand() % 13;

	//Si la zona esta vacia coloca la vida
    if(juego->mapa[x][y] != 2 && juego->mapa[x][y] != 3 && juego->mapa[x][y] != 4)
      juego->mapa[x][y] = 9;
}


// Actualiza balas
void actualizar_estado(Juego *juego) {
    actualizar_balas(juego);
}

// Movimiento del tanque
int mover_tanque(Juego *juego, Tanque *t, int jugador) {

    //Si la posicion esta vacia, mueve el tanque hacia Arriba
    if (t->direccion==0 && juego->mapa[(t->fila)-1][t->columna]==VACIO){
        juego->mapa[t->fila][t->columna]=VACIO;
        t->fila -= 1;
        juego->mapa[t->fila][t->columna]=jugador;
        return 1;

	//Si hay una vida la recoge
    }else if (t->direccion==0 && juego->mapa[(t->fila)-1][t->columna]==VIDA){
        Mix_PlayChannel(-1, snd_vida, 0);
        juego->mapa[t->fila][t->columna]=VACIO;
        t->fila -= 1;
        juego->mapa[t->fila][t->columna]=jugador;
        t->vida+=1;
        return 1;
    } 

    //Si la posicion esta vacia, mueve el tanque hacia la Derecha
    else if (t->direccion==1 && juego->mapa[t->fila][(t->columna)+1]==VACIO){
        juego->mapa[t->fila][t->columna]=VACIO;
        t->columna += 1;
        juego->mapa[t->fila][t->columna]=jugador;
        return 1;

	//Si hay una vida la recoge
    }else if (t->direccion==1 && juego->mapa[t->fila][(t->columna)+1]==VIDA){
        Mix_PlayChannel(-1, snd_vida, 0);
        juego->mapa[t->fila][t->columna]=VACIO;
        t->columna += 1;
        juego->mapa[t->fila][t->columna]=jugador;
        t->vida+=1;
        return 1;
    }

    //Si la posicion esta vacia, mueve el tanque hacia Abajo
    else if (t->direccion==2 && juego->mapa[(t->fila)+1][t->columna]==VACIO){
        juego->mapa[t->fila][t->columna]=VACIO;
        t->fila += 1;
        juego->mapa[t->fila][t->columna]=jugador;
        return 1;

	//Si hay una vida la recoge
    }else if (t->direccion==2 && juego->mapa[(t->fila)+1][t->columna]==VIDA){
        Mix_PlayChannel(-1, snd_vida, 0);
        juego->mapa[t->fila][t->columna]=VACIO;
        t->fila += 1;
        juego->mapa[t->fila][t->columna]=jugador;
        t->vida+=1;
        return 1;
    }

    //Si la posicion esta vacia, mueve el tanque hacia la Izquierda
    else if (t->direccion==3  && juego->mapa[t->fila][(t->columna)-1]==VACIO){
        juego->mapa[t->fila][t->columna]=VACIO;
        t->columna -= 1;
        juego->mapa[t->fila][t->columna]=jugador;
        return 1;

	//Si hay una vida la recoge
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

/* Funcion que genera una bala en la direccion que este apuntando el tanque, la bala queda ubicada una posicion delante
del tanque. Se anaden varias condiciones en caso de que enfrente del tanque haya un bloque u objeto */
void disparar(Juego *juego, Tanque *t) {

	//anade un disparo al contador
    t->disparos+=1;

    Mix_PlayChannel(-1, snd_disparo, 0);
	
    //Disparo hacia arriba
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

    //Disparo hacia la derecha
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

    //Disparo hacia abajo
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

    //Disparo hacia la izquierda
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

/* Funcion encargada de actualizar la posicion de las balas. Primero lee el mapa para encontrar las balas generadas,
una vez obtenida las coordenadas, la funcion mueve una posicion la bala hasta que esta se encuentre con algun objeto. */
void actualizar_balas(Juego *juego) {

    int mapa_nuevo[ALTO][ANCHO];

    //Crea un nuevo mapa que contendra la posicion actualizada de las balas
    for(int i = 0; i < ALTO; i++) {
        for(int j = 0; j < ANCHO; j++) {
            mapa_nuevo[i][j] = juego->mapa[i][j];
        }
    }

	//Ciclo para buscar las balas
    for(int i = 0; i < ALTO; i++) {
        for(int j = 0; j < ANCHO; j++) {

			/* Mueve las balas una posicion.
			En caso de haber un bloque destruible adyacente, este se destruira junto con la bala.
			En caso de haber un bloque indestructible adyacente, la bala se destruira.
			En caso de haber un tanque adyacente, a este se le restara una vida
			Esto se aplica a cada direccion de la bala*/
			
			// Interacciones Bala arriba
            if(juego->mapa[i][j]==BALA_ARRIBA){
                if(juego->mapa[(i)-1][j]==VACIO){
                    mapa_nuevo[i][j]=VACIO;
                    mapa_nuevo[(i)-1][j]=BALA_ARRIBA;
                }
                else if(juego->mapa[(i)-1][j]==INDESTRUCTIBLE){
                    mapa_nuevo[i][j]=VACIO;
                }
                else if(juego->mapa[(i)-1][j]==DESTRUCTIBLE){
                    mapa_nuevo[i][j]=VACIO;
                    mapa_nuevo[(i)-1][j]=VACIO;
                    Mix_PlayChannel(-1, snd_explosion, 0);
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
		                mapa_nuevo[i-1][j]=VACIO;
		                mapa_nuevo[(i)-2][j]=VACIO;
                    Mix_PlayChannel(-1, snd_explosion, 0);
		            }
		        }

            }

			// Interacciones Bala derecha
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
                    Mix_PlayChannel(-1, snd_explosion, 0);
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
                    mapa_nuevo[i][j+1]=VACIO;
		                mapa_nuevo[i][j+2]=VACIO;
                    Mix_PlayChannel(-1, snd_explosion, 0);
		          }
		        }

            } 

			// Interacciones Bala abajo
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
                    Mix_PlayChannel(-1, snd_explosion, 0);
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
                    mapa_nuevo[i+1][j]=VACIO;
		            mapa_nuevo[(i)+2][j]=VACIO;
                Mix_PlayChannel(-1, snd_explosion, 0);
		            }
		        }

            }

			// Interacciones Bala Izquierda
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
                    Mix_PlayChannel(-1, snd_explosion, 0);
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
                    mapa_nuevo[i][j-1]=VACIO;
		            mapa_nuevo[i][j-2]=VACIO;
                Mix_PlayChannel(-1, snd_explosion, 0);
		          }
		        }

            } 
        }
    }

	//Copia la nueva posicion de las balas al mapa orignial
    for(int i = 0; i < ALTO; i++) {
        for(int j = 0; j < ANCHO; j++) {
            juego->mapa[i][j] = mapa_nuevo[i][j];
        }
    }
}


// Retorna 1 si algun jugador se queda sin vidas, retorna 0 en caso contrario
int juego_terminado(Juego *juego) {

	
    if(juego->jugador1.vida<=0){
        return 1;
    }

    if(juego->jugador2.vida<=0){
        return 1;
    }

    return 0;
}

// Funcion encargada de liberar la memoria que ocupa el mapa
void liberar_memoria(int **mapa) {
  // Se verifica que haya un mapa
  if(mapa == NULL)
    return;

  // Liberar memoria de cada fila
  for(int i = 0; i < ANCHO; i++) {
    free(mapa[i]);
  }

  // Se libera meoria de cada columna
  free(mapa);
}

// Funcion que implementa el algoritmo DFS
void dfs(int x, int y, int **mapa, int visitado[ANCHO][LARGO]) {

  // Evitar errores en caso que se salga de los limites del mapa
  if(x < 0 || x >= ANCHO || y < 0 || y >= LARGO)
    return;

  // Verificar que se haya verificado esta celda de manera anterior
  if(visitado[x][y])
    return;

  // Verificar que sea un bloque indestructible
  if(mapa[x][y] == 2)
    return;

  // Si la celda no ha sido visitada y no es indestructible, se marca como visitada
  visitado[x][y] = 1;

  // Se verifica lo mismo pero con las celdas adyacentes a este
  dfs(x + 1, y, mapa, visitado);
  dfs(x - 1, y, mapa, visitado);
  dfs(x, y + 1, mapa, visitado);
  dfs(x, y - 1, mapa, visitado);
}

// Esta funcion verifica que el mapa generado sea accesible por los jugadores
int conectados(int **mapa) {
  // Matriz para llevar el registro de las celdas visitadas
  int visitado[ANCHO][LARGO] = {0};

  int x = -1, y = -1;

  // Empezamos en la primera celda que no sea indestructible
  for(int i = 1; i < ANCHO - 1; i++) {
    // El bucle se termina hasta que se encuentre la primera celda libre
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

  // Verifica la validez del mapa
  if(x == -1)
    return 0;

  dfs(x, y, mapa, visitado);

  // Se recorre todo el mapa para verificar la existencia de celdas que no se puedan recorrer
  for(int i = 1; i < ANCHO - 1; i++) {
    for(int j = 1; j < LARGO - 1; j++) {
      if(mapa[i][j] != 2 && !visitado[i][j]) {
        return 0;
      }
    }
  }

  return 1;
}

// Funcion encargada de generar la estructura basica de un mapa (Filas, Columnas)
int** generar_mapa() {
  srand(time(NULL));

  // Reservar las filas del mapa
  int **mapa = (int**)malloc(ANCHO * sizeof(int*));

  // Verificar que se haya reservado de manera correcta la memoria dinamica
  if(mapa == NULL) {
    // printf("No se ha podido asignar memoria para las filas del mapa \n");
    return NULL;
  }

  // Reservar las columnas del mapa
  for(int i = 0; i < ANCHO; i++) {
    mapa[i] = (int*)malloc(LARGO * sizeof(int));

    // Verificar que se haya reservado de manera correcta las columnas del mapa
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
      // Verificar que estamos las celdas en el borde del mapa
      if(i == 0 || i == ANCHO - 1|| j == 0 || j == LARGO - 1) { 
        mapa[i][j] = 2;
      }
      else {
        mapa[i][j] = 0;
      }
    }
  }
    
  // Generar de manera aleatoria los obstaculos, ignorando las celdas que estan en los bordes
  for(int i = 1; i < LARGO - 1; i++) {
    for(int j = 1; j < ANCHO - 1; j++) {
      // Generar un numero aleatorio entre 1 - 10
      int probabilidad = (rand() % (10 - 1 + 1) + 1);

      // 20% de que se genere un bloque indestructible
      if(probabilidad > 6 && probabilidad <= 8) {
        mapa[i][j] = 2;
      }

      // 20% de que se genere un bloque destructible
      else if(probabilidad > 8) {
        mapa[i][j] = 1;
      }
    }
  }

  return mapa;
}

// Funcion encargada de colocar la posicion de los tanques en el mapa
void colocar_tanques(int **mapa) {
  // Se inicializan las coordenadas de cada tanque
  int tanque1_x, tanque1_y, tanque2_x, tanque2_y;

  // Hasta que el tanque1 no tenga coordenadas, el bucle se va a ejectuar indefinidamente
  while(1) {
    // Se generan las coordenadas del tanque1 de manera aleatoria
    tanque1_x = (rand() % (12 - 1 + 1) + 1); 
    tanque1_y = (rand() % (12 - 1 + 1) + 1);

    // Se verifica que las coordenadas generadas no tenga un bloque indestructible
    if(mapa[tanque1_x][tanque1_y] != 2){
      mapa[tanque1_x][tanque1_y] = 3; // Se genera el tanque 1
      break;
    }
  }

  // Hasta que el tanque1 no tenga coordenadas, el bucle se va a ejectuar indefinidamente
  while(1) {
    // Se generan las coordenadas del tanque2 de manera aleatoria
    tanque2_x = (rand() % (12 - 1 + 1) + 1);
    tanque2_y = (rand() % (12 - 1 + 1) + 1);

    // Se verifica que las coordenadas generadas no tenga un bloque indestructible
    // Y que no se encuentre a menos de 7 bloques del otro tanque
    if(mapa[tanque2_x][tanque2_y] != 2 && (abs(tanque2_x - tanque1_x) + abs(tanque2_y - tanque1_y)) >= 7){
      mapa[tanque2_x][tanque2_y] = 4; // Se genera el tanque 2
      break;
    }
  }
}


void generar_archivo_mapa() {

  int **mapa;
  int cantidad_mapas = 0;

  FILE *fp;

  // Este bucle se repite de forma indefinida hasta que se genere un mapa el cual no tenga zonas cerrdas
  // Las cuales el jugador no puede alcanzar.
  // En caso que se gener un mapa invalido, se descarta y se genera otro
  do {
    // Se libera de memoria mapa, en caso que se haya generado un mapa invalido anteriormente
    if(cantidad_mapas == 1)
      liberar_memoria(mapa);

    // Se genera la estructura basica del mapa
    mapa = generar_mapa();
    cantidad_mapas = 1; // Se marca que ya se asigno memoria al mapa

    // Verificacion en caso que la asignacion de memoria dinamica haya fallado
    if(mapa == NULL) {
      return;
    }

    // Verificamos que todas las celdas del mapa sean accesibles
    if(conectados(mapa)) {
      colocar_tanques(mapa); // Se generan los tanques en el mapa
      break;
    }

  } while(1); 

  /*
  for(int i = 0; i < LARGO; i++) {
    for(int j = 0; j < ANCHO; j++) {
      printf("%d ", mapa[i][j]);
    }

    printf("\n");
  }
  */

  // Abrimos el archivo mapa.txt
  fp = fopen("mapa.txt", "w");

  // Verificamos que el archivo se haya abierto de manera correcta
  if(fp == NULL) {
    printf("Ha habido un problema al abrir el archivo");
    return;
  }

  // Guardamos el mapa en el archivo mapa.txt
  for(int i = 0; i < LARGO; i++) {
    for(int j = 0; j < ANCHO; j++) {
      fprintf(fp, "%d ", mapa[i][j]);
    }

    fprintf(fp, "\n");
  }

  // Cerramos el archivo
  fclose(fp);
  liberar_memoria(mapa); // Liberamos la memoria ram que estaba utilizando el mapa
}

// Funcion encargada de guardar el mapa de un partida ya en curso (Guardar partida)
void generar_guardado_mapa(int **mapa) {
  FILE *fp;

  // Se abre el archivo partida.txt donde se va a guardar el mapa
  fp = fopen("partida.txt", "w");

  // Se verifica que el archivo se haya abierto de forma correcta
  if(fp == NULL) {
    printf("Ha habido nu problema al guardar el mapa");
    return;
  }

  // Se guarda cada elemento del mapa en el archivo
  for(int i = 0; i < LARGO; i++) {
    for(int j = 0; j < ANCHO; j++) {
      fprintf(fp, "%d ", mapa[i][j]);
    }

    fprintf(fp, "\n");
  }

  // Se cierra el archivo
  fclose(fp);
}
