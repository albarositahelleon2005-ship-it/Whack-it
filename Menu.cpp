/**
 * \file Menu.cpp
 * \brief Implementación del menú principal.
 * \date 13/09/2026
 */

#include "raylib.h"

#include "Menu.hpp"
#include "Boton.hpp"
#include "Dibujo.hpp"
#include "Iconos.hpp"
#include "Tema.hpp"

//***********************************************
// DATOS DEL MENU
//***********************************************

const int NUM_OPCIONES = 5;

// Las etiquetas y el destino de cada opcion se guardan en dos arreglos
// paralelos: ETIQUETAS[i] lleva a DESTINOS[i]. Agregar una opcion al menu es
// agregar dos lineas de datos y nada mas.

static const char* ETIQUETAS[NUM_OPCIONES] = {
    "Jugar",
    "Puntaje",
    "Instrucciones",
    "Creditos",
    "Salir"
};

static const Escena_Estado DESTINOS[NUM_OPCIONES] = {
    Escena_configuracion,
    Escena_puntajes,
    Escena_instrucciones,
    Escena_creditos,
    Escena_salir
};

// Donde arranca la columna de botones y cuanto se separan entre si. El
// tamano de cada boton lo fija Iconos.hpp, porque ahi se achican las imagenes.
static const int MARGEN_IZQUIERDO = 100;
static const int Y_PRIMERA_OPCION = 300;
static const int SEPARACION       = 72;

// 'static' a nivel de archivo: esta variable solo existe dentro de Menu.cpp.
// main.cpp no sabe -ni tiene por que saber- cual opcion esta resaltada.
static int opcionSeleccionada = 0;

/**
 * \brief El area clicable de una opcion del menu: el boton de madera completo.
 *
 * Es la misma zona donde DibujarMenu pone la imagen, asi que el clic cae
 * justo donde el jugador ve el boton.
 * \param indice Opcion, desde cero.
 */
static Rectangle areaOpcion(int indice)
{
    return rectangulo((float)MARGEN_IZQUIERDO,
                      (float)(Y_PRIMERA_OPCION + indice * SEPARACION),
                      (float)ANCHO_BOTON_INTRO, (float)ALTO_BOTON_INTRO);
}

//***********************************************
// MENU PRINCIPAL
//***********************************************

Escena_Estado ActualizarMenu()
{
    Rectangle areas[NUM_OPCIONES];
    for(int i = 0; i < NUM_OPCIONES; i++) areas[i] = areaOpcion(i);

    // Tres formas de mover la seleccion, todas actualizando la misma
    // variable: flechas, y el mouse si se mueve sobre otra opcion. Ninguna
    // sabe de la existencia de la otra, y no hace falta que lo sepan.
    moverSeleccion(opcionSeleccionada, NUM_OPCIONES, KEY_DOWN, KEY_UP);
    seguirRaton(areas, NUM_OPCIONES, opcionSeleccionada);

    // Confirmar es Enter/Espacio sobre la resaltada, o un clic -y un clic ya
    // dejo la opcion resaltada un instante antes, gracias a seguirRaton-.
    if(confirmado(areas[opcionSeleccionada])){
        return DESTINOS[opcionSeleccionada];
    }

    // Nadie ha elegido nada: nos quedamos donde estamos.
    return Escena_menu;
}

void DibujarMenu()
{
    dibujarFondo(fondoPrincipal());

    for(int i = 0; i < NUM_OPCIONES; i++){

        // Lo unico que distingue a la opcion resaltada es que imagen se usa:
        // la _P (prendida) para la resaltada y la _A (apagada) para las demas.
        // No hay que guardar ningun estado extra: se decide aqui, en el
        // momento de dibujar. Eso es el modo inmediato.
        bool      seleccionada = (i == opcionSeleccionada);
        Rectangle area         = areaOpcion(i);
        Texture2D boton        = botonIntro(i, seleccionada);

        if(boton.id != 0){
            DrawTexture(boton, (int)area.x, (int)area.y, WHITE);
        } else {
            // Si falta el PNG el menu sigue sirviendo: se cae al texto de antes.
            const char* etiqueta = seleccionada
                                 ? TextFormat("> %s", ETIQUETAS[i])
                                 : ETIQUETAS[i];
            dibujarTexto(etiqueta, (int)area.x + 10, (int)area.y + 12,
                     seleccionada ? 36 : 30,
                     seleccionada ? COLOR_SELECCION : COLOR_TEXTO);
        }
    }
}
