/**
 * \file Dibujo.hpp
 * \brief Utilidades de dibujo que comparten todas las pantallas.
 * \date 29/07/2026
 */

#ifndef DIBUJO_HPP_INCLUDED
#define DIBUJO_HPP_INCLUDED

#include "raylib.h"

/**
 * \brief Carga la tipograf&iacute;a del juego (recursos/fuente/ARCO_juego.ttf).
 *
 * Llamar una sola vez, despu&eacute;s de InitWindow. Si el archivo falta, el texto
 * se sigue dibujando con la fuente de f&aacute;brica de raylib.
 */
void CargarFuente();

/// Libera la tipograf&iacute;a. Llamar una sola vez, antes de CloseWindow.
void DescargarFuente();

/**
 * \brief Dibuja un texto con la tipograf&iacute;a del juego.
 *
 * Reemplaza a DrawText en todo el proyecto: as&iacute; la fuente se cambia en un
 * solo lugar. Mismos par&aacute;metros que DrawText.
 *
 * \param texto  Cadena a dibujar.
 * \param x      Esquina izquierda.
 * \param y      Esquina superior.
 * \param tamano Alto de la letra en p&iacute;xeles.
 * \param color  Color del texto.
 */
void dibujarTexto(const char* texto, int x, int y, int tamano, Color color);

/**
 * \brief Cu&aacute;nto mide de ancho un texto con la tipograf&iacute;a del juego.
 *
 * Reemplaza a MeasureText, que mide con la fuente de f&aacute;brica y dar&iacute;a anchos
 * equivocados para centrar.
 */
int medirTexto(const char* texto, int tamano);

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
 * \brief Dibuja una imagen estirada a toda la ventana, como fondo.
 *
 * Se estira en vez de dibujarla a su tama&ntilde;o para que una imagen de otra
 * resoluci&oacute;n siga cubriendo todo sin tocar c&oacute;digo. Si la textura no carg&oacute;
 * (id 0) no dibuja nada y queda el color de fondo de siempre.
 *
 * \param fondo Textura a dibujar.
 */
void dibujarFondo(Texture2D fondo);

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
