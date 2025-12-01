SE NECESITA EL SDL MIXER PARA LOS AUDIOS

Para compilar:

 gcc fusion.c logica.c audio.c inicio.c -o juego.exe \
 $(pkg-config --cflags --libs sdl2 SDL2_image SDL2_ttf)

 linux: 
 
gcc fusion.c logica.c audio.c inicio.c -o juego $(pkg-config --cflags --libs sdl2) $(pkg-config --cflags --libs SDL2_image) $(pkg-config --cflags --libs SDL2_ttf) $(pkg-config --cflags --libs SDL2_mixer)
