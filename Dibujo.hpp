/**
 * \file Dibujo.hpp
 * \brief Utilidades de dibujo que comparten todas las pantallas.
 * \date 29/07/2026
 */

#ifndef DIBUJO_HPP_INCLUDED
#define DIBUJO_HPP_INCLUDED

#include "raylib.h"

/**
 * \brief Dibuja un texto centrado horizontalmente en la ventana.
 *
 * raylib solo sabe dibujar texto a partir de una esquina, as&iacute; que centrar es
 * medir el texto y restarle la mitad al centro de la ventana. Se usa en todas
 * las pantallas, por eso vive aqu&iacute; y no dentro de una en particular.
 *
 * \param texto  Cadena a dibujar.
 * \param y      Coordenada vertical de la esquina superior del texto.
 * \param tamano Tama&ntilde;o de la fuente en p&iacute;xeles.
 * \param color  Color del texto.
 */
void dibujarTextoCentrado(const char* texto, int y, int tamano, Color color);

/**
 * \brief Dibuja una pantalla provisional con su nombre y la ayuda para volver.
 *
 * Sirve de relleno para las escenas que todav&iacute;a no existen, para poder navegar
 * el men&uacute; completo desde el primer d&iacute;a. Se ir&aacute; borrando conforme cada pantalla
 * se implemente de verdad.
 *
 * \param nombre Nombre de la pantalla que ir&aacute; en su lugar.
 */
void dibujarPantallaPendiente(const char* nombre);

#endif // DIBUJO_HPP_INCLUDED
