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


void vidas(Juego *juego){

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
    else if(t->direccion==0 && juego->mapa[(t->fila)-1][t->columna]==DESTRUCTIBLE){
        juego->mapa[(t->fila)-1][t->columna]=VACIO;
    }

    else if(t->direccion==1 && juego->mapa[t->fila][(t->columna)+1]==VACIO){
        juego->mapa[t->fila][(t->columna)+1]=BALA_DERECHA;
    }
    else if(t->direccion==1 && juego->mapa[t->fila][(t->columna)+1]==DESTRUCTIBLE){
        juego->mapa[t->fila][(t->columna)+1]=VACIO;
    }

    else if(t->direccion==2 && juego->mapa[(t->fila)+1][t->columna]==VACIO){
        juego->mapa[(t->fila)+1][t->columna]=BALA_ABAJO;
    }
    else if(t->direccion==2 && juego->mapa[(t->fila)+1][t->columna]==DESTRUCTIBLE){
        juego->mapa[(t->fila)+1][t->columna]=VACIO;
    }

    else if(t->direccion==3 && juego->mapa[t->fila][(t->columna)-1]==VACIO){
        juego->mapa[t->fila][(t->columna)-1]=BALA_IZQUIERDA;
    }
    else if(t->direccion==3 && juego->mapa[t->fila][(t->columna)-1]==DESTRUCTIBLE){
        juego->mapa[t->fila][(t->columna)-1]=VACIO;
    }


}

/* Actualiza posición de balas e interacciones */
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
                    mapa_nuevo[i][j]=VACIO;
                    mapa_nuevo[(i)-1][j]=VACIO;
                }
                else if(juego->mapa[(i)-1][j]==TANQUE1){
                    juego->jugador1.vida-=1;
                    mapa_nuevo[i][j]=VACIO;
                }
                else if(juego->mapa[(i)-1][j]==TANQUE2){
                    juego->jugador2.vida-=1;
                    mapa_nuevo[i][j]=VACIO;
                }
                else{
                    mapa_nuevo[i][j]=VACIO;
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
                    juego->jugador1.vida-=1;
                    mapa_nuevo[i][j]=VACIO;
                }
                else if(juego->mapa[i][j+1]==TANQUE2){
                    juego->jugador2.vida-=1;
                    mapa_nuevo[i][j]=VACIO;
                }
                else{
                    mapa_nuevo[i][j]=VACIO;
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
                    juego->jugador1.vida-=1;
                    mapa_nuevo[i][j]=VACIO;
                }
                else if(juego->mapa[(i)+1][j]==TANQUE2){
                    juego->jugador2.vida-=1;
                    mapa_nuevo[i][j]=VACIO;
                }
                else{
                    mapa_nuevo[i][j]=VACIO;
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
                    juego->jugador1.vida-=1;
                    mapa_nuevo[i][j]=VACIO;
                }
                else if(juego->mapa[i][j-1]==TANQUE2){
                    juego->jugador2.vida-=1;
                    mapa_nuevo[i][j]=VACIO;
                }
                else{
                    mapa_nuevo[i][j]=VACIO;
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


/* Retorna 1 si alguien ganó */
int juego_terminado(Juego *juego) {

    if(juego->jugador1.vida<=0){
        int i = juego->jugador1.fila;
        int j = juego->jugador1.columna;

        juego->mapa[i][j]=VACIO;
        return 1;

    }

    if(juego->jugador2.vida<=0){
        int i = juego->jugador1.fila;
        int j = juego->jugador1.columna;

        juego->mapa[i][j]=VACIO;
        return 1;
    }
}
