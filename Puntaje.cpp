/**
 * \file Puntaje.cpp
 * \brief Implementación (provisional) de la pantalla de mejores puntajes.
 * \date 06/09/2026
 */

#include "raylib.h"

#include "Puntaje.hpp"
#include "Dibujo.hpp"

Escena_Estado ActualizarPuntaje()
{
    if(IsKeyPressed(KEY_ESCAPE)) return Escena_menu;
    return Escena_puntajes;
}

void DibujarPuntaje()
{
    dibujarPantallaPendiente("MEJORES PUNTAJES");
}
