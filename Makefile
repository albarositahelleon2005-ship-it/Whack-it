# =============================================================================
#  Whack it - compilacion desde la linea de comandos
# =============================================================================
#
#  Sirve en Windows, macOS y Linux, con las mismas tres ordenes. Es lo que usan
#  los botones de Visual Studio Code (.vscode/tasks.json). Quien prefiera
#  Code::Blocks sigue abriendo WhackIt.cbp y presionando F9: es el mismo
#  compilador por otro camino, no dos formas distintas de construir el juego.
#
#  ---- Uso, parado en la carpeta del proyecto ----
#
#      Windows                    macOS y Linux
#      mingw32-make               make             compila
#      mingw32-make ejecutar      make ejecutar    compila y abre el juego
#      mingw32-make limpiar       make limpiar     borra lo compilado
#
#  En Windows el programa se llama mingw32-make y no make: asi lo nombra el
#  MinGW que trae Code::Blocks. Vive en C:\Program Files\CodeBlocks\MinGW\bin,
#  que NO esta en el PATH del sistema; por eso las tareas de VS Code se lo
#  agregan al PATH antes de llamarlo, y por eso escribir "make" en una consola
#  recien abierta responde que no existe.
#
#  ---- Preparacion, una sola vez ----
#
#  Windows: nada. raylib viene dentro del repositorio, en libs/raylib, y el
#           compilador lo trae Code::Blocks 25.03.
#
#  macOS:   xcode-select --install            compilador de Apple
#           brew install raylib pkg-config    raylib para Mac
#
#           raylib tiene que ser 5.5 o mas nueva: el juego usa IsTextureValid y
#           DrawRectangleRoundedLinesEx, que no existen antes. Se revisa con
#           pkg-config --modversion raylib
#
# =============================================================================

NOMBRE = WhackIt

# wildcard agarra todos los .cpp de la carpeta. Se hace asi para que agregar un
# archivo al proyecto no obligue a acordarse de editar este Makefile: en
# Code::Blocks se agrega al .cbp y de este lado aparece solo.
FUENTES = $(wildcard *.cpp)

CXX = g++

# -g mete la informacion de depuracion dentro del ejecutable: que linea del
# codigo corresponde a que instruccion. Sin ella el depurador corre el programa
# pero no sabe donde va, y los puntos de interrupcion no se detienen en ningun
# lado. No hace el juego mas lento; solo mas pesado en disco.
CXXFLAGS = -std=c++17 -Wall -fexceptions -g -DPLATFORM_DESKTOP

# make define la variable OS con el valor Windows_NT cuando corre en Windows. Se
# usa eso para distinguir el sistema, y no uname como mas abajo, porque uname no
# existe en la consola de Windows: preguntarle daria vacio siempre.
ifeq ($(OS),Windows_NT)

    # raylib viene dentro del repositorio y se referencia con rutas relativas:
    # clonar y compilar, sin instalar nada aparte.
    CXXFLAGS += -Ilibs/raylib/include

    # El orden raylib -> opengl32 -> gdi32 -> winmm no es capricho. El enlazador
    # resuelve de izquierda a derecha: quien USA los simbolos va antes que quien
    # los PROVEE. Invertirlo da "undefined reference" con todo bien instalado.
    ENLACE = -Llibs/raylib/lib -lraylib -lopengl32 -lgdi32 -lwinmm

    # -static mete libgcc y libstdc++ dentro del .exe. Sin esto el juego solo
    # arranca donde este instalado MinGW: al abrirlo desde el explorador o desde
    # otra consola Windows no encuentra libstdc++-6.dll y lo unico que sale es un
    # cuadro de error. Con esta bandera el ejecutable se vale por si mismo, que
    # es lo mismo que hace el target Release del .cbp para el dia del rally.
    ENLACE += -static

    SALIDA = $(NOMBRE).exe
    CORRER = $(NOMBRE).exe

    # En Windows el recetario lo ejecuta cmd, asi que aqui se borra con del y no
    # con rm. El "if exist" evita el error cuando no hay nada que borrar.
    BORRAR = if exist $(NOMBRE).exe del $(NOMBRE).exe

else

    # pkg-config responde donde quedaron los encabezados y la biblioteca de
    # raylib. Se le pregunta a el en vez de escribir la ruta a mano porque
    # Homebrew instala en lugares distintos segun el Mac: /usr/local en los Intel
    # y /opt/homebrew en los Apple Silicon. Una ruta fija romperia en la mitad de
    # las maquinas.
    CXXFLAGS += $(shell pkg-config --cflags raylib)
    ENLACE    = $(shell pkg-config --libs raylib)

    SISTEMA := $(shell uname -s)

    ifeq ($(SISTEMA),Darwin)
        # macOS: raylib se apoya en estos servicios del sistema para la ventana,
        # el dibujo y la entrada. Van DESPUES de -lraylib por la misma regla de
        # izquierda a derecha que en Windows.
        ENLACE += -framework CoreVideo -framework IOKit -framework Cocoa -framework OpenGL
    else
        # Linux
        ENLACE += -lGL -lm -lpthread -ldl -lrt -lX11
    endif

    SALIDA = $(NOMBRE)
    CORRER = ./$(NOMBRE)
    BORRAR = rm -f $(NOMBRE)

endif

# Se recompila todo de un jalon en vez de archivo por archivo. Con estos fuentes
# tarda unos segundos y evita tener que explicar objetos intermedios.
$(SALIDA): $(FUENTES)
	$(CXX) $(CXXFLAGS) $(FUENTES) $(ENLACE) -o $(SALIDA)

# Se ejecuta desde la carpeta del proyecto, no desde donde quedo el binario: el
# juego busca sus imagenes en recursos/ relativo a donde se le llama.
ejecutar: $(SALIDA)
	$(CORRER)

limpiar:
	$(BORRAR)

# Estos dos nombres son acciones, no archivos. Sin esta linea, el dia que exista
# un archivo llamado 'limpiar' make creeria que ya esta hecho y no haria nada.
.PHONY: ejecutar limpiar
