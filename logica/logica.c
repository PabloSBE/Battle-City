#include "logica.h"
#include <stdio.h>
#include <time.h>


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
                juego->jugador1.direccion = 0;
            }

            //Guarda la ubicacion del jugador 2 en el struct
            if(juego->mapa[i][j] == 4) {
                juego->jugador2.fila = i;
                juego->jugador2.columna = j;
                juego->jugador2.vida = 3;
                juego->jugador2.direccion = 0;
            }
        }
    }
    

}

/* Actualiza balas, movimientos automáticos, etc */
void actualizar_estado(Juego *juego) {
    actualizar_balas(juego);

}

/* Devuelve 1 si el tanque logró moverse, 0 si no */
int mover_tanque(Juego *juego, Tanque *t, int jugador) {

    //Arriba
    if (t->direccion==0 && juego->mapa[(t->fila)-1][t->columna]==VACIO){
        juego->mapa[t->fila][t->columna]=VACIO;
        t->fila -= 1;
        juego->mapa[t->fila][t->columna]=jugador;
        return 1;
    } 

    //Derecha
    else if (t->direccion==1 && juego->mapa[t->fila][(t->columna)+1]==VACIO){
        juego->mapa[t->fila][t->columna]=VACIO;
        t->columna += 1;
        juego->mapa[t->fila][t->columna]=jugador;
        return 1;
    } 

    //Abajo
    else if (t->direccion==2 && juego->mapa[(t->fila)+1][t->columna]==VACIO){
        juego->mapa[t->fila][t->columna]=VACIO;
        t->fila += 1;
        juego->mapa[t->fila][t->columna]=jugador;
        return 1;
    } 

    //Izquierda
    else if (t->direccion==3  && juego->mapa[t->fila][(t->columna)-1]==VACIO){
        juego->mapa[t->fila][t->columna]=VACIO;
        t->columna -= 1;
        juego->mapa[t->fila][t->columna]=jugador;
        return 1;
    }
    else{
        return 0;  
    }
}

/* Crea bala desde la posición del tanque */
void disparar(Juego *juego, Tanque *t) {

    if(t->direccion==0 && juego->mapa[(t->fila)-1][t->columna]==VACIO){
        juego->mapa[(t->fila)-1][t->columna]=BALA_ARRIBA;
    }



}

/* Actualiza posición de balas e interacciones */
void actualizar_balas(Juego *juego) {

    for(int i = 0; i < ALTO; i++) {
        for(int j = 0; j < ANCHO; j++) {

            //Colision ante cada tipo de elemento
            if(juego->mapa[i][j]==BALA_ARRIBA){
                if(juego->mapa[(i)-1][j]==VACIO){
                    juego->mapa[i][j]=VACIO;
                    juego->mapa[(i)-1][j]=BALA_ARRIBA;
                }
                else if(juego->mapa[(i)-1][j]==INDESTRUCTIBLE){
                    juego->mapa[i][j]=VACIO;
                }
                else if(juego->mapa[(i)-1][j]==DESTRUCTIBLE){
                    juego->mapa[i][j]=VACIO;
                    juego->mapa[(i)-1][j]=VACIO;
                }
                else if(juego->mapa[(i)-1][j]==TANQUE1){
                    juego->jugador1.vida-=1;
                }
                else if(juego->mapa[(i)-1][j]==TANQUE2){
                    juego->jugador2.vida-=1;
                }

            }





        }
    }


}


/* Retorna 1 si alguien ganó */
int juego_terminado(Juego *juego) {
    return 0;
}
