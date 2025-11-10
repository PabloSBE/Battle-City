#include <stdio.h>
#include <ctype.h>
#include "logica.h"

#define ALTO 13
#define ANCHO 13

// Función temporal para mostrar el mapa (solo debug)
void mostrar_mapa(Juego *j) {
    for(int i = 0; i < ALTO; i++) {
        for(int j2 = 0; j2 < ANCHO; j2++) {
            printf("%d ", j->mapa[i][j2]);
        }
        printf("\n");
    }
    printf("\n");
}

int main() {

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


    // ================================
    // Inicializar estado del juego
    // ================================
    inicializar_juego(&juego);

    // Mostrar mapa inicial
    printf("Mapa inicial:\n");
    mostrar_mapa(&juego);

    // ================================
    // Bucle principal
    // ================================
    char tecla;

    printf("Controles jugador 1: WASD\n");
    printf("Salir: q\n\n");

    while(1) {

        printf("Ingresa comando: ");
        scanf(" %c", &tecla);

        if(tecla == 'q')
            break;

        // ===== Movimiento jugador 1 =====
        if(tecla == 'w'){
            juego.jugador1.direccion = 0;
            mover_tanque(&juego, &juego.jugador1, 3);
        }
        if(tecla == 'd'){
            juego.jugador1.direccion = 1;
            mover_tanque(&juego, &juego.jugador1, 3);
        }
        if(tecla == 's'){
            juego.jugador1.direccion = 2;
            mover_tanque(&juego, &juego.jugador1, 3);
        }
        if(tecla == 'a'){
            juego.jugador1.direccion = 3;
            mover_tanque(&juego, &juego.jugador1, 3);
        }

        if(tecla == 'f'){
            disparar(&juego, &juego.jugador1);
        }
        /*
        if(tecla == 'i') mover_tanque(&juego, &juego.jugador2, 0);
        if(tecla == 'l') mover_tanque(&juego, &juego.jugador2, 1);
        if(tecla == 'k') mover_tanque(&juego, &juego.jugador2, 2);
        if(tecla == 'j') mover_tanque(&juego, &juego.jugador2, 3);
        */

        // Actualiza lógica global (balas, etc)
        actualizar_estado(&juego);

        // Mostrar mapa
        mostrar_mapa(&juego);
    }

    printf("Juego finalizado.\n");
    return 0;
}
