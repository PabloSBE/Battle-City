BATTLE CITY

Juego de tanques 1v1 en el que 2 jugadores, deben moverse por el mapa, destruir
obstaculos, y acabar con el tanque enemigo. El mapa contiene paredes
indestructibles, bloques destruibles y vidas consumibles que se irán regenerando.

Controles P1:

Movimiento: W A S D
Disparo: F


Controles P2:

Movimiento: (Flechas del teclado)
Disparo: L


Librerias necesarias para compilar:

SDL2
SDL2_image
SDL2_mixer
SDL2_ttf

Para compilar en Windows:

gcc fusion.c logica.c audio.c inicio.c -o juego.exe
$(pkg-config --cflags --libs sdl2 SDL2_image SDL2_ttf)


Para compilar en linux:

gcc fusion.c logica.c audio.c inicio.c -o juego.exe $(pkg-config --cflags --libs sdl2 SDL2_image SDL2_ttf SDL2_mixer)