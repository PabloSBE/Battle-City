#ifndef LOGICA_H
#define LOGICA_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define ALTO 13
#define ANCHO 13

// Códigos de celdas
#define VACIO 0
#define DESTRUCTIBLE 1
#define INDESTRUCTIBLE 2
#define TANQUE1 3
#define TANQUE2 4
#define BALA_ARRIBA 5
#define BALA_DERECHA 6
#define BALA_ABAJO 7
#define BALA_IZQUIERDA 8
#define VIDA 9  

typedef struct {
    SDL_Rect rect;
    SDL_Color color;
    char texto[50];
} Boton;

typedef struct {
    int fila, columna;
    int vida;
    int disparos;
    int direccion;   // 0=arriba, 1=derecha, 2=abajo, 3=izquierda
} Tanque;

typedef struct {
    int **mapa;
    Tanque jugador1;
    Tanque jugador2;
} Juego;

/* ----- Manejo general ----- */
void inicializar_juego(Juego *juego);

void dibujar_texto(SDL_Renderer* r, TTF_Font* f, const char* msg, int x, int y);
void dibujar_boton(SDL_Renderer *render, TTF_Font *fuente, Boton *boton);

void dibujar_hud(SDL_Renderer *render, TTF_Font *fuente, Juego *juego);
int dibujar_menu(SDL_Renderer *render, TTF_Font *fuente, TTF_Font *fuenteBoton, Juego *juego);
void dibujar_fin(SDL_Renderer *render, TTF_Font *fuente, Juego *juego);

int click_boton(Boton *boton, int x, int y);

void vidas(Juego *juego);
void actualizar_estado(Juego *juego);

/* ----- Movimientos ----- */
int mover_tanque(Juego *juego, Tanque *t, int jugador);

/* ----- Disparo ----- */
void disparar(Juego *juego, Tanque *t);
void actualizar_balas(Juego *juego);


/* ----- Fin del juego ----- */
int juego_terminado(Juego *juego);

/* ----- Generar Mapa ----- */
void generar_archivo_mapa();
void generar_guardado_mapa(int **mapa);

#endif
