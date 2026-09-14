/**
 * \file Menu.cpp
 * \brief Implementación del menú principal.
 * \date 13/09/2026
 */

#include "raylib.h"

#include "Menu.hpp"
#include "Boton.hpp"
#include "Dibujo.hpp"
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

// Cuanto se recorren las opciones desde el borde izquierdo de la ventana.
static const int MARGEN_IZQUIERDO = 120;

// 'static' a nivel de archivo: esta variable solo existe dentro de Menu.cpp.
// main.cpp no sabe -ni tiene por que saber- cual opcion esta resaltada.
static int opcionSeleccionada = 0;

/**
 * \brief El area clicable de una opcion del menu.
 *
 * Es mas grande que el texto en si (ver DibujarMenu) a proposito: asi el
 * jugador no tiene que apuntarle exacto a la letra para que el mouse la
 * cuente como "encima".
 * \param indice Opcion, desde cero.
 */
static Rectangle areaOpcion(int indice)
{
    return rectangulo((float)MARGEN_IZQUIERDO - 10.0f,
                       260.0f + indice * 60.0f - 8.0f,
                       260.0f, 46.0f);
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
    dibujarTextoCentrado("WHACK IT!", 90, 80, COLOR_TITULO);

    for(int i = 0; i < NUM_OPCIONES; i++){

        // Lo unico que distingue a la opcion resaltada es como se dibuja. No
        // hay que guardar ningun estado extra: se decide aqui, en el momento
        // de dibujar. Eso es el modo inmediato.
        bool seleccionada = (i == opcionSeleccionada);

        Color color   = seleccionada ? COLOR_SELECCION : COLOR_TEXTO;
        int   tamano  = seleccionada ? 36 : 30;
        int   y       = 260 + i * 60;

        // A diferencia del texto centrado, aqui todas las opciones arrancan
        // en la misma columna (MARGEN_IZQUIERDO). Solo la resaltada lleva el
        // ">" al frente, y como el ancho cambia entre "Jugar" y "> Jugar" se
        // dibuja con DrawText directo en vez de dibujarTextoCentrado, que
        // siempre calcula el centro de la ventana.
        const char* etiqueta = seleccionada
                             ? TextFormat("> %s", ETIQUETAS[i])
                             : ETIQUETAS[i];

        DrawText(etiqueta, MARGEN_IZQUIERDO, y, tamano, color);
    }

    dibujarTextoCentrado("Flechas o mouse para moverte     Enter o clic para elegir", 660, 20, COLOR_TENUE);
}
