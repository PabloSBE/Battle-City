#include <stdio.h>

#define LARGO 30

int main() {
  FILE *fp;

  fp = fopen("mapa.txt", "r");
  char linea[LARGO];

  while(fgets(linea, LARGO, fp)) {
    printf("%s", linea);
  }

  fclose(fp);

  return 0;
}
