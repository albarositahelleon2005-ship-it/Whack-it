/**
 * \file Sprites.hpp
 * \brief Las animaciones del topo y de la bomba, cargadas una sola vez.
 * \date 13/09/2026
 *
 * Mismo criterio que Iconos.hpp: los dos GIFs se cargan al arrancar el juego y
 * se comparten entre todos los pozos. Cargarlos requiere que la ventana ya
 * esté abierta, así que CargarSprites() se llama desde main.cpp después de
 * InitWindow, nunca antes.
 *
 * Los archivos van en recursos/imagenes/enemigo.gif y recursos/imagenes/
 * bomba.gif. Para cambiar el personaje basta con reemplazar el .gif: el código
 * no depende de cuántos cuadros traiga ni de qué tamaño sea.
 */

#ifndef SPRITES_HPP_INCLUDED
#define SPRITES_HPP_INCLUDED

#include "Animacion.hpp"

/// A qué tamaño se guarda cada cuadro de los gifs, en píxeles.
const int LADO_SPRITE = 128;

/**
 * \brief Carga los dos GIFs. Llamar una sola vez, después de InitWindow.
 *
 * Tarda un momento (hay que achicar unos 200 cuadros entre los dos), así que
 * conviene llamarla en la carga inicial y no a media partida.
 */
void CargarSprites();

/**
 * \brief Libera los GIFs. Llamar una sola vez, antes de CloseWindow.
 */
void DescargarSprites();

/// La animación del topo (el que hay que golpear).
const Animacion& spriteEnemigo();

/// La animación de la bomba (la que hay que dejar en paz).
const Animacion& spriteBomba();

#endif // SPRITES_HPP_INCLUDED
