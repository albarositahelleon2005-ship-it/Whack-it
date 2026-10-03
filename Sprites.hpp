/**
 * \file Sprites.hpp
 * \brief Las animaciones del topo, el topo dorado y la bomba, y las imágenes de
 * cada uno al ser golpeado, cargadas una sola vez.
 * \date 13/09/2026
 *
 * Mismo criterio que Iconos.hpp: las imágenes se cargan al arrancar el juego y
 * se comparten entre todos los pozos. Cargarlos requiere que la ventana ya
 * esté abierta, así que CargarSprites() se llama desde main.cpp después de
 * InitWindow, nunca antes.
 *
 * Los archivos van en recursos/imagenes/. El código no depende de cuántos
 * cuadros traiga cada uno ni de qué tamaño sea, pero sí del nombre exacto con
 * extensión: si la imagen nueva es .png donde antes había un .gif, hay que
 * corregir la ruta en Sprites.cpp.
 */

#ifndef SPRITES_HPP_INCLUDED
#define SPRITES_HPP_INCLUDED

#include "Animacion.hpp"

/// A qué tamaño se guarda cada cuadro de los gifs, en píxeles. Un poco más que
/// LADO_OBJETO (VistaTablero.cpp) para que al dibujarlos se achiquen en vez de
/// estirarse, que es lo que los vería borrosos.
const int LADO_SPRITE = 160;

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

/// La animación del topo dorado.
const Animacion& spritePremium();

/// Imagen fija del topo recién aplastado.
const Animacion& spriteEnemigoAplastado();

/// Imagen fija del topo dorado recién aplastado.
const Animacion& spritePremiumAplastado();

/// Imagen fija de la bomba explotando.
const Animacion& spriteBombaExplotada();

#endif // SPRITES_HPP_INCLUDED
