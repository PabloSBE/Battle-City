#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define LARGO 13
#define ANCHO 13

void liberar_memoria(int **mapa) {
  if(mapa == NULL)
    return;

  // Liberar memoria de cada fila
  for(int i = 0; i < ANCHO; i++) {
    free(mapa[i]);
  }

  free(mapa);
}

void dfs(int x, int y, int **mapa, int visitado[ANCHO][LARGO]) {
  if(x < 0 || x >= ANCHO || y < 0 || y >= LARGO)
    return;

  if(visitado[x][y])
    return;

  if(mapa[x][y] == 2)
    return;

  visitado[x][y] = 1;

  dfs(x + 1, y, mapa, visitado);
  dfs(x - 1, y, mapa, visitado);
  dfs(x, y + 1, mapa, visitado);
  dfs(x, y - 1, mapa, visitado);
}

int conectados(int **mapa) {
  int visitado[ANCHO][LARGO] = {0};

  int x = -1, y = -1;

  for(int i = 1; i < ANCHO - 1; i++) {
    for(int j = 1; j < LARGO - 1; j++) {
      if(mapa[i][j] != 2) {
        x = i;
        y = j;
        break;
      }
    }

    if(x != -1)
      break;
  }

  if(x == -1)
    return 0;

  dfs(x, y, mapa, visitado);

  for(int i = 1; i < ANCHO - 1; i++) {
    for(int j = 1; j < LARGO - 1; j++) {
      if(mapa[i][j] != 2 && !visitado[i][j]) {
        return 0;
      }
    }
  }

  return 1;
}

int** generar_mapa() {
  srand(time(NULL));

  int **mapa = (int**)malloc(ANCHO * sizeof(int*));

  if(mapa == NULL) {
    printf("No se ha podido asignar memoria para las filas del mapa \n");
    return NULL;
  }

  for(int i = 0; i < ANCHO; i++) {
    mapa[i] = (int*)malloc(LARGO * sizeof(int));

    if(mapa[i] == NULL) {
      printf("No se ha podido asignar memoria para las columnas del mapa \n");
      return NULL;
    }
  }

  // 0 = Espacio vacios
  // 1 = Bloque destructible
  // 2 = Bloque indestructible
  // 3 = Tanque 1
  // 4 = Tanque 2
  // 5 = Vida

  // Generamos un mapa vacio (celdas 0) y con un borde indestructible (celdas 2) 
  for(int i = 0; i < ANCHO; i++) {
    for(int j = 0; j < LARGO; j++) {
      if(i == 0 || i == ANCHO - 1|| j == 0 || j == LARGO - 1) { 
        mapa[i][j] = 2;
      }
      else {
        mapa[i][j] = 0;
      }
    }
  }
    
  for(int i = 1; i < LARGO - 1; i++) {
    for(int j = 1; j < ANCHO - 1; j++) {
      int probabilidad = (rand() % (10 - 1 + 1) + 1);


      if(probabilidad > 6 && probabilidad <= 8) {
        mapa[i][j] = 2;
      }

      else if(probabilidad > 8) {
        mapa[i][j] = 1;
      }
    }
  }

  return mapa;
}

void colocar_tanques(int **mapa) {
  int tanque1_x, tanque1_y, tanque2_x, tanque2_y;

  while(1) {
    tanque1_x = (rand() % (12 - 1 + 1) + 1);
    tanque1_y = (rand() % (12 - 1 + 1) + 1);

    if(mapa[tanque1_x][tanque1_y] != 2){
      mapa[tanque1_x][tanque1_y] = 3;
      break;
    }
  }

  while(1) {
    tanque2_x = (rand() % (12 - 1 + 1) + 1);
    tanque2_y = (rand() % (12 - 1 + 1) + 1);

    if(mapa[tanque2_x][tanque2_y] != 2 && (abs(tanque2_x - tanque1_x) + abs(tanque2_y - tanque1_y)) >= 7){
      mapa[tanque2_x][tanque2_y] = 4;
      break;
    }
  }
}

void colocar_vidas(int **mapa) {
  for(int i = 0; i < 3; i++) {
    int x = (rand() % (13 - 1 + 0) + 0);
    int y = (rand() % (13 - 1 + 0) + 0);

    printf("%d, %d \n", x, y);
  
    if(mapa[x][y] != 2 && mapa[x][y] != 3 && mapa[x][y] != 4)
      mapa[x][y] = 5;
  }
}

int main() {

  int **mapa;
  int cantidad_mapas = 0;

  FILE *fp;

  do {
    if(cantidad_mapas == 1)
      liberar_memoria(mapa);

    mapa = generar_mapa();
    cantidad_mapas = 1;

    if(mapa == NULL) {
      return 1;
    }

    if(conectados(mapa)) {
      colocar_tanques(mapa);
      colocar_vidas(mapa);
      break;
    }

  } while(1); 

  for(int i = 0; i < LARGO; i++) {
    for(int j = 0; j < ANCHO; j++) {
      printf("%d ", mapa[i][j]);
    }

    printf("\n");
  }

  fp = fopen("mapa.txt", "w");

  if(fp == NULL) {
    printf("Ha habido un problema al abrir el archivo");
    return 1;
  }

  for(int i = 0; i < LARGO; i++) {
    for(int j = 0; j < ANCHO; j++) {
      fprintf(fp, "%d ", mapa[i][j]);
    }

    fprintf(fp, "\n");
  }

  fclose(fp);
  liberar_memoria(mapa);

  return 0;
}
