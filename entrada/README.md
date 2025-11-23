# Generador

El archivo "generador_v1.c" contiene el codigo correspondiente para generar un laberinto de 13x13 casillas y luego exporetarlo como un archivo de texto llamado "mapa.txt".

Las casillas en el laberinto son:
- 0: Casilla libre.
- 1: Casilla con bloque destructible.
- 2: Casilla con bloque indestructible
- 3: Casilla cual contiene el primer tanque.
- 4: Casilla cual contiene el segundo tanque.
- 5: Casilla cual contiene una vida.

El laberinto se genera a traves de un disposicion de casillas aleatorias para luego ser verificado por el algoritmo DFS.

En caso que el algoritmo considere que el laberinto no es valido, el laberinto se descarta y se genera otro. Asi sucesivamente hasta tener un laberinto valido.

El algoritmo DFS o Busqueda de Profundidad. Es una algoritmo el cual recorre todos los nodos de un arbol (en este caso el laberinto) de manera ordenada. En este caso se utiliza para verificar que todas las casillas libres esten conectadas entre si o tengan una casilla con un bloque destructible. 

Se utiiza este algoritmo para evitar la generacion de un laberinto el cual puede contener casillas libres bloqueadas por bloques indestructibles.


# Cargar:

El archivo "cargar_v1.c" simplemente lee el archivo de texto el cual contiene el mapa y lo imprime.
