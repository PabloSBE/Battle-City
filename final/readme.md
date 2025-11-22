Para compilar:

gcc fusion.c logica.c -o juego.exe $(pkg-config --cflags --libs sdl2 SDL2_image)
