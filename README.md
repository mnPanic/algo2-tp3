# algo2-tp3

Algoritmos y Estructuras de datos II.

TP 3 - Exorcismo Extremo

## Integrantes

Nombre                  | LU        | Mail
------------------------|-----------|-------------------------
Rosinov, Gaston Einan   | 37/18     | grosinov@gmail.com
Schuster, Martin Ariel  | 208/18    | m.a.schuster98@gmail.com
Panichelli, Manuel      | 72/18     | panicmanu@gmail.com

## Disclaimer

Pasan todos los tests, pero al correrlos con Valgrind, presentan un leak a la hora de crear un `LocalizadorEstatico`.

Como esto es "tema de la cátedra" suponemos que no es problema.

## Correr interfaz gráfica en Mac

```bash
## Dependencias
brew install cmake
brew install sdl2
brew install boost
brew install sdl2_image
# No se si es necesario
brew install sdl2_ttf

## Buildear
cd Solucion
# Solo si ya existe el directorio build/
rm -r build
mkdir build
cd build
cmake -DWITH_GUI=ON ..
make
./ExtremeExorcism
```
