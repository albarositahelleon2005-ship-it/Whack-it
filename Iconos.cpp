/**
 * \file Iconos.cpp
 * \brief Implementación de la carga de íconos.
 * \date 13/09/2026
 */

#include "Iconos.hpp"

// 'static' a nivel de archivo: nadie fuera de aqui toca estas texturas
// directamente, solo a traves de las funciones de abajo. Asi, si un dia cambia
// como se guardan, solo hay que tocar este archivo.
static Texture2D texturaRegresar;
static Texture2D texturaPausa;
static Texture2D texturaAjustes;

void CargarIconos()
{
    texturaRegresar = LoadTexture("recursos/imagenes/icono_regresar.png");
    texturaPausa    = LoadTexture("recursos/imagenes/icono_pausa.png");
    texturaAjustes  = LoadTexture("recursos/imagenes/icono_ajustes.png");
}

void DescargarIconos()
{
    UnloadTexture(texturaRegresar);
    UnloadTexture(texturaPausa);
    UnloadTexture(texturaAjustes);
}

Texture2D iconoRegresar()
{
    return texturaRegresar;
}

Texture2D iconoPausa()
{
    return texturaPausa;
}

Texture2D iconoAjustes()
{
    return texturaAjustes;
}
