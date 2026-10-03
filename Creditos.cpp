/**
 * \file Creditos.cpp
 * \brief Implementación de la pantalla de créditos.
 * \date 06/09/2026
 */

#include "raylib.h"

#include "Creditos.hpp"
#include "Dibujo.hpp"
#include "Iconos.hpp"
#include "Tema.hpp"

// Solo ASCII: la fuente no trae acentos ni enes (ver CLAUDE.md), por eso
// "Ninez", "Cientifica" y "Computacion" van sin ellos. El parrafo va cortado a
// mano y no con un ajuste automatico: son cuatro renglones fijos, y asi no se
// meten debajo de las nubes de los lados.
//
// El nombre del evento va en naranja, y empieza a media linea: por eso cada
// renglon se parte en dos pedazos, el normal y el naranja (cualquiera de los
// dos puede ir vacio).
static const int NUM_RENGLONES_INTRO = 4;
static const char* INTRO_NORMAL[NUM_RENGLONES_INTRO] = {
    "Este juego fue realizado para el ",
    "",
    "por estudiantes de la Licenciatura en",
    "Ciencias de la Computacion"
};
static const char* INTRO_NARANJA[NUM_RENGLONES_INTRO] = {
    "Rally de la",
    "Ninez Cientifica y Expo Profesiones STEM 2026",
    "",
    ""
};

static const int NUM_DESARROLLADORES = 4;
static const char* DESARROLLADORES[NUM_DESARROLLADORES] = {
    "- Alba Rosa Helleon Cardenas",
    "- Angel Daniel Duron Urbina",
    "- Ivana Lin Chenoweth Galaz",
    "- Jesus Axel Sanchez Montoy"
};

static const int NUM_ARTISTAS = 2;
static const char* ARTISTAS[NUM_ARTISTAS] = {
    "- Andrea Camila Pacheco de los Reyes",
    "- Angelica Helleon Cardenas"
};

/**
 * \brief Dibuja una columna: su encabezado (en naranja) y la lista de nombres debajo.
 */
static void dibujarColumna(const char* encabezado, const char* const* nombres, int cantidad,
                           int x, int y, Color color)
{
    dibujarTexto(encabezado, x, y, 28, COLOR_NARANJA);

    for(int i = 0; i < cantidad; i++){
        dibujarTexto(nombres[i], x, y + 44 + i * 34, 22, color);
    }
}

Escena_Estado ActualizarCreditos()
{
    if(IsKeyPressed(KEY_ESCAPE)) return Escena_menu;
    return Escena_creditos;
}

void DibujarCreditos()
{
    // El fondo ya trae el letrero de CREDITOS. Solo si falta se escribe el
    // titulo a mano, para que la pantalla no quede sin nombre.
    Texture2D fondo = fondoCreditos();
    dibujarFondo(fondo);

    if(fondo.id == 0) dibujarTextoCentrado("CREDITOS", 84, 44, COLOR_TITULO);

    // Sobre el cielo claro el cafe de las letras de madera se lee bien.
    Color texto = (fondo.id != 0) ? COLOR_TEXTO_MADERA : COLOR_TEXTO;

    // Se mide el renglon completo para centrarlo, y luego se dibuja cada
    // pedazo con su color, uno pegado al otro.
    const int TAMANO_INTRO = 22;

    for(int i = 0; i < NUM_RENGLONES_INTRO; i++){
        int anchoNormal  = medirTexto(INTRO_NORMAL[i],  TAMANO_INTRO);
        int anchoNaranja = medirTexto(INTRO_NARANJA[i], TAMANO_INTRO);

        int x = (GetScreenWidth() - anchoNormal - anchoNaranja) / 2;
        int y = 212 + i * 32;

        dibujarTexto(INTRO_NORMAL[i],  x,               y, TAMANO_INTRO, texto);
        dibujarTexto(INTRO_NARANJA[i], x + anchoNormal, y, TAMANO_INTRO, COLOR_NARANJA);
    }

    // Dos columnas, como en el texto original. La de la derecha empieza mas
    // alla del centro porque sus nombres son los mas largos y la de la
    // izquierda no debe chocar con ellos.
    dibujarColumna("Desarrolladores", DESARROLLADORES, NUM_DESARROLLADORES, 170, 380, texto);
    dibujarColumna("Artistas",        ARTISTAS,        NUM_ARTISTAS,        660, 380, texto);

    dibujarTextoCentrado("ESC para volver al menu", 660, 18, texto);
}
