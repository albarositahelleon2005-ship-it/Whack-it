/**
 * \file Instrucciones.cpp
 * \brief Implementación de la pantalla de instrucciones.
 * \date 13/09/2026
 */

#include "raylib.h"

#include "Instrucciones.hpp"
#include "Dibujo.hpp"
#include "Dificultad.hpp"
#include "Tema.hpp"

// Las reglas se escriben aqui como puro texto, en un arreglo, para que
// agregar o quitar un renglon sea agregar o quitar una linea de datos. Van sin
// acentos porque la fuente que trae raylib de fabrica solo cubre ASCII.
static const int NUM_RENGLONES = 8;

static const char* RENGLONES[NUM_RENGLONES] = {
    "Golpea a los topos con clic izquierdo antes de que se escondan.",
    "",
    "Cada topo golpeado suma 1 punto y alarga tu racha.",
    "Golpear una bomba resta 2 puntos, quita una vida y borra la racha.",
    "Si un topo se esconde sin que lo golpees pierdes la racha, pero no vida.",
    "El puntaje nunca baja de cero.",
    "",
    "La partida termina cuando te quedas sin vidas."
};

Escena_Estado ActualizarInstrucciones()
{
    if(IsKeyPressed(KEY_ESCAPE)) return Escena_menu;
    return Escena_instrucciones;
}

void DibujarInstrucciones()
{
    dibujarTextoCentrado("INSTRUCCIONES", 84, 44, COLOR_TITULO);

    const int MARGEN = 180;

    for(int i = 0; i < NUM_RENGLONES; i++){
        DrawText(RENGLONES[i], MARGEN, 170 + i * 34, 22, COLOR_TEXTO);
    }

    // La tabla de niveles se arma desde reglasDe(), no a mano: si alguien
    // cambia el balance en Dificultad.cpp, esta pantalla se entera sola.
    DrawText("Niveles", MARGEN, 470, 26, COLOR_SELECCION);

    for(int i = 0; i < NUM_DIFICULTADES; i++){

        ReglasDificultad reglas = reglasDe((Dificultad)i);

        DrawText(TextFormat("%-8s  %2d pozos   %d vida(s)   visibles de %.1f a %.1f s",
                            NOMBRES_DIFICULTAD[i], reglas.pozos, reglas.vidas,
                            reglas.visibleInicial, reglas.visibleMinimo),
                 MARGEN, 512 + i * 32, 22, COLOR_TEXTO);
    }

    dibujarTextoCentrado("ESC para volver al menu", 660, 18, COLOR_TENUE);
}
