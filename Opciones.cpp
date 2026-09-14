/**
 * \file Opciones.cpp
 * \brief Implementación de la pantalla de opciones.
 * \date 06/09/2026
 */

#include "raylib.h"

#include "Opciones.hpp"
#include "Dibujo.hpp"

Escena_Estado ActualizarOpciones()
{
    if(IsKeyPressed(KEY_ESCAPE)) return Escena_menu;
    return Escena_opciones;
}

void DibujarOpciones()
{
    dibujarPantallaPendiente("OPCIONES");
}
