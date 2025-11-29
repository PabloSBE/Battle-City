#include "audio.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

Mix_Chunk* snd_mover;
Mix_Chunk* snd_disparo;
Mix_Chunk* snd_explosion;
Mix_Chunk* snd_vida;
Mix_Chunk* snd_daño;

Mix_Music* musica_fondo;


int cargar_audio() {               
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("Error al iniciar SDL_mixer: %s\n", Mix_GetError());
        return 0;
    }

    snd_mover=Mix_LoadWAV("mover.wav");
    snd_disparo=Mix_LoadWAV("disparo.wav");
    snd_explosion=Mix_LoadWAV("explosion.wav");
    snd_vida=Mix_LoadWAV("vida.wav");
    snd_daño=Mix_LoadWAV("daño.wav");

    if (!snd_mover || !snd_disparo || !snd_explosion || !snd_vida || !snd_daño) {
        printf("Error cargando sonidos: %s\n",Mix_GetError());
        return 0;
    }
    Mix_VolumeChunk(snd_mover,28);       // volumen del movimiento
    Mix_VolumeChunk(snd_disparo,95);    // volumen del disparo
    Mix_VolumeChunk(snd_explosion,90);  // volumen de la explosión
    Mix_VolumeChunk(snd_vida,80);        // volumen de recogida de vida
    Mix_VolumeChunk(snd_daño,90);        // volumen del daño

    srand(time(NULL));
    int cancion=rand()%5+1;
    char archivo[50];
    sprintf(archivo,"musica%d.mp3",cancion);
    musica_fondo=Mix_LoadMUS(archivo);
    if (!musica_fondo){
        printf("Error cargando música: %s\n",Mix_GetError());
        return 0;
    }
    Mix_VolumeMusic(35);  // volumen de la música (0-128)
    Mix_PlayMusic(musica_fondo,-1); // loop infinito
    return 1;
}

void liberar_audio() {             
    Mix_FreeChunk(snd_mover);
    Mix_FreeChunk(snd_disparo);
    Mix_FreeChunk(snd_explosion);
    Mix_FreeChunk(snd_vida);
    Mix_FreeChunk(snd_daño);

    Mix_FreeMusic(musica_fondo);
    Mix_CloseAudio();
}