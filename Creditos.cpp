/**
 * \file Creditos.cpp
 * \brief Implementación de la pantalla de créditos.
 * \date 06/09/2026
 */

#include "raylib.h"

#include "Creditos.hpp"
#include "Dibujo.hpp"

Escena_Estado ActualizarCreditos()
{
    if(IsKeyPressed(KEY_ESCAPE)) return Escena_menu;
    return Escena_creditos;
}

void DibujarCreditos()
{
    dibujarPantallaPendiente("CREDITOS");
}
