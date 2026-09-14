/**
 * \file Iconos.hpp
 * \brief Carga las imágenes que se usan como botón (regresar, pausa, ajustes).
 * \date 13/09/2026
 *
 * Las texturas se cargan una sola vez, al arrancar el juego, y se comparten
 * entre pantallas: por eso viven aquí y no dentro de Menu.cpp o Pausa.cpp.
 * Cargarlas requiere que la ventana ya esté abierta (InitWindow), así que
 * CargarIconos() se llama desde main.cpp después de InitWindow, nunca antes.
 *
 * Las imágenes van en recursos/imagenes/. Para agregar un ícono nuevo:
 *   1. Pon el archivo .png en esa carpeta.
 *   2. Agrega una Texture2D en Iconos.cpp y su LoadTexture en CargarIconos().
 *   3. Agrega su UnloadTexture en DescargarIconos().
 *   4. Declara aquí la función que lo devuelve.
 */

#ifndef ICONOS_HPP_INCLUDED
#define ICONOS_HPP_INCLUDED

#include "raylib.h"

/**
 * \brief Carga todas las imágenes de botones. Llamar una sola vez, después
 * de InitWindow.
 */
void CargarIconos();

/**
 * \brief Libera las texturas. Llamar una sola vez, antes de CloseWindow.
 */
void DescargarIconos();

/// Ícono de flecha, para el botón de regresar (esquina superior izquierda).
Texture2D iconoRegresar();

/// Ícono de dos barras, para pausar durante la partida y para el panel de pausa.
Texture2D iconoPausa();

/// Ícono de engrane, para abrir la ventana de ajustes (esquina superior derecha).
Texture2D iconoAjustes();

#endif // ICONOS_HPP_INCLUDED
