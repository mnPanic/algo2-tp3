# Para los tests

## Compilación

1. `mkdir build`
2. `cd build`
3. `cmake ..`
4. `make`

## Ejecución

Desde el inicio del proyecto:

1. `cd build`
2. `./bin/correrTests


# Para la interfaz gráfica

## Dependencias

Se necesitan dependencias que pueden ser instaladas en Ubuntu de la siguiente
manera:

    sudo apt install libboost-iostream-dev libboost-filesystem-dev libsdl2-dev libsdl2-image-dev

## Compilación

Si ya se armó la carpeta `build`, es necesario borrarla:

    rm -r build

Luego:

1. `mkdir build`
2. `cd build`
3. `cmake -DWITH-GUI=ON ..`
4. `make`

## Ejecución

Desde el inicio del proyecto:

1. `cd build/bin`
2. `./ExtremeExorcism`

# Sobre la interfaz gráfica

El mapa se obtiene a partir del archivo `resources/maps/m1.txt` donde los `.` son los espacios
en blanco y los `#` son las paredes. El archivo puede modificarse para crear mapas nuevos. Es necesario recompilar el 
juego para que se actualice. Alternativamente se puede copiar el archivo de texto a `build/bin/resources/maps`.

En la versión actual, el juego soporta un solo jugador que se mueve son la siguiente disposición de teclas:
```
  I  
J K L
```

Y dispara con la tecla `U`.
Se puede salir del juego con la tecla `Q`.

