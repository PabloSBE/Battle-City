#ifndef LOGICA_H
#define LOGICA_H

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

typedef struct {
    int fila, columna;
    int vida;
    int direccion;   // 0=arriba, 1=derecha, 2=abajo, 3=izquierda
} Tanque;

typedef struct {
    int **mapa;
    Tanque jugador1;
    Tanque jugador2;
} Juego;

/* ----- Manejo general ----- */
void inicializar_juego(Juego *juego);
void actualizar_estado(Juego *juego);

/* ----- Movimientos ----- */
int mover_tanque(Juego *juego, Tanque *t, int jugador);

/* ----- Disparo ----- */
void disparar(Juego *juego, Tanque *t);
void actualizar_balas(Juego *juego);


/* ----- Fin del juego ----- */
int juego_terminado(Juego *juego);

#endif
