/**
 * \file Sprites.cpp
 * \brief Implementación de la carga de los GIFs del juego.
 * \date 13/09/2026
 */

#include "Sprites.hpp"

// Los cuadros por segundo salen del gif original: enemigo.gif tiene sus
// cuadros a 100 ms (10 por segundo) y bomba.gif a 60 ms (unos 16.7). raylib no
// devuelve esos tiempos al cargar, asi que se anotan aqui. Si se cambia un gif
// por otro mas rapido o mas lento, este es el numero a tocar.
static const float FPS_ENEMIGO = 10.0f;
static const float FPS_BOMBA   = 16.7f;

static Animacion animEnemigo;
static Animacion animBomba;

void CargarSprites()
{
    animEnemigo = cargarAnimacion("recursos/imagenes/enemigo.gif", LADO_SPRITE, FPS_ENEMIGO);
    animBomba   = cargarAnimacion("recursos/imagenes/bomba.gif",   LADO_SPRITE, FPS_BOMBA);
}

void DescargarSprites()
{
    descargarAnimacion(animEnemigo);
    descargarAnimacion(animBomba);
}

const Animacion& spriteEnemigo()
{
    return animEnemigo;
}

const Animacion& spriteBomba()
{
    return animBomba;
}
