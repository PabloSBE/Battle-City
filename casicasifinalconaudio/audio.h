#ifndef AUDIO_H
#define AUDIO_H

#include <SDL2/SDL_mixer.h>

extern Mix_Chunk* snd_mover;
extern Mix_Chunk* snd_disparo;
extern Mix_Chunk* snd_explosion;
extern Mix_Chunk* snd_vida;
extern Mix_Chunk* snd_daño;

extern Mix_Music* musica_fondo;   
int cargar_audio();              
void liberar_audio();       

#endif